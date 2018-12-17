#pragma once

#ifdef WIN32
#define __attribute(x)
#define __attribute__(x)
#endif


#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>


typedef const char *const   pString;
typedef unsigned int        uint;
typedef unsigned short int  uint16;
typedef signed short int    int16;
typedef unsigned char       uint8;
typedef unsigned char       uchar;
typedef signed char         int8;

typedef void(*pFuncVV)(void);

#define ERROR_VALUE_UINT8 255

#define TIME_UPDATE_KEYBOARD 2   ///< ����� ����� �������� ����������
