//=============================================================================
// File Name : my_types.h
//=============================================================================
#ifndef __MY_TYPES_H_
#define __MY_TYPES_H_

typedef unsigned int 		U32;
typedef signed int			S32;
typedef unsigned short		U16;
typedef short int			S16;
typedef unsigned char		U8;
typedef signed char 		S8;
typedef unsigned long long 	ULL;

typedef struct
{
	int day;
	int year;
	int mon;
	int date;
	int hour;
	int min;
	int sec;
}RTC_TIME;

#endif