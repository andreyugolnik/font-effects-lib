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

struct fe_node;

struct fe_effect
{
    char id[32];

    char text[32];
    char path_font[256]; // TODO opt
    char path_back[256]; // TODO opt
    float distance;

    fe_node* out_node;

    short px, py;

    fe_node** nodes;
    int num;
    int size;
};

FONT_EFFECT_EXPORT
fe_node* fe_effect_find_node(const fe_effect* ef, int id);

FONT_EFFECT_EXPORT
fe_node* fe_effect_find_node_by_type(const fe_effect* ef, int FE_NODE_TYPE);

FONT_EFFECT_EXPORT
fe_node* fe_effect_get_node(const fe_effect* ef, int i);

FONT_EFFECT_EXPORT
const char* fe_effect_get_name(const fe_effect* ef);

FONT_EFFECT_EXPORT
const char* fe_effect_get_text(const fe_effect* ef);

FONT_EFFECT_EXPORT
const char* fe_effect_get_path_font(const fe_effect* ef);

FONT_EFFECT_EXPORT
const char* fe_effect_get_path_back(const fe_effect* ef);
