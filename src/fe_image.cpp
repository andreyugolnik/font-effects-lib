/**********************************************\
*
*  Font Effects library by
*  Denis Muratshin / frankinshtein
*
*  Code cleanup by
*  Andrey A. Ugolnik
*
\**********************************************/

#include "fe/fe_image.h"

#include "ImageDataOperations.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>

void* _fe_alloc(size_t size);
void _fe_free(void* ptr);

fe::ImageData* asImage(fe_image* im);
const fe::ImageData* asImage(const fe_image* im);

fe::ImageData* asImage(fe_image* im)
{
    return static_cast<fe::ImageData*>(im);
}

const fe::ImageData* asImage(const fe_image* im)
{
    return static_cast<const fe::ImageData*>(im);
}

void image_free_malloc(fe_image* im)
{
    _fe_free(im->data);
    im->data = 0;
    im->free = 0;
}

void fe_image_create(fe_image* im, int w, int h, FE_IMAGE_FORMAT f)
{
    im->w = w;
    im->h = h;
    im->format = f;
    im->bytespp = getBytesPerPixel(im->format);
    im->pitch = im->bytespp * im->w;
    im->data = (uint8_t*)_fe_alloc(im->pitch * im->h);
    im->free = image_free_malloc;
}

void fe_image_free(fe_image* im)
{
    if (im->free)
        im->free(im);
}

fe_image fe_image_get_rect(const fe_image* im, int x, int y, int w, int h)
{
    fe_image res = asImage(im)->getRect(x, y, w, h);
    res.free = 0; //subrect can't be deleted
    return res;
}

void fe_image_get_rect2(fe_image* p, const fe_image* im, int x, int y, int w, int h)
{
    *p = asImage(im)->getRect(x, y, w, h);
    p->free = 0;
}

void fe_image_copy(const fe_image* src, fe_image* dest)
{
    fe::operations::copy(*asImage(src), *asImage(dest));
}

void fe_image_fill(fe_image* dest, const fe_color* color_)
{
    auto color = static_cast<const Color*>(color_);
    fe::operations::fill(*asImage(dest), *color);
}

void fe_image_copy_alloc(const fe_image* src, fe_image* dest)
{
    *dest = *src;
    dest->pitch = dest->w * dest->bytespp;
    dest->data = (uint8_t*)_fe_alloc(dest->pitch * dest->h);
    dest->free = image_free_malloc;
    fe_image_copy(src, dest);
}

void fe_image_blit(const fe_image* src, fe_image* dest)
{
    fe::operations::blit(*asImage(src), *asImage(dest));
}

void fe_image_unpremultiply(fe_image* src)
{
    fe::operations::unpremultiply(*asImage(src));
}

void fe_image_premultiply(fe_image* gl)
{
    fe::operations::premultiply(*asImage(gl));
}

int getBytesPerPixel(FE_IMAGE_FORMAT tf)
{
    switch (tf)
    {
    case FE_IMG_A8:
        return 1;
    case FE_IMG_B8G8R8A8:
    case FE_IMG_R8G8B8A8:
        return 4;
    case FE_IMG_DISTANCE:
        return sizeof(PixDist);
    default:
        return 0;
    }
    return 0;
}
