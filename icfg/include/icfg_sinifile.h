//
//  icfg_sinifile.h
//  ibf
//
//  Created by wangzhia on 2017/6/7.
//  Copyright 2017 All rights reserved.
//


/**
 *description:
 *			专门用于处理INI数据文件-H文件
 *文件格式:
 *			key1=value1				//"key1" 域名(不能为空), "=" 分隔符(不能为空), "value1" 域值(可以为空)
 *			key2=value2				//域定义，不能单独存在，必须在组定义区内。
 *			key3=value3
 *			------------------------------
 *			[内部数据结构关系]
 *			|---m_KeyValueList|---icfg_Map
 *							  |---icfg_Map
 */

#ifndef icfg_sinifile_h
#define icfg_sinifile_h

#include <stdio.h>
#include "icfg_tool.h"

using namespace std;

class icfg_sinifile
{
public:
	icfg_sinifile();
	~icfg_sinifile();
	int icfg_ReadFile(const char *cfg_name);
	void icfg_Init(int file_type);
	
	char *icfg_Key_GetValue(char *key_name);
	char *icfg_Key_GetValue(char *key_name,char *result_value);
	bool icfg_SetKey(char *key_name,char *value);
	
private:
	map<string,icfg_Map*> m_KeyValueList;//存放全部的Key,Value
	int m_FileType;//文件系统类型 0:本地 1:sdfs（默认0）
};

#endif /* icfg_sinifile_h */
