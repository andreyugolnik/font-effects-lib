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

struct fe_effect;

struct fe_bundle
{
    fe_effect* effect;
    int num;
};

FONT_EFFECT_EXPORT
fe_bundle* fe_bundle_load(const void* data, int size);

FONT_EFFECT_EXPORT
void fe_bundle_free(fe_bundle*);

FONT_EFFECT_EXPORT
fe_effect* fe_bundle_get_effect(fe_bundle*, int i);

FONT_EFFECT_EXPORT
fe_effect* fe_bundle_get_effect_by_name(fe_bundle*, const char* name);
