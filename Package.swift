// swift-tools-version:5.4

import PackageDescription

let package = Package(
    name: "Verovio",
    products: [
        .library(
            name: "Verovio",
            type: .dynamic,
            targets: ["Verovio"]
        ),
    ],
    targets: [
        .target(
            name: "Verovio",
            dependencies: [],
            path: ".",
            exclude: [
                "setup.py",
                "appveyor.yml",
                "MANIFEST.in",
                "COPYING.LESSER",
                "CHANGELOG.md",
                "codemeta.json",
                "Verovio.podspec",
                "COPYING",
                "README.md",
                "fonts/",
                "bindings/android",
                "bindings/c",
                "bindings/iOS",
                "bindings/java",
                "bindings/python",
                "bindings/qt",
                "doc/",
                "emscripten/",
                "cmake/",
                "tools/",
                "include/json/LICENSE",
                "include/json/Makefile",
                "include/json/README.textile",
                "libmei/atts_fingering.cpp",
                "libmei/atts_usersymbols.cpp",
                "libmei/atts_harmony.cpp",
                "libmei/atts_performance.cpp",
                "libmei/atts_header.cpp",
                "libmei/atts_figtable.cpp",
                "libmei/atts_edittrans.cpp",
                "bindings/spm/create_spm_header_symlinks.sh",
                "bindings/spm/Tests/VerovioTests"
            ],
            sources: [
                "bindings/spm/Sources/Verovio/",
                "libmei/",
                "include/",
                "include/hum/",
                "include/json/",
                "include/midi/",
                "include/pugi/",
                "include/utf8/",
                "include/vrv/",
                "include/zip/",
                "src/",
                "src/hum/",
                "src/json/",
                "src/midi/",
                "src/pugi/"
            ],
            resources: [.copy("data/")],
            publicHeadersPath: "bindings/spm/Sources/Verovio",
            cxxSettings: [
                .headerSearchPath("libmei/"),
                .headerSearchPath("include/**"),
                .headerSearchPath("src/**"),
                .define("COCOAPODS"),
            ],
            linkerSettings: nil
        ),
        .testTarget(name: "VerovioTests",
                    dependencies: ["Verovio"],
                    path: "bindings/spm/Tests/VerovioTests",
                    exclude: [],
                    sources: nil,
                    resources: [
                        .process("TestData.musicxml")
                    ],
                    cSettings: nil,
                    cxxSettings: nil,
                    swiftSettings: nil,
                    linkerSettings: nil)
    ],
    cxxLanguageStandard: .cxx17
)
