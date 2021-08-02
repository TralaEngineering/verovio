#import "VerovioWrapper.h"
#import <dispatch/dispatch.h>


@implementation Verovio

static dispatch_queue_t renderQueue = dispatch_queue_create("com.rism.Verovio.sheetMusicRender", DISPATCH_QUEUE_SERIAL);

#pragma mark - init & dealloc

- (instancetype)init {
    self = [super init];
    if (self) {
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            NSBundle * verovioBundle = [NSBundle bundleForClass:[self class]];
            NSString * resourcePath = [verovioBundle URLsForResourcesWithExtension:@"xml" subdirectory:@"data"].firstObject.URLByDeletingLastPathComponent.path;
            vrv::Resources::SetPath([resourcePath cStringUsingEncoding:NSUTF8StringEncoding]);
            if (!vrv::Resources::InitFonts()) {
                NSAssert(false, @"Can't init fonts");
            }
        });
    }
    return self;
}

#pragma mark - rendering

- (NSString *)renderFirstPageForURL:(NSURL *)url withOptions:(NSDictionary *)options {
    NSString * __block svg;
    dispatch_sync(renderQueue, ^{
        vrv::Toolkit * toolkit = new vrv::Toolkit(false);
        NSError * error = nil;
        NSData * optionData = [NSJSONSerialization dataWithJSONObject:options options:0 error:&error];
        if (error) {
            NSAssert(false, @"Error while serializing options: %@", error);
        }
        NSString * optionString = [[NSString alloc] initWithData:optionData encoding:NSUTF8StringEncoding];
        std::string cOptions = std::string([optionString cStringUsingEncoding:NSUTF8StringEncoding]);
        toolkit->SetOptions(cOptions);
        std::string filePath = std::string([url.path cStringUsingEncoding: NSUTF8StringEncoding]);
        toolkit->LoadFile(filePath);
        toolkit->RedoLayout();
        svg = [NSString stringWithCString:toolkit->RenderToSVG(1).c_str() encoding:NSUTF8StringEncoding];
        delete toolkit;
    });
    return svg;
}

@end
