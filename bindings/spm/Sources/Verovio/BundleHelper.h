//
//  BundleHelper.h
//  
//
//  Created by Joshua Elkins on 8/2/21.
//

#import <Foundation/Foundation.h>


@interface BundleHelper: NSObject

/// For some reason, SPM's generated bundle accessor helper method
/// isn't accessible from a .mm file (the static function that retrieves
/// the bundle for the current module fails to resolve at link time.)
/// So this simple class, define in a .m / non-ObjC++ file, simply
/// makes the bundle for module available in the .mm file.
+ (NSBundle *)bundleForModule;

@end
