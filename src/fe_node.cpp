/**********************************************\
*
*  Font Effects library by
*  Denis Muratshin / frankinshtein
*
*  Code cleanup by
*  Andrey A. Ugolnik
*
\**********************************************/

#include "fe/fe_node.h"

#include "ImageDataOperations.h"
#include "fe/fe_effect.h"
#include "fe/fe_gradient.h"
#include "fe/fe_image.h"
#include "fe_node_effects.h"
#include <cassert>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void* _fe_alloc(size_t size);
void _fe_free(void* ptr);

fe::ImageData* asImage(fe_image* im);
const fe::ImageData* asImage(const fe_image* im);

#ifdef SAVE_NODES
FONT_EFFECT_EXPORT
bool fe_image_save_tga(const fe_image* src, const char* fname);
#endif

void fe_im_empty(fe_im& empty)
{
    empty.x = 0;
    empty.y = 0;
    empty.image.w = 0;
    empty.image.h = 0;
    empty.image.data = (uint8_t*)1;
    empty.image.bytespp = 4;
    empty.image.pitch = 4;
    empty.image.format = FE_IMG_R8G8B8A8;
    empty.image.free = 0;
}

fe_im get_image(const fe_node* node, const fe_args* args)
{
    fe_im_cache* cached = args->cache.images;
    if (cached && cached[node->index].image.image.data)
    {
        fe_im res = cached[node->index].image;
        res.image.free = 0;
        return res;
    }

    fe_im r = node->get_image(node, args);

    r.x += static_cast<int>(node->x * args->scale);
    r.y += static_cast<int>(node->y * args->scale);

#ifdef SAVE_NODES
    char str[255];
    sprintf(str, "../temp/%d.tga", node->id);
    fe_image_save_tga(&r.image, str);
#endif

    if (cached)
    {
        cached[node->index].image = r;
        r.image.free = 0;
    }

    return r;
}

fe_im get_image(const fe_node* node, int in, const fe_args* args)
{
    const fe_node* n = node->in[in].node;
    return get_image(n, args);
}

int get_pins(const fe_node* node, const fe_args* args, fe_im* res, int Max)
{
    int num = 0;
    for (int i = FE_MAX_PINS - 1; i >= 0; --i)
    {
        const fe_node* in = node->in[i].node;
        if (in)
        {
            res[num] = get_image(in, args);
            num++;
        }
    }

    return num;
}

void fe_node_init(fe_node* node, int tp, get_node_image f)
{
    node->get_image = f;
    node->x = 0;
    node->y = 0;
    node->flags = 0;
    node->type = tp;
    node->effect = 0;
    node->index = -1;
    node->name[0] = 0;
    for (int i = 0; i < FE_MAX_PINS; ++i)
        node->in[i].node = 0;

    static int id = 1;
    node->id = id++;
}

fe_node_image* fe_node_image_alloc()
{
    fe_node_image* node = (fe_node_image*)_fe_alloc(sizeof(fe_node_image));
    fe_node_init(&node->base, fe_node_type_source_image, (get_node_image)fe_node_image_get_image);
    return node;
}

fe_node_image_fixed* fe_node_image_fixed_alloc()
{
    fe_node_image_fixed* node = (fe_node_image_fixed*)_fe_alloc(sizeof(fe_node_image_fixed));
    fe_node_init(&node->base, fe_node_type_image_fixed, (get_node_image)fe_node_image_fixed_get_image);
    node->im.x = 0;
    node->im.y = 0;

    return node;
}

fe_node_mix* fe_node_mix_alloc()
{
    fe_node_mix* node = (fe_node_mix*)_fe_alloc(sizeof(fe_node_mix));
    fe_node_init(&node->base, fe_node_type_mix, (get_node_image)fe_node_default_get_image);

    return node;
}

fe_node_out* fe_node_out_alloc()
{
    fe_node_out* node = (fe_node_out*)_fe_alloc(sizeof(fe_node_out));
    fe_node_init(&node->base, fe_node_type_out, (get_node_image)fe_node_out_get_image);

    return node;
}

fe_node_outline* fe_node_outline_alloc()
{
    fe_node_outline* node = (fe_node_outline*)_fe_alloc(sizeof(fe_node_outline));
    fe_node_init(&node->base, fe_node_type_outline, (get_node_image)fe_node_outline_get_image);
    node->base.properties_float[fe_const_param_float_outline_rad] = 1.0f;
    node->base.properties_float[fe_const_param_float_outline_sharpness] = 1.0f;

    return node;
}

fe_node_distance_field* fe_node_distance_field_alloc()
{
    fe_node_distance_field* node = (fe_node_distance_field*)_fe_alloc(sizeof(fe_node_distance_field));
    fe_node_init(&node->base, fe_node_type_distance_field, (get_node_image)fe_node_distance_field_get_image);
    node->base.properties_float[fe_const_param_float_distance_field_rad] = 10.0f;
    return node;
}

fe_node* fe_node_distance_field_auto_alloc()
{
    fe_node* node = (fe_node*)_fe_alloc(sizeof(fe_node));
    fe_node_init(node, fe_node_type_distance_field_auto, (get_node_image)fe_node_distance_field_auto_get_image);
    return node;
}

fe_node* fe_node_stroke_simple_alloc()
{
    fe_node* node = (fe_node*)_fe_alloc(sizeof(fe_node));
    fe_node_init(node, fe_node_type_stroke_simple, (get_node_image)fe_node_stroke_simple_get_image);
    return node;
}

fe_node* fe_node_subtract_alloc()
{
    fe_node* node = (fe_node*)_fe_alloc(sizeof(fe_node));
    fe_node_init(node, fe_node_type_subtract, (get_node_image)fe_node_subtract_get_image);
    return node;
}

fe_node* fe_node_light_alloc()
{
    fe_node* node = (fe_node*)_fe_alloc(sizeof(fe_node));
    fe_node_init(node, fe_node_type_light, (get_node_image)fe_node_light_get_image);
    return node;
}

fe_node_fill* fe_node_fill_alloc()
{
    fe_node_fill* node = (fe_node_fill*)_fe_alloc(sizeof(fe_node_fill));
    fe_node_init(&node->base, fe_node_type_fill, (get_node_image)fe_node_fill_get_image);

    node->grad.colors_num = 1;
    node->grad.colors[0].value = 0xffffffff;
    node->grad.colors_pos[0] = 0;

    node->grad.alpha_num = 1;
    node->grad.alpha[0] = 255;
    node->grad.alpha_pos[0] = 0;

    node->plane.a = 0;
    node->plane.b = 1;
    node->plane.d = 0;
    node->plane.scale = 1;

    return node;
}

fe_node_fill_radial* fe_node_fill_radial_alloc()
{
    fe_node_fill_radial* node = (fe_node_fill_radial*)_fe_alloc(sizeof(fe_node_fill_radial));
    fe_node_init(&node->base, fe_node_type_fill_radial, (get_node_image)fe_node_fill_radial_get_image);

    node->grad.colors_num = 1;
    node->grad.colors[0].value = 0xffffffff;
    node->grad.colors_pos[0] = 0;

    node->grad.alpha_num = 1;
    node->grad.alpha[0] = 255;
    node->grad.alpha_pos[0] = 0;

    return node;
}

fe_node* fe_node_alloc(int node_type)
{
    fe_node_type nt = (fe_node_type)node_type;
    switch (nt)
    {
    case fe_node_type_source_image:
        return (fe_node*)fe_node_image_alloc();
    case fe_node_type_source_text: {
        fe_node* node = (fe_node*)_fe_alloc(sizeof(fe_node));
        fe_node_init(node, nt, (get_node_image)fe_node_image_get_image);
        return node;
    }
    case fe_node_type_image_fixed:
        return (fe_node*)fe_node_image_fixed_alloc();
    case fe_node_type_fill:
        return (fe_node*)fe_node_fill_alloc();
    case fe_node_type_fill_radial:
        return (fe_node*)fe_node_fill_radial_alloc();
    case fe_node_type_outline:
        return (fe_node*)fe_node_outline_alloc();
    case fe_node_type_mix:
        return (fe_node*)fe_node_mix_alloc();
    case fe_node_type_distance_field:
        return (fe_node*)fe_node_distance_field_alloc();
    case fe_node_type_distance_field_auto:
        return (fe_node*)fe_node_distance_field_auto_alloc();
    case fe_node_type_out:
        return (fe_node*)fe_node_out_alloc();
    case fe_node_type_stroke_simple:
        return fe_node_stroke_simple_alloc();
    case fe_node_type_subtract:
        return fe_node_subtract_alloc();
    case fe_node_type_light:
        return fe_node_light_alloc();
    default: {
        fe_node* node = (fe_node*)_fe_alloc(sizeof(fe_node));
        fe_node_init(node, nt, (get_node_image)fe_node_default_get_image);
        return node;
    }
    }
    return 0;
}

void _fe_node_free(fe_node* node)
{
    _fe_free(node);
}

void _fe_node_connect(const fe_node* src, fe_node* dest, int pin)
{
    dest->in[pin].node = src;
}

int fe_node_get_in_node_id(const fe_node* node, int i)
{
    const fe_pin* pin = &node->in[i];
    if (pin->node)
        return pin->node->id;
    return 0;
}

void update_df_rad(const fe_node* node, fe_args* args)
{
    if (node->type == fe_node_type_fill_radial)
    {
        const fe_node* in = node->in[0].node;
        if (in)
        {
            float& rad = args->cache.images[in->index].df_rad;
            rad = std::max(rad, node->properties_float[fe_const_param_float_fill_radial_rad_outer]);
        }
    }

    for (int i = 0; i < FE_MAX_PINS; ++i)
    {
        const fe_node* in = node->in[i].node;
        if (in)
            update_df_rad(in, args);
    }
}

bool fe_node_apply_internal(int font_size, const fe_im* gl, const fe_node* node, fe_im* res, int num)
{
    fe_args args;
    args.size = font_size;
    args.base = *gl;
    args.base.y = font_size - args.base.y;
    args.base.image.free = 0; //can't delete not owner
    args.scale = font_size / 100.0f;

    int size = sizeof(fe_im_cache) * num;
    fe_im_cache* imc = (fe_im_cache*)alloca(size);
    args.cache.images = imc;
    args.cache.num = num;

    memset(imc, 0, size);

    for (int i = 0; i < num; ++i)
        imc[i].df_rad = 0.0f;

    update_df_rad(node, &args);

    *res = get_image(node, &args);
    res->y = font_size - res->y;

    res->image.free = imc[node->index].image.image.free;
    imc[node->index].image.image.free = 0;

    for (int i = 0; i < num; ++i)
        fe_image_free(&imc[i].image.image);

    return true;
}

bool fe_node_apply2(int font_size, const fe_im* gl, const fe_node* node, fe_im* res)
{
    return fe_node_apply_internal(font_size, gl, node, res, node->effect->num);
}

bool fe_node_apply(
    int font_size,
    int x, int y,
    int w, int h, FE_IMAGE_FORMAT format, int pitch, const void* data,
    const fe_node* node, fe_im* res)
{
    fe_im gl;
    gl.x = x;
    gl.y = y;
    gl.image.data = (uint8_t*)data;
    gl.image.bytespp = getBytesPerPixel(format);
    gl.image.format = format;
    gl.image.free = 0;
    gl.image.w = w;
    gl.image.h = h;
    gl.image.pitch = pitch;

    return fe_node_apply2(font_size, &gl, node, res);
}

void fe_convert_result(fe_im* src, fe_im* dest, FE_IMAGE_FORMAT dest_format, int convert_options)
{
    if (dest_format == TF_UNDEFINED)
        dest_format = src->image.format;

    dest->x = src->x;
    dest->y = src->y;

    auto srcImage = asImage(&src->image);
    auto destImage = asImage(&dest->image);

    if (convert_options & fe_convert_option_downsample2x)
    {
        int nw = (src->image.w + 1) / 2;
        int nh = (src->image.h + 1) / 2;
        fe_image_create(&dest->image, nw, nh, dest_format);

        fe::PixelR8G8B8A8 src_pf;

        if (dest_format == FE_IMG_B8G8R8A8)
        {
            fe::PixelB8G8R8A8 dest_pf;
            fe::operations::downsample(src_pf, dest_pf, srcImage, destImage);
        }

        if (dest_format == FE_IMG_R8G8B8A8)
        {
            fe::PixelR8G8B8A8 dest_pf;
            fe::operations::downsample(src_pf, dest_pf, srcImage, destImage);
        }

        dest->x /= 2;
        dest->y /= 2;
    }
    else
    {
        fe_image_create(&dest->image, src->image.w, src->image.h, dest_format);
        fe::operations::blit(*srcImage, *destImage);
    }

    if (convert_options & fe_convert_option_unpremultiply)
    {
        fe_image_unpremultiply(&dest->image);
    }
}
