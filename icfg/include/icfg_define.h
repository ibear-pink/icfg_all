#ifndef _ICFG_DEFINE_H_
#define _ICFG_DEFINE_H_

#include <string.h>

typedef enum
{
	SYSTEM_FILE_TYPE = 0,
	SDFS_FILE_TYPE = 1
} FileType;

#define ICFG_BUFFER_MAX_LEN				4096
#define ICFG_NAME_MAX_LEN				64
#define ICFG_VALUE_MAX_LEN				256

#endif
