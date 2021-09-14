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

/*internal*/
struct fe_state
{
    char* token;
    char* data;
    int size;
    bool error;
};
