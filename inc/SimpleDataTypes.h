#ifndef _SIMPLE_DATA_TYPES_H_
#define _SIMPLE_DATA_TYPES_H_

#include <vector>
#include <list>
#include <string>

#include <stdio.h>
#include <sys/types.h>

/**
*	Definition of data types
*/
typedef signed char 		int8_t;
typedef unsigned char 		uint8_t;
#ifndef __uint32_t_defined
typedef unsigned int		uint32_t;
# define __uint32_t_defined
#endif
typedef unsigned long long int 	uint64_t;
/*
typedef signed long int 		int32_t;
typedef signed int 			int16_t;
typedef unsigned int 			uint16_t;

typedef signed long long int 	int64_t;

*/

typedef std::vector<uint8_t>		ByteBuffer_t;
typedef std::vector<std::string>	StringArray_t;
typedef std::list<std::string>  	StringList_t;

#endif //_SIMPLE_DATA_TYPES_H_
