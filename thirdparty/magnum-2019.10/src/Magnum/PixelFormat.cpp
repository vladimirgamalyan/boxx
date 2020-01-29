/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "PixelFormat.h"

#include <Corrade/Containers/ArrayView.h>
#include <Corrade/Utility/Assert.h>
#include <Corrade/Utility/Debug.h>

#include "Magnum/Math/Vector3.h"

namespace Magnum {

UnsignedInt pixelSize(const PixelFormat format) {
    CORRADE_ASSERT(!(UnsignedInt(format) & (1 << 31)),
        "pixelSize(): can't determine size of an implementation-specific format", {});

    #ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic error "-Wswitch"
    #endif
    switch(format) {
        case PixelFormat::R8Unorm:
        case PixelFormat::R8Snorm:
        case PixelFormat::R8Srgb:
        case PixelFormat::R8UI:
        case PixelFormat::R8I:
            return 1;
        case PixelFormat::RG8Unorm:
        case PixelFormat::RG8Snorm:
        case PixelFormat::RG8Srgb:
        case PixelFormat::RG8UI:
        case PixelFormat::RG8I:
        case PixelFormat::R16Unorm:
        case PixelFormat::R16Snorm:
        case PixelFormat::R16UI:
        case PixelFormat::R16I:
        case PixelFormat::R16F:
            return 2;
        case PixelFormat::RGB8Unorm:
        case PixelFormat::RGB8Snorm:
        case PixelFormat::RGB8Srgb:
        case PixelFormat::RGB8UI:
        case PixelFormat::RGB8I:
            return 3;
        case PixelFormat::RGBA8Unorm:
        case PixelFormat::RGBA8Snorm:
        case PixelFormat::RGBA8Srgb:
        case PixelFormat::RGBA8UI:
        case PixelFormat::RGBA8I:
        case PixelFormat::RG16Unorm:
        case PixelFormat::RG16Snorm:
        case PixelFormat::RG16UI:
        case PixelFormat::RG16I:
        case PixelFormat::RG16F:
        case PixelFormat::R32UI:
        case PixelFormat::R32I:
        case PixelFormat::R32F:
            return 4;
        case PixelFormat::RGB16Unorm:
        case PixelFormat::RGB16Snorm:
        case PixelFormat::RGB16UI:
        case PixelFormat::RGB16I:
        case PixelFormat::RGB16F:
            return 6;
        case PixelFormat::RGBA16Unorm:
        case PixelFormat::RGBA16Snorm:
        case PixelFormat::RGBA16UI:
        case PixelFormat::RGBA16I:
        case PixelFormat::RGBA16F:
        case PixelFormat::RG32UI:
        case PixelFormat::RG32I:
        case PixelFormat::RG32F:
            return 8;
        case PixelFormat::RGB32UI:
        case PixelFormat::RGB32I:
        case PixelFormat::RGB32F:
            return 12;
        case PixelFormat::RGBA32UI:
        case PixelFormat::RGBA32I:
        case PixelFormat::RGBA32F:
            return 16;
    }
    #ifdef __GNUC__
    #pragma GCC diagnostic pop
    #endif

    CORRADE_ASSERT(false, "pixelSize(): invalid format" << format, {});
}

#ifndef DOXYGEN_GENERATING_OUTPUT
Debug& operator<<(Debug& debug, const PixelFormat value) {
    if(isPixelFormatImplementationSpecific(value)) {
        return debug << "PixelFormat::ImplementationSpecific(" << Debug::nospace << reinterpret_cast<void*>(pixelFormatUnwrap(value)) << Debug::nospace << ")";
    }

    #ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic error "-Wswitch"
    #endif
    switch(value) {
        /* LCOV_EXCL_START */
        #define _c(value) case PixelFormat::value: return debug << "PixelFormat::" #value;
        _c(R8Unorm)
        _c(RG8Unorm)
        _c(RGB8Unorm)
        _c(RGBA8Unorm)
        _c(R8Snorm)
        _c(RG8Snorm)
        _c(RGB8Snorm)
        _c(RGBA8Snorm)
        _c(R8Srgb)
        _c(RG8Srgb)
        _c(RGB8Srgb)
        _c(RGBA8Srgb)
        _c(R8UI)
        _c(RG8UI)
        _c(RGB8UI)
        _c(RGBA8UI)
        _c(R8I)
        _c(RG8I)
        _c(RGB8I)
        _c(RGBA8I)
        _c(R16Unorm)
        _c(RG16Unorm)
        _c(RGB16Unorm)
        _c(RGBA16Unorm)
        _c(R16Snorm)
        _c(RG16Snorm)
        _c(RGB16Snorm)
        _c(RGBA16Snorm)
        _c(R16UI)
        _c(RG16UI)
        _c(RGB16UI)
        _c(RGBA16UI)
        _c(R16I)
        _c(RG16I)
        _c(RGB16I)
        _c(RGBA16I)
        _c(R32UI)
        _c(RG32UI)
        _c(RGB32UI)
        _c(RGBA32UI)
        _c(R32I)
        _c(RG32I)
        _c(RGB32I)
        _c(RGBA32I)
        _c(R16F)
        _c(RG16F)
        _c(RGB16F)
        _c(RGBA16F)
        _c(R32F)
        _c(RG32F)
        _c(RGB32F)
        _c(RGBA32F)
        #undef _c
        /* LCOV_EXCL_STOP */
    }
    #ifdef __GNUC__
    #pragma GCC diagnostic pop
    #endif

    return debug << "PixelFormat(" << Debug::nospace << reinterpret_cast<void*>(UnsignedInt(value)) << Debug::nospace << ")";
}
#endif

namespace {

#ifndef DOXYGEN_GENERATING_OUTPUT /* It gets *really* confused */
constexpr UnsignedShort CompressedBlockData[] {
    /* Assuming w/h/d/s is never larger than 16 (and never zero), each number
       has 1 subtracted and packed into four bits, 16 bits in total. The size
       is supplied in bits, so first divide by eight and then subtract 1. For
       the currently ~100 supported formats that makes this table to be about
       256 bytes.*/
    #define _c(format, width, height, depth, size) \
        ((width - 1) << 12) | \
        ((height - 1) << 8) | \
        ((depth - 1) << 4) | \
        ((size >> 3) - 1),
    #include "Magnum/Implementation/compressedPixelFormatMapping.hpp"
    #undef _s
    #undef _c
};
#endif

}

Vector3i compressedBlockSize(const CompressedPixelFormat format) {
    CORRADE_ASSERT(!(UnsignedInt(format) & (1 << 31)),
        "compressedBlockSize(): can't determine size of an implementation-specific format", {});

    CORRADE_ASSERT(UnsignedInt(format) - 1 < Containers::arraySize(CompressedBlockData),
        "compressedBlockSize(): invalid format" << format, {});
    const UnsignedInt data = CompressedBlockData[UnsignedInt(format) - 1];
    return {
        (Int(data >> 12) & 0xf) + 1,
        (Int(data >>  8) & 0xf) + 1,
        (Int(data >>  4) & 0xf) + 1,
    };
}

UnsignedInt compressedBlockDataSize(const CompressedPixelFormat format) {
    CORRADE_ASSERT(!(UnsignedInt(format) & (1 << 31)),
        "compressedBlockDataSize(): can't determine size of an implementation-specific format", {});

    CORRADE_ASSERT(UnsignedInt(format) - 1 < Containers::arraySize(CompressedBlockData),
        "compressedBlockDataSize(): invalid format" << format, {});
    return (CompressedBlockData[UnsignedInt(format) - 1] & 0xf) + 1;
}

#ifndef DOXYGEN_GENERATING_OUTPUT
Debug& operator<<(Debug& debug, const CompressedPixelFormat value) {
    if(isCompressedPixelFormatImplementationSpecific(value)) {
        return debug << "CompressedPixelFormat::ImplementationSpecific(" << Debug::nospace << reinterpret_cast<void*>(compressedPixelFormatUnwrap(value)) << Debug::nospace << ")";
    }

    #ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic error "-Wswitch"
    #endif
    switch(value) {
        /* LCOV_EXCL_START */
        #define _c(value) case CompressedPixelFormat::value: return debug << "CompressedPixelFormat::" #value;
        _c(Bc1RGBUnorm)
        _c(Bc1RGBSrgb)
        _c(Bc1RGBAUnorm)
        _c(Bc1RGBASrgb)
        _c(Bc2RGBAUnorm)
        _c(Bc2RGBASrgb)
        _c(Bc3RGBAUnorm)
        _c(Bc3RGBASrgb)
        _c(Bc4RUnorm)
        _c(Bc4RSnorm)
        _c(Bc5RGUnorm)
        _c(Bc5RGSnorm)
        _c(Bc6hRGBUfloat)
        _c(Bc6hRGBSfloat)
        _c(Bc7RGBAUnorm)
        _c(Bc7RGBASrgb)
        _c(EacR11Unorm)
        _c(EacR11Snorm)
        _c(EacRG11Unorm)
        _c(EacRG11Snorm)
        _c(Etc2RGB8Unorm)
        _c(Etc2RGB8Srgb)
        _c(Etc2RGB8A1Unorm)
        _c(Etc2RGB8A1Srgb)
        _c(Etc2RGBA8Unorm)
        _c(Etc2RGBA8Srgb)
        _c(Astc4x4RGBAUnorm)
        _c(Astc4x4RGBASrgb)
        _c(Astc4x4RGBAF)
        _c(Astc5x4RGBAUnorm)
        _c(Astc5x4RGBASrgb)
        _c(Astc5x4RGBAF)
        _c(Astc5x5RGBAUnorm)
        _c(Astc5x5RGBASrgb)
        _c(Astc5x5RGBAF)
        _c(Astc6x5RGBAUnorm)
        _c(Astc6x5RGBASrgb)
        _c(Astc6x5RGBAF)
        _c(Astc6x6RGBAUnorm)
        _c(Astc6x6RGBASrgb)
        _c(Astc6x6RGBAF)
        _c(Astc8x5RGBAUnorm)
        _c(Astc8x5RGBASrgb)
        _c(Astc8x5RGBAF)
        _c(Astc8x6RGBAUnorm)
        _c(Astc8x6RGBASrgb)
        _c(Astc8x6RGBAF)
        _c(Astc8x8RGBAUnorm)
        _c(Astc8x8RGBASrgb)
        _c(Astc8x8RGBAF)
        _c(Astc10x5RGBAUnorm)
        _c(Astc10x5RGBASrgb)
        _c(Astc10x5RGBAF)
        _c(Astc10x6RGBAUnorm)
        _c(Astc10x6RGBASrgb)
        _c(Astc10x6RGBAF)
        _c(Astc10x8RGBAUnorm)
        _c(Astc10x8RGBASrgb)
        _c(Astc10x8RGBAF)
        _c(Astc10x10RGBAUnorm)
        _c(Astc10x10RGBASrgb)
        _c(Astc10x10RGBAF)
        _c(Astc12x10RGBAUnorm)
        _c(Astc12x10RGBASrgb)
        _c(Astc12x10RGBAF)
        _c(Astc12x12RGBAUnorm)
        _c(Astc12x12RGBASrgb)
        _c(Astc12x12RGBAF)
        _c(Astc3x3x3RGBAUnorm)
        _c(Astc3x3x3RGBASrgb)
        _c(Astc3x3x3RGBAF)
        _c(Astc4x3x3RGBAUnorm)
        _c(Astc4x3x3RGBASrgb)
        _c(Astc4x3x3RGBAF)
        _c(Astc4x4x3RGBAUnorm)
        _c(Astc4x4x3RGBASrgb)
        _c(Astc4x4x3RGBAF)
        _c(Astc4x4x4RGBAUnorm)
        _c(Astc4x4x4RGBASrgb)
        _c(Astc4x4x4RGBAF)
        _c(Astc5x4x4RGBAUnorm)
        _c(Astc5x4x4RGBASrgb)
        _c(Astc5x4x4RGBAF)
        _c(Astc5x5x4RGBAUnorm)
        _c(Astc5x5x4RGBASrgb)
        _c(Astc5x5x4RGBAF)
        _c(Astc5x5x5RGBAUnorm)
        _c(Astc5x5x5RGBASrgb)
        _c(Astc5x5x5RGBAF)
        _c(Astc6x5x5RGBAUnorm)
        _c(Astc6x5x5RGBASrgb)
        _c(Astc6x5x5RGBAF)
        _c(Astc6x6x5RGBAUnorm)
        _c(Astc6x6x5RGBASrgb)
        _c(Astc6x6x5RGBAF)
        _c(Astc6x6x6RGBAUnorm)
        _c(Astc6x6x6RGBASrgb)
        _c(Astc6x6x6RGBAF)
        _c(PvrtcRGB2bppUnorm)
        _c(PvrtcRGB2bppSrgb)
        _c(PvrtcRGBA2bppUnorm)
        _c(PvrtcRGBA2bppSrgb)
        _c(PvrtcRGB4bppUnorm)
        _c(PvrtcRGB4bppSrgb)
        _c(PvrtcRGBA4bppUnorm)
        _c(PvrtcRGBA4bppSrgb)
        #undef _c
        /* LCOV_EXCL_STOP */
    }
    #ifdef __GNUC__
    #pragma GCC diagnostic pop
    #endif

    return debug << "CompressedPixelFormat(" << Debug::nospace << reinterpret_cast<void*>(UnsignedInt(value)) << Debug::nospace << ")";
}
#endif

}
