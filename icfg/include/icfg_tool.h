//
//  icfg_tool.hpp
//  ibf
//
//  Created by wangzhia on 2017/6/6.
//  Copyright  2017 wangzhia. All rights reserved.
//

#ifndef icfg_tool_h
#define icfg_tool_h

#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include "string"
#include "map"
#include "vector"
#include "ifile_system.h"
#ifdef _SDFS_CFG_
#include "ifile_sdfs.h"
#endif
#include "icfg_define.h"

/*文件键值对象结构*/
typedef  struct
{
	char chName[ICFG_NAME_MAX_LEN+1];
	char chValue[ICFG_VALUE_MAX_LEN+1];
} icfg_Map;

extern int strupper(char *);
extern int cf_file_ifdir(const char *fn);
extern char *IniFile__RTrim(char *chBuffer);
extern char* IniFile__LTrim(char *chBuffer);
extern char* IniFile__Trim(char *chBuffer);
extern int IniFile__Analyze1(char *chBuffer, char *chResult);
extern int get_env_value (const char *pSrcPath, char *szDstPath);
extern int IniFile__Analyze2(char *chBuffer, char *chResult1, char *chResult2,char * env);

#endif /* icfg_tool_h */
