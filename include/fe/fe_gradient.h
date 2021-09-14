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

#include "fe_export.h"
#include <cstdint>

struct fe_image;

struct fe_vec2
{
    int x;
    int y;
};

#define FE_GRAD_MAX_COLORS 16

struct fe_plane
{
    float a;
    float b;
    float d;
    float scale;
};

struct fe_color
{
    struct rgba_color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    union
    {
        rgba_color rgba;
        //uint32_t argb;
        uint32_t value;
    };
};

struct fe_grad
{
    fe_color colors[FE_GRAD_MAX_COLORS];
    float colors_pos[FE_GRAD_MAX_COLORS];
    int colors_num;

    uint8_t alpha[FE_GRAD_MAX_COLORS];
    float alpha_pos[FE_GRAD_MAX_COLORS];
    int alpha_num;
};

FONT_EFFECT_EXPORT
void fe_gradient_create(fe_image* im, int width, int height,
                        const fe_color* colors, const float* positions, int num,
                        const uint8_t* alpha, const float* alphaPositions, int alphaNum);
