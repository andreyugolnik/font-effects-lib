/**********************************************\
*
*  Font Effects library by
*  Denis Muratshin / frankinshtein
*
*  Code cleanup by
*  Andrey A. Ugolnik
*
\**********************************************/

#include "ImageDataOperations.h"
//#include "math/Color.h"
#include <cstring>

namespace fe
{
    namespace operations
    {
        bool check(const ImageData& src, const ImageData& dest)
        {
            FE_ASSERT(dest.w == src.w);
            FE_ASSERT(dest.h == src.h);
            FE_ASSERT(src.data);
            FE_ASSERT(dest.data);
            //OX_ASSERT(src.pitch);
            //OX_ASSERT(dest.pitch);
            FE_ASSERT(src.bytespp);
            FE_ASSERT(dest.bytespp);
            if (dest.w != src.w || dest.h != src.h || !src.data || !dest.data || !src.pitch || !dest.pitch || !src.bytespp || !dest.bytespp)
                return false;

            return true;
        }

        void copy(const ImageData& src, const ImageData& dest)
        {
            if (!check(src, dest))
                return;

            FE_ASSERT(src.format == dest.format);

            int bppPitch = src.w * src.bytespp;

            if (src.pitch == dest.pitch && bppPitch == dest.pitch)
                memcpy(dest.data, src.data, bppPitch * src.h);
            else
            {
                const uint8_t* srcLine = src.data;
                uint8_t* destLine = dest.data;

                const int srch = src.h;
                const int srcpitch = src.pitch;
                const int destpitch = dest.pitch;
                for (int h = 0; h < srch; h++)
                {
                    memcpy(destLine, srcLine, bppPitch);
                    srcLine += srcpitch;
                    destLine += destpitch;
                }
            }
        }

        void move(const ImageData& src, const ImageData& dest)
        {
            if (!check(src, dest))
                return;

            FE_ASSERT(src.format == dest.format);

            int bppPitch = src.w * src.bytespp;

            if (src.pitch == dest.pitch && bppPitch == dest.pitch)
                memmove(dest.data, src.data, bppPitch * src.h);
            else
            {
                const uint8_t* srcLine = src.data;
                uint8_t* destLine = dest.data;

                const int srch = src.h;
                const int srcpitch = src.pitch;
                const int destpitch = dest.pitch;
                for (int h = 0; h < srch; h++)
                {
                    memmove(destLine, srcLine, bppPitch);
                    srcLine += srcpitch;
                    destLine += destpitch;
                }
            }
        }

        void blit(const ImageData& src, const ImageData& dest)
        {
            if (src.format == dest.format)
            {
                copy(src, dest);
                return;
            }

            op_blit op;
            applyOperation(op, src, dest);
        }

        void blitPremultiply(const ImageData& src, const ImageData& dest)
        {
            op_premultipliedAlpha op;
            applyOperation(op, src, dest);
        }

        void premultiply(const ImageData& dest)
        {
            blitPremultiply(dest, dest);
        }

        void unpremultiply(const ImageData& dest)
        {
            op_unpremultipliedAlpha op;
            applyOperation(op, dest, dest);
        }

        void blitColored(const ImageData& src, const ImageData& dest, const Color& c)
        {
            Pixel p;
            p.r = c.rgba.r;
            p.g = c.rgba.g;
            p.b = c.rgba.b;
            p.a = c.rgba.a;

            op_blit_colored op(p);
            applyOperation(op, src, dest);
        }

        void flipY(const ImageData& src, const ImageData& dest)
        {
            if (!check(src, dest))
                return;

            const uint8_t* srcLine = src.data;
            uint8_t* destLine = dest.data + dest.pitch * dest.h - dest.pitch;

            int bppPitch = src.w * src.bytespp;

            const int srch = src.h;
            const int srcpitch = src.pitch;
            const int destpitch = dest.pitch;
            for (int h = 0; h < srch; h++)
            {
                memcpy(destLine, srcLine, bppPitch);
                srcLine += srcpitch;
                destLine -= destpitch;
            }
        }

        void blend(const ImageData& src, const ImageData& dest)
        {
            op_blend_srcAlpha_invSrcAlpha op;
            applyOperation(op, src, dest);
        }

        void fill(ImageData& dest, const Color& color)
        {
            op_fill fill;
            fill.color.r = color.rgba.r;
            fill.color.g = color.rgba.g;
            fill.color.b = color.rgba.b;
            fill.color.a = color.rgba.a;

            applyOperation(fill, dest);
        }

    } // namespace operations

} // namespace fe
