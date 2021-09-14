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

struct fe_color;
struct fe_image;

typedef void (*free_image_fn)(fe_image*);

class PixDist // TODO move
{
public:
    float d1;
    float d2;
    short x;
    short y;
};

enum FE_IMAGE_FORMAT
{
    TF_UNDEFINED = 0,
    FE_IMG_A8 = 1,
    FE_IMG_A16 = 2,
    FE_IMG_R8G8B8A8 = 3,
    FE_IMG_B8G8R8A8 = 4,
    FE_IMG_DISTANCE = 5,
    FE_IMG_MAX = 0xffffffff
};

struct fe_image
{
    int w;
    int h;
    int pitch;
    int bytespp;
    uint8_t* data;
    FE_IMAGE_FORMAT format;

    free_image_fn free;
};

FONT_EFFECT_EXPORT
fe_image fe_image_get_rect(const fe_image* im, int x, int y, int w, int h);

FONT_EFFECT_EXPORT
void fe_image_get_rect2(fe_image*, const fe_image* im, int x, int y, int w, int h);

FONT_EFFECT_EXPORT
void fe_image_create(fe_image*, int w, int h, FE_IMAGE_FORMAT);

FONT_EFFECT_EXPORT
void fe_image_premultiply(fe_image*);

FONT_EFFECT_EXPORT
void fe_image_copy_alloc(const fe_image* src, fe_image* dest);

FONT_EFFECT_EXPORT
void fe_image_free(fe_image* dest);

FONT_EFFECT_EXPORT
void fe_image_copy(const fe_image* src, fe_image* dest);

FONT_EFFECT_EXPORT
void fe_image_fill(fe_image* dest, const fe_color* color);

FONT_EFFECT_EXPORT
void fe_image_blit(const fe_image* src, fe_image* dest);

FONT_EFFECT_EXPORT
void fe_image_unpremultiply(fe_image* src);

FONT_EFFECT_EXPORT
int getBytesPerPixel(FE_IMAGE_FORMAT);
