/*
 * JPEG_CONVERTER.h
 *
 *  Created on: Jul 28, 2017
 *      Author: babu_wanyeki
 */

// NanoJPEG -- KeyJ's Tiny Baseline JPEG Decoder
// version 1.3.5 (2016-11-14)

// COMPILE-TIME CONFIGURATION
// ==========================
//
// The following aspects of NanoJPEG can be controlled with preprocessor
// defines:
//
// _NJ_EXAMPLE_PROGRAM     = Compile a main() function with an example
//                           program.
// _NJ_INCLUDE_HEADER_ONLY = Don't compile anything, just act as a header
//                           file for NanoJPEG. Example:
//                               #define _NJ_INCLUDE_HEADER_ONLY
//                               #include "nanojpeg.c"
//                               int main(void) {
//                                   njInit();
//                                   // your code here
//                                   njDone();
//                               }
// NJ_USE_LIBC=1           = Use the malloc(), free(), memset() and memcpy()
//                           functions from the standard C library (default).
// NJ_USE_LIBC=0           = Don't use the standard C library. In this mode,
//                           external functions njAlloc(), njFreeMem(),
//                           njFillMem() and njCopyMem() need to be defined
//                           and implemented somewhere.
// NJ_USE_WIN32=0          = Normal mode (default).
// NJ_USE_WIN32=1          = If compiling with MSVC for Win32 and
//                           NJ_USE_LIBC=0, NanoJPEG will use its own
//                           implementations of the required C library
//                           functions (default if compiling with MSVC and
//                           NJ_USE_LIBC=0).
// NJ_CHROMA_FILTER=1      = Use the bicubic chroma upsampling filter
//                           (default).
// NJ_CHROMA_FILTER=0      = Use simple pixel repetition for chroma upsampling
//                           (bad quality, but faster and less code).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CAMERA_JPEG_CONVERTER_H_
#define CAMERA_JPEG_CONVERTER_H_



// nj_result_t: Result codes for njDecode().
typedef enum _nj_result {
    NJ_OK = 0,        // no error, decoding successful
    NJ_NO_JPEG,       // not a JPEG file
    NJ_UNSUPPORTED,   // unsupported format
    NJ_OUT_OF_MEM,    // out of memory
    NJ_INTERNAL_ERR,  // internal error
    NJ_SYNTAX_ERROR,  // syntax error
    __NJ_FINISHED,    // used internally, will never be reported
} nj_result_t;

// njInit: Initialize NanoJPEG.
// For safety reasons, this should be called at least one time before using
// using any of the other NanoJPEG functions.
void njInit(void);

// njDecode: Decode a JPEG image.
// Decodes a memory dump of a JPEG file into internal buffers.
// Parameters:
//   jpeg = The pointer to the memory dump.
//   size = The size of the JPEG file.
// Return value: The error code in case of failure, or NJ_OK (zero) on success.
nj_result_t njDecode(const void* jpeg, const int size);

// njGetWidth: Return the width (in pixels) of the most recently decoded
// image. If njDecode() failed, the result of njGetWidth() is undefined.
int njGetWidth(void);

// njGetHeight: Return the height (in pixels) of the most recently decoded
// image. If njDecode() failed, the result of njGetHeight() is undefined.
int njGetHeight(void);

// njIsColor: Return 1 if the most recently decoded image is a color image
// (RGB) or 0 if it is a grayscale image.
int njIsColor(void);

// njGetImage: Returns the decoded image data.
// Returns a pointer to the most recently image. The memory layout it byte-
// oriented, top-down, without any padding between lines. Pixels of color
// images will be stored as three consecutive bytes for the red, green and
// blue channels. This data format is thus compatible with the PGM or PPM
// file formats and the OpenGL texture formats GL_LUMINANCE8 or GL_RGB8.
// If njDecode() failed, the result of njGetImage() is undefined.
unsigned char* njGetImage(void);

// njGetImageSize: Returns the size (in bytes) of the image data returned
// by njGetImage(). If njDecode() failed, the result of njGetImageSize() is
// undefined.
int njGetImageSize(void);

// njDone: Uninitialize NanoJPEG.
// Resets NanoJPEG's internal state and frees all memory that has been
// allocated at run-time by NanoJPEG. It is still possible to decode another
// image after a njDone() call.
void njDone(void);

#endif//_NANOJPEG_H


///////////////////////////////////////////////////////////////////////////////
// CONFIGURATION SECTION                                                     //
// adjust the default settings for the NJ_ defines here                      //
///////////////////////////////////////////////////////////////////////////////

#ifndef NJ_USE_LIBC
#define NJ_USE_LIBC 1
#endif

#ifndef NJ_USE_WIN32

#define NJ_USE_WIN32 0
#endif


#ifndef NJ_CHROMA_FILTER
#define NJ_CHROMA_FILTER 1







#endif /* CAMERA_JPEG_CONVERTER_H_ */
