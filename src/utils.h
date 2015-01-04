#ifndef utils_h
#define utils_h
#include <string>

typedef unsigned long long u64;
typedef unsigned long      u32;
typedef unsigned short     u16;

#ifdef _MSC_VER
#define SSCANF(X) sscanf_s X
#else
#define SSCANF(X) sscanf X
#endif

#endif