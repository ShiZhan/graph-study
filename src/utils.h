#ifndef utils_h
#define utils_h
#include <string>

#ifdef _MSC_VER
#define SSCANF(X) sscanf_s X
#else
#define SSCANF(X) sscanf X
#endif

#endif