
#pragma once

#include <wtypes.h>

#define SAFEDELETE(p) if(p) { delete (p); (p) = NULL; } 
#define SAFE_DELETE SAFEDELETE

#define SAFEDELETE_ARRAY(p) if(p) { delete [] (p); (p) = NULL; } 
#define SAFE_DELETE_ARRAY SAFEDELETE_ARRAY

#ifndef _ATL
#define TRACE printf
#endif
