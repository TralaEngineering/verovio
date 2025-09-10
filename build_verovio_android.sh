#!/bin/bash
set -e

# -------------------------------
# Configuration
# -------------------------------
BASE_DIR=$(cd "$(dirname "$0")" && pwd)
BUILD_DIR="$BASE_DIR/build"
OUTPUT_LIBS="$BASE_DIR/libs"

echo "BASE DIR $BASE_DIR"
echo "BUILD DIR $BUILD_DIR"

PLATFORM_VERSION=27
ABIS=("armeabi-v7a" "arm64-v8a" "x86" "x86_64")

if [ -z "$ANDROID_NDK" ]; then
    echo "Please set ANDROID_NDK to the Android NDK folder"
    exit 1
fi

HOST_OS_ARCH=darwin-x86_64

echo "Cleaning previous builds..."
rm -rf "$BUILD_DIR"
rm -rf "$OUTPUT_LIBS"
mkdir -p "$OUTPUT_LIBS"

# -------------------------------
# Build function
# -------------------------------
build_verovio() {
    ABI=$1

    case $ABI in
        armeabi-v7a)
            TOOLCHAIN_PREFIX=armv7a-linux-androideabi
            ARCH=armv7-a
            MARCH_FLAG="-march=armv7-a"
            ;;
        arm64-v8a)
            TOOLCHAIN_PREFIX=aarch64-linux-android
            ARCH=aarch64
            MARCH_FLAG="-march=armv8-a"
            ;;
        x86)
            TOOLCHAIN_PREFIX=i686-linux-android
            ARCH=x86
            MARCH_FLAG="-march=i686"
            ;;
        x86_64)
            TOOLCHAIN_PREFIX=x86_64-linux-android
            ARCH=x86_64
            MARCH_FLAG="-march=x86-64"
            ;;
        *)
            echo "Unknown ABI: $ABI"
            exit 1
            ;;
    esac

    TOOLCHAIN_PATH="$ANDROID_NDK/toolchains/llvm/prebuilt/$HOST_OS_ARCH/bin"
    SYSROOT="$ANDROID_NDK/toolchains/llvm/prebuilt/$HOST_OS_ARCH/sysroot"

    BUILD_ABI_DIR="$BUILD_DIR/$ABI"
    mkdir -p "$BUILD_ABI_DIR"
    cd "$BUILD_ABI_DIR"

    echo "=============================="
    echo "Building Verovio for ABI: $ABI"
    echo "=============================="

    CC="$TOOLCHAIN_PATH/${TOOLCHAIN_PREFIX}${PLATFORM_VERSION}-clang"
    CXX="$TOOLCHAIN_PATH/${TOOLCHAIN_PREFIX}${PLATFORM_VERSION}-clang++"
    AR="$TOOLCHAIN_PATH/llvm-ar"
    RANLIB="$TOOLCHAIN_PATH/llvm-ranlib"
    STRIP="$TOOLCHAIN_PATH/llvm-strip"

    echo "CC=$CC"
    echo "CXX=$CXX"

    # Source files
    SRC_FILES=$(find "$BASE_DIR/src" "$BASE_DIR/libmei" "$BASE_DIR/include" -name "*.cpp")
    SRC_FILES+=" $BASE_DIR/tools/c_wrapper.cpp"

    # Include paths
    INCLUDE_FLAGS="-I$BASE_DIR/include -I$BASE_DIR/include/midi -I$BASE_DIR/include/hum \
-I$BASE_DIR/include/json -I$BASE_DIR/include/pugi -I$BASE_DIR/include/utf8 \
-I$BASE_DIR/include/zip -I$BASE_DIR/include/vrv -I$BASE_DIR/libmei"

    # Compilation flags
    CFLAGS="-O3 -fPIC -DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON -std=c++17 $MARCH_FLAG"
    LDFLAGS="-shared -Wl,-z,max-page-size=16384"

    echo "Compiling..."
    $CXX $CFLAGS $INCLUDE_FLAGS $SRC_FILES $LDFLAGS -o libverovio.so

    # Copy to libs/ABI
    mkdir -p "$OUTPUT_LIBS/$ABI"
    cp libverovio.so "$OUTPUT_LIBS/$ABI/"

    echo "Build for $ABI completed. Library copied to $OUTPUT_LIBS/$ABI"
}

# -------------------------------
# Build all ABIs
# -------------------------------
for ABI in "${ABIS[@]}"; do
    build_verovio $ABI
done

echo "All Verovio Android builds completed. Libraries are in $OUTPUT_LIBS"
