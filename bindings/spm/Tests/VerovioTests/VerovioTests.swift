//
//  File.swift
//  
//
//  Created by Joshua Elkins on 8/2/21.
//

import Foundation
import XCTest
import Verovio


class VerovioTests: XCTestCase {

    func testCanAccessVerovio() throws {
        let testDataURL = Bundle.module.url(forResource: "TestData", withExtension: "musicxml")!
        let svg = Verovio().renderFirstPage(url: testDataURL, options: [:])
        XCTAssert(svg.count > 0, "SVG was zero length")
    }
}
