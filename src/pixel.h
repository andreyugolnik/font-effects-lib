/**********************************************\
*
*  Font Effects library by
*  Denis Muratshin / frankinshtein
*
*  Code cleanup by
*  Andrey A. Ugolnik
*
\**********************************************/

#pragma once

#include "fe-include.h"

namespace fe
{
    const uint8_t lookupTable4to8[] = { 0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255 };
    const uint8_t lookupTable5to8[] = { 0, 8, 16, 24, 32, 41, 49, 57, 65, 74, 82, 90, 98, 106, 115, 123, 131, 139, 148, 156, 164, 172, 180, 189, 197, 205, 213, 222, 230, 238, 246, 255 };
    const uint8_t lookupTable6to8[] = { 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125, 129, 133, 137, 141, 145, 149, 153, 157, 161, 165, 170, 174, 178, 182, 186, 190, 194, 198, 202, 206, 210, 214, 218, 222, 226, 230, 234, 238, 242, 246, 250, 255 };

    struct Pixel
    {
        union
        {
            struct
            {
                uint8_t bytes[4];
            };

            struct
            {
                uint8_t r, g, b, a;
            };

            uint32_t rgba;
        };
    };

    inline Pixel initPixel(uint32_t rgba)
    {
        Pixel p;
        p.rgba = rgba;
        return p;
    }

    inline Pixel initPixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        Pixel p;
        p.r = r;
        p.g = g;
        p.b = b;
        p.a = a;
        return p;
    }

    class PixelBase
    {
    public:
        virtual void getPixel(const uint8_t* data, Pixel& p, int x, int y) const = 0;
    };

    class PixelA8 : public PixelBase
    {
    public:
        void getPixel(const uint8_t* data, Pixel& p, int x, int y) const override
        {
            p.a = data[0];
            p.r = p.a;
            p.g = p.a;
            p.b = p.a;
            //p.a = data[0];
        }

        void setPixel(uint8_t* data, const Pixel& p) const
        {
            *data = p.a;
        }

        void copy(const uint8_t* src, uint8_t* dst) const
        {
            *((uint8_t*)dst) = *((uint8_t*)src);
        }

        uint8_t snap_a(uint8_t alpha) const
        {
            return alpha;
        }
    };

    class PixelDISTANCE : public PixelBase
    {
    public:
        void getPixel(const uint8_t* data, Pixel& p, int x, int y) const override
        {
            auto d1 = *(float*)data;
            auto d2 = *((float*)data + 1);
            auto t = int((d1 - d2) * 10);
            auto f = t;
            if (f < 0)
                f = -f;
            if (f > 255)
                f = 255;
            auto color = 255 - f;

            if (d1 > 0)
            {
                p.r = color;
                p.g = 0;
                p.b = 0;
            }
            if (d1 < 0)
            {
                p.r = 0;
                p.g = color;
                p.b = 0;
            }

            if (d1 == 0)
            {
                color = int(255 - d2 * 255);
                p.r = color;
                p.g = color;
                p.b = color;
            }
            p.a = 255;
        }

        void setPixel(uint8_t* data, const Pixel& p) const
        {
            *data = p.a;
        }

        void copy(const uint8_t* src, uint8_t* dst) const
        {
            *((uint8_t*)dst) = *((uint8_t*)src);
        }

        uint8_t snap_a(uint8_t alpha) const
        {
            return alpha;
        }
    };

    class PixelL8 : public PixelBase
    {
    public:
        void getPixel(const uint8_t* data, Pixel& p, int x, int y) const override
        {
            uint8_t color = *data;
            p.r = color;
            p.g = color;
            p.b = color;
            p.a = 255;
        }

        void setPixel(uint8_t* data, const Pixel& p) const
        {
            *data = (p.r + p.g + p.b) / 3;
        }

        void copy(const uint8_t* src, uint8_t* dst) const
        {
            *dst = *src;
        }

        uint8_t snap_a(uint8_t alpha) const
        {
            return 255;
        }
    };

    class PixelR5G6B5 : public PixelBase
    {
        /*
        in memory: BBBBB_GGGGGG_RRRRR
        in dword:  RRRRR_GGGGGG_BBBBB
        */

    public:
        void getPixel(const uint8_t* data, Pixel& p, int x, int y) const override
        {
            auto color = *((uint16_t*)data);
            p.r = lookupTable5to8[(color & 0xF800) >> 11];
            p.g = lookupTable6to8[(color & 0x7E0) >> 5];
            p.b = lookupTable5to8[(color & 0x1F)];
            p.a = 255;
        }

        void setPixel(uint8_t* data, const Pixel& p) const
        {
            auto pshort = (uint16_t*)data;
            *pshort = ((p.r >> 3) << 11) | ((p.g >> 2) << 5) | (p.b >> 3);
        }

        void copy(const uint8_t* src, uint8_t* dst) const
        {
            *((uint16_t*)dst) = *((uint16_t*)src);
        }

        uint8_t snap_a(uint8_t alpha) const
        {
            return 255;
        }
    };

    class PixelR8G8B8A8 : public PixelBase
    {
        /*
        in memory: R8 G8 B8 A8
        in dword: A8 B8 G8 R8
        */

    public:
        void getPixel(const uint8_t* data, Pixel& p, int x, int y) const override
        {
            p.r = data[0];
            p.g = data[1];
            p.b = data[2];
            p.a = data[3];
        }

        void setPixel(uint8_t* data, const Pixel& p) const
        {
            data[0] = p.r;
            data[1] = p.g;
            data[2] = p.b;
            data[3] = p.a;
        }

        void copy(const uint8_t* src, uint8_t* dst) const
        {
            *((uint32_t*)dst) = *((uint32_t*)src);
        }

        uint8_t snap_a(uint8_t alpha) const
        {
            return alpha;
        }
    };

    class PixelB8G8R8A8 : public PixelBase
    {
        /*
        in memory: B8 G8 R8 A8
        in dword: A8 R8 G8 B8
        */

    public:
        void getPixel(const uint8_t* data, Pixel& p, int x, int y) const override
        {
            p.r = data[2];
            p.g = data[1];
            p.b = data[0];
            p.a = data[3];
        }

        void setPixel(uint8_t* data, const Pixel& p) const
        {
            data[2] = p.r;
            data[1] = p.g;
            data[0] = p.b;
            data[3] = p.a;
        }

        void copy(const uint8_t* src, uint8_t* dst) const
        {
            *((uint32_t*)dst) = *((uint32_t*)src);
        }

        uint8_t snap_a(uint8_t alpha) const
        {
            return alpha;
        }
    };

} // namespace fe
