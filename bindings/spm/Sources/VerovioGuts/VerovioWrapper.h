#import <Foundation/Foundation.h>


@interface Verovio: NSObject

- (instancetype)init;
- (NSString *)renderFirstPageForURL:(NSURL *)url withOptions:(NSDictionary *)options NS_SWIFT_NAME(renderFirstPage(url:options:));

@end
