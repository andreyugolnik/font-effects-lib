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

#include "ImageData.h"
#include "fe/fe_gradient.h"
#include "fe/fe_image.h"
#include <cassert>

class Color : public fe_color
{
public:
    Color() = default;

    Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
    {
        rgba.r = R;
        rgba.g = G;
        rgba.b = B;
        rgba.a = A;
    }

    Color(const fe_color& c)
    {
        *(fe_color*)(this) = c;
    }
};

#define FE_ASSERT(arg) assert(arg)
