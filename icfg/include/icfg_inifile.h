//
//  icfg_inifile.h
//  ibf
//
//  Created by wangzhia on 2017/6/7.
//  Copyright 2017 All rights reserved.
//


/**
 *description:
 *			专门用于处理INI数据文件-H文件
 *文件格式:
 *			[group1]				//组定义, 组名不能为空
 *			key1=value1				//"key1" 域名(不能为空), "=" 分隔符(不能为空), "value1" 域值(可以为空)
 *			key2=value2				//域定义，不能单独存在，必须在组定义区内。
 *			[group2]
 *			key1=value1
 *			[group3]
 *			------------------------------
 *			[内部数据结构关系]
 *			m_GroupList|---m_KeyValueList
 *				       |---m_KeyValueList|---icfg_Map
 *										 |---icfg_Map
 */

#ifndef icfg_inifile_h
#define icfg_inifile_h

#include <stdio.h>
#include "icfg_tool.h"

using namespace std;

class icfg_inifile
{
public:
	icfg_inifile();
#ifdef _SDFS_CFG_
	icfg_inifile(SDFS *sp);
#endif
	virtual ~icfg_inifile();
	virtual int icfg_ReadFile(const char *cfg_name);
	virtual void icfg_Init(int file_type);
	
	virtual char *icfg_Key_GetValue(const char *group_name,const char *key_name);
	//当返回值为空时，返回result_value
	virtual char *icfg_Key_GetValue(const char *group_name,const char *key_name,const char *result_value);
	virtual bool icfg_SetKey(char *group_name,char *key_name,char *value);
	virtual map<string,icfg_Map*> icfg_Group_GetKeys(const char *group_name);
	
private:
	int m_FileType;//文件系统类型 0:本地 1:sdfs（默认0）
	map<string,map<string,icfg_Map*> > m_GroupList;//以组为单位存放
#ifdef _SDFS_CFG_
	SDFS *m_sp;
#endif
};

#endif /* icfg_inifile_h */
