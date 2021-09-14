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

#include "fe/fe_image.h"
#include "pixel.h"

namespace fe
{
#define ALL_FORMATS_SWITCH(format)    \
    switch (format)                   \
    {                                 \
        FORMAT_CASE(A8);              \
        FORMAT_CASE(R8G8B8A8);        \
        FORMAT_CASE(B8G8R8A8);        \
        FORMAT_CASE(DISTANCE);        \
    default:                          \
        FE_ASSERT(!"unknown format"); \
    }

    class ImageData : public fe_image
    {
    public:
        ImageData();
        ImageData(const fe_image& b);
        ImageData(int W, int H, int Pitch, FE_IMAGE_FORMAT Format, void* Data = 0);
        ImageData(const ImageData& b, void* Data);
        ~ImageData();

        //ImageData getRect(const Rect& r) const;
        ImageData getRect(int x, int y, int w, int h) const;
        ImageData getRect(int x, int y) const;
        uint8_t* getPixelPtr(int x, int y) const;
    };

} // namespace fe
