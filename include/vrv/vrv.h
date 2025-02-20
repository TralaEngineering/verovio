/////////////////////////////////////////////////////////////////////////////
// Name:        vrv.h
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_H__
#define __VRV_H__

#include <cstring>
#include <map>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <vector>

#ifndef _WIN32
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "atttypes.h"

namespace vrv {

class Glyph;
class Object;

/**
 * The following functions are helpers for formatting, conversion, or logging.
 * Most of them differ if they are used in the command line tool or in emscripten
 */

void LogDebug(const char *fmt, ...);
void LogError(const char *fmt, ...);
void LogMessage(const char *fmt, ...);
void LogWarning(const char *fmt, ...);

/**
 * Member and functions specific to loging that uses a vector of string to buffer the logs.
 */
enum consoleLogLevel { CONSOLE_LOG = 0, CONSOLE_INFO, CONSOLE_WARN, CONSOLE_ERROR, CONSOLE_DEBUG };
extern std::vector<std::string> logBuffer;
bool LogBufferContains(const std::string &s);
void LogString(std::string message, consoleLogLevel level);

/**
 * Utility for comparing doubles
 */
bool AreEqual(double dFirstVal, double dSecondVal);

/**
 * Extract the uuid from a any uri string
 */
std::string ExtractUuidFragment(std::string refUuid);

/**
 * Utility for converting UTF16 (std::wstring) to UTF-8
 */
std::string UTF16to8(const std::wstring &in);

/**
 * Utility for converting UTF-8 to UTF16 (std::wstring)
 */
std::wstring UTF8to16(const std::string &in);

/**
 * Format a string using vsnprintf.
 * The maximum length is giving by STRING_FORMAT_MAX_LEN
 */
std::string StringFormat(const char *fmt, ...);
// This is the implementation callable with variable arguments
std::string StringFormatVariable(const char *format, va_list arg);

/**
 * Return a formatted version (####.####.####) of the file version.
 * This can be used for comparing if the file version is < or >
 */
std::string GetFileVersion(int vmaj, int vmin, int vrev);

/**
 * Return the filename (without extension) extracted from the fullpath
 */
std::string GetFilename(std::string fullpath);

/**
 * Return the version number (X.X.X)
 */
std::string GetVersion();

/**
 * Encode the integer value using the specified base (max is 62)
 * Base 36 uses 0-9 and a-z, base 62 also A-Z.
 */
std::string BaseEncodeInt(int value, int base);

/**
 *
 */
extern bool logging;
extern bool loggingToBuffer;

/**
 * Functions for logging in milliseconds the elapsed time of an
 * operation (for debugging purposes).
 * LogElapsedTimeStart needs to be called before the operation
 *
 * Ex:
 *
 * LogElapsedTimeStart();
 * ... Do something
 * LogElapsedTimeEnd("name of the operation");
 */
extern struct timeval start;
void LogElapsedTimeStart();
void LogElapsedTimeEnd(const char *msg = "unspecified operation");

/**
 * Method that simply checks if the Object is not NULL
 * Also asserts it for stopping in debug mode
 */
bool Check(Object *object);

//----------------------------------------------------------------------------
// Resources
//----------------------------------------------------------------------------

/**
 * This class provides static resource values.
 * The default values can be changed by setters.
 */

class Resources {
public:
    using StyleAttributes = std::pair<data_FONTWEIGHT, data_FONTSTYLE>;
    using GlyphMap = std::map<wchar_t, Glyph>;
    using GlyphTextMap = std::map<StyleAttributes, GlyphMap>;

    //----------------//
    // Static methods //
    //----------------//

    /**
     * @name Setters and getters for static environment variables
     */
    ///@{
    /** Resource path */
    static std::string GetPath() { return s_path; }
    static void SetPath(const std::string &path) { s_path = path; }
    /** Init the SMufL music and text fonts */
    static bool InitFonts();
    /** Init the text font (bounding boxes and ASCII only) */
    static bool InitTextFont(const std::string &fontName, const StyleAttributes &style);
    /** Select a particular font */
    static bool SetFont(const std::string &fontName);
    /** Returns the glyph (if exists) for a glyph code in the current SMuFL font */
    static Glyph *GetGlyph(wchar_t smuflCode);
    /** Returns the glyph (if exists) for a glyph name in the current SMuFL font */
    static Glyph *GetGlyph(const std::string &smuflName);
    /** Returns the glyph (if exists) for a glyph name in the current SMuFL font */
    static wchar_t GetGlyphCode(const std::string &smuflName);
    /** Set current text style*/
    static void SelectTextFont(data_FONTWEIGHT fontWeight, data_FONTSTYLE fontStyle);
    /** Returns the glyph (if exists) for the text font (bounding box and ASCII only) */
    static Glyph *GetTextGlyph(wchar_t code);
    ///@}

private:
    static bool LoadFont(const std::string &fontName);

private:
    //----------------//
    // Static members //
    //----------------//

    /** The path to the resources directory (e.g., for the svg/ subdirectory with fonts as XML */
    static thread_local std::string s_path;
    /** The loaded SMuFL font */
    static thread_local GlyphMap s_font;
    /** A text font used for bounding box calculations */
    static thread_local GlyphTextMap s_textFont;
    static thread_local StyleAttributes s_currentStyle;
    static const StyleAttributes k_defaultStyle;
    /**
     * A map of glyph name / code
     * The map in generated by ./fonts/generate_all.sh into ./src/smufl.cpp
     */
    static const std::map<std::string, wchar_t> s_smuflNames;
};

//----------------------------------------------------------------------------
// Base64 code borrowed
//----------------------------------------------------------------------------

std::string Base64Encode(unsigned char const *bytesToEncode, unsigned int len);
std::vector<unsigned char> Base64Decode(std::string const &encodedString);

} // namespace vrv

#endif
