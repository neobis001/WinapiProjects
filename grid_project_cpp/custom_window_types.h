//This header file uses the UNICODE header name to go in sync with the Windows.h headers
//with using either Unicode or ASCII versions of functions

#pragma once
#include <Windows.h>

#ifdef UNICODE
typedef WCHAR DYNCHAR;
#else
typedef CHAR DYNCHAR;
#endif

#ifdef UNICODE
typedef LPWSTR DYNSTR;
#else
typedef LPSTR DYNSTR;
#endif

#ifdef UNICODE
typedef LPCWSTR DYNCSTR;
#else
typedef LPCSTR DYNCSTR;
#endif

typedef int* pint;