//
//  VerovioTests.mm
//  
//
//  Created by Joshua Elkins on 7/30/21.
//

@import XCTest;
@import Verovio;


@interface VerovioTests : XCTestCase
@end


@implementation VerovioTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testExample {
    vrv::Toolkit * toolkit = new vrv::Toolkit(false);
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
}

@end
