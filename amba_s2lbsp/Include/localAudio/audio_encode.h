/*
This program is distributed under the terms of the 'MIT license'. The text
of this licence follows...

Copyright (c) 2004 J.D.Medhurst (a.k.a. Tixy)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

 
#ifndef __AUDIOENCODE_H__
#define __AUDIOENCODE_H__


#if _DEBUG
#undef DEBUG
#define DEBUG	/**< Defined when compiling code for debugging */
#endif

#if defined(_MSC_VER)
// Compiling for Microsoft Visual C++...
#define BREAKPOINT	{ _asm int 3 }			/**< Invoke debugger */
#define IMPORT		__declspec(dllexport)	/**< Mark a function which is to be imported from a DLL */
#define EXPORT		__declspec(dllexport)	/**< Mark a function to be exported from a DLL */
#define ASSERT(c)	{if(!(c)) BREAKPOINT;}	/**< Assert that expression 'c' is true */
typedef __int64 longlong;
typedef unsigned __int64 ulonglong;

// disable annoying warnings from MSVC...
#pragma warning( disable : 4244 )	/* conversion from x to y, possible loss of data */
#pragma warning( disable : 4514 )	/* unreferenced inline function has been removed */
#pragma warning( disable : 4146 )	/* unary minus operator applied to unsigned type, result still unsigned */
#pragma warning( disable : 4710 )	/* function x not inlined */
#pragma warning( disable : 4355 )   /* 'this' : used in base member initializer list */
#pragma warning( disable : 4512 )   /* assignment operator could not be generated */
#pragma warning( disable : 4800 )	/* forcing value to bool 'true' or 'false' (performance warning) */

#elif defined(__EPOC32__) || defined(__WINS__)
// Compiling for Symbian OS...

#define COMPILE_FOR_SYMBIAN
#include <e32std.h>

#if !defined(__BREAKPOINT)

#if defined(__WINS__)

#if defined(__WINSCW__)
#define BREAKPOINT { _asm byte 0xcc }		/**< Invoke debugger */
#else
#define BREAKPOINT { _asm int 3 }			/**< Invoke debugger */
#endif

#else
#define BREAKPOINT							/**< Invoke debugger */
#endif

#else
#define BREAKPOINT	{__BREAKPOINT()}		/**< Invoke debugger */
#endif

#define IMPORT		IMPORT_C				/**< Mark a function which is to be imported from a DLL */
#define EXPORT		EXPORT_C				/**< Mark a function to be exported from a DLL */

#undef ASSERT
#define ASSERT(c)	{if(!(c)) BREAKPOINT;}	/**< Assert that expression 'c' is true */

#if !defined(DEBUG) && defined(_DEBUG)
#define DEBUG								/**< Defined when compiling code for debugging */
#endif

typedef TInt64 longlong;
typedef TUint64 ulonglong;


#else
// Compiling for unknown system...
#define BREAKPOINT							/**< Invoke debugger */
#define IMPORT								/**< Mark a function which is to be imported from a DLL */
#define EXPORT								/**< Mark a function to be exported from a DLL */
#define ASSERT(c) (void)(c)					/**< Assert that expression 'c' is true */
typedef long long longlong;
typedef unsigned long long ulonglong;

#endif


#ifdef DEBUG
#define ASSERT_DEBUG(c) ASSERT(c)	/**< Assert that expression 'c' is true (when compiled for debugging)*/
#else
#define ASSERT_DEBUG(c) 			/**< Assert that expression 'c' is true (when compiled for debugging)*/
#endif


#ifndef ASSERT_COMPILE
/** Assert, at compile time, that expression 'c' is true. */
#define ASSERT_COMPILE(c) void assert_compile(int assert_compile[(c)?1:-1])
#endif


#ifndef NULL
#define NULL 0		/**< Used to represent a null pointer type */
#endif


/**
@defgroup integers Common - Basic Integer Types.

These definitions will need to be modified on systems where 'char', 'short' and 'int'
have sizes different from 8, 16 and 32 bits.

Note, 'int' is assumed to be in 2s complement format and at least 32 bits in size.
@{
*/
typedef unsigned char		uint8_t;		/**< An 8 bit unsigned integer */
typedef unsigned short		uint16_t;		/**< An 16 bit unsigned integer */
typedef unsigned int		uint32_t;		/**< An 32 bit unsigned integer */
typedef ulonglong			uint64_t;		/**< An 64 bit unsigned integer */
typedef signed char			int8_t;			/**< An 8 bit signed integer (2s complement) */
typedef signed short		int16_t;		/**< An 16 bit signed integer (2s complement) */
typedef signed int			int32_t;		/**< An 32 bit signed integer (2s complement) */
typedef longlong			int64_t;		/**< An 64 bit signed integer (2s complement) */
typedef int					intptr_t;		/**< An signed integer of the same size as a pointer type */
typedef unsigned int		uintptr_t;		/**< An unsigned integer of the same size as a pointer type */
typedef int64_t				intmax_t;		/**< Largest signed integer type */
typedef uint64_t			uintmax_t;		/**< Largest unsigned integer type */
typedef uintptr_t			size_t;			/**< A size of an object or memory region */
typedef intptr_t			ptrdiff_t;		/**< A signed integer which can hold the different between two pointer */

/** @} */ // End of group

ASSERT_COMPILE(sizeof(uintptr_t)==sizeof(void*));
ASSERT_COMPILE(sizeof(intptr_t)==sizeof(void*));


/**
Number of bits in an int.
*/
#define INT_BITS (sizeof(int)*8)


/**
Return log2(x) rounded up to the nearest integer.
@param x An unsigned integer which must be less than 2^32.
*/
#define LOG2(x)	((unsigned)(\
	(unsigned)(x)<=(unsigned)(1<< 0) ?  0 : (unsigned)(x)<=(unsigned)(1<< 1) ?  1 : (unsigned)(x)<=(unsigned)(1<< 2) ?  2 : (unsigned)(x)<=(unsigned)(1<< 3) ?  3 : \
	(unsigned)(x)<=(unsigned)(1<< 4) ?  4 : (unsigned)(x)<=(unsigned)(1<< 5) ?  5 : (unsigned)(x)<=(unsigned)(1<< 6) ?  6 : (unsigned)(x)<=(unsigned)(1<< 7) ?  7 : \
	(unsigned)(x)<=(unsigned)(1<< 8) ?  8 : (unsigned)(x)<=(unsigned)(1<< 9) ?  9 : (unsigned)(x)<=(unsigned)(1<<10) ? 10 : (unsigned)(x)<=(unsigned)(1<<11) ? 11 : \
	(unsigned)(x)<=(unsigned)(1<<12) ? 12 : (unsigned)(x)<=(unsigned)(1<<13) ? 13 : (unsigned)(x)<=(unsigned)(1<<14) ? 14 : (unsigned)(x)<=(unsigned)(1<<15) ? 15 : \
	(unsigned)(x)<=(unsigned)(1<<16) ? 16 : (unsigned)(x)<=(unsigned)(1<<17) ? 17 : (unsigned)(x)<=(unsigned)(1<<18) ? 18 : (unsigned)(x)<=(unsigned)(1<<19) ? 19 : \
	(unsigned)(x)<=(unsigned)(1<<20) ? 20 : (unsigned)(x)<=(unsigned)(1<<21) ? 21 : (unsigned)(x)<=(unsigned)(1<<22) ? 22 : (unsigned)(x)<=(unsigned)(1<<23) ? 23 : \
	(unsigned)(x)<=(unsigned)(1<<24) ? 24 : (unsigned)(x)<=(unsigned)(1<<25) ? 25 : (unsigned)(x)<=(unsigned)(1<<26) ? 26 : (unsigned)(x)<=(unsigned)(1<<27) ? 27 : \
	(unsigned)(x)<=(unsigned)(1<<28) ? 28 : (unsigned)(x)<=(unsigned)(1<<29) ? 29 : (unsigned)(x)<=(unsigned)(1<<30) ? 30 : (unsigned)(x)<=(unsigned)(1<<31) ? 31 : \
	32))

#if 0
#if __GNUC__<4
/**
Calculate address offset of member within a type.
*/
#define offsetof(type,member)	((size_t)(&((type*)256)->member)-256)
#else
#define offsetof(type,member)	__builtin_offsetof(type,member)
#endif

#endif

#if defined(__GNUC__) && defined(_ARM)

/**
Used to GCC "warning: control reaches end of non-void function" in __naked__ functions.
*/
#define dummy_return(type) register type _r0 asm("r0"); asm("" : "=r"(_r0)); return _r0

#endif


//****************************************************************************************************
//****************************************************************************************************
//****************************************************************************************************
//****************************************************************************************************

/**
@defgroup g711 Audio Codec - ITU-T Recomendation G711
@{
*/

/**
@brief A class which implements ITU-T (formerly CCITT) Recomendation G711
	   "Pulse Code Modulation (PCM) of Voice Frequencies"

This encodes and decodes uniform PCM values to/from 8 bit A-law and u-Law values.

Note, the methods in this class use uniform PCM values which are of 16 bits precision,
these are 'left justified' values corresponding to the 13 and 14 bit values described
in G711.

@version 2006-05-20
	- Changed code to use standard typedefs, e.g. replaced uint8 with uint8_t, and made use of size_t.
*/

uint8_t ALawEncodeChang(int16_t pcm16);
int ALawDecodeChang(uint8_t alaw);
uint8_t ULawEncodeChang(int16_t pcm16);
int ULawDecodeChang(uint8_t ulaw);
uint8_t ALawToULawChang(uint8_t alaw);
uint8_t ULawToALawChang(uint8_t ulaw);
unsigned ALawEncode(uint8_t* dst, int16_t* src, size_t srcSize);
unsigned ALawDecode(int16_t* dst, const uint8_t* src, size_t srcSize);
unsigned ULawEncode(uint8_t* dst, int16_t* src, size_t srcSize);
unsigned ULawDecode(int16_t* dst, const uint8_t* src, size_t srcSize);
unsigned ALawToULaw(uint8_t* dst, const uint8_t* src, size_t srcSize);
unsigned ULawToALaw(uint8_t* dst, const uint8_t* src, size_t srcSize);

#endif

