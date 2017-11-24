//
//  icfg_inifile.cpp
//  ibf
//
//  Created by wangzhia on 2017/6/7.
//  Copyright 2017 All rights reserved.
//


#include "icfg_inifile.h"

icfg_inifile::icfg_inifile()
{
	m_FileType = 0;
}

#ifdef _SDFS_CFG_
icfg_inifile::icfg_inifile(SDFS *sp)
{
	m_sp = sp;
	m_FileType = SDFS_FILE_TYPE;
}
#endif

icfg_inifile::~icfg_inifile()
{
	if (m_GroupList.size() == 0)
	{
		return;
	}
	
	//内存释放
	for (map<string,map<string,icfg_Map*> >::iterator iter = m_GroupList.begin(); iter != m_GroupList.end(); iter++)
	{
		map<string,icfg_Map*> vList = iter->second;
		for (map<string,icfg_Map*>::iterator vIter = vList.begin(); vIter != vList.end(); vIter++)
		{
			icfg_Map *pNode = vIter->second;
			delete pNode;
			pNode = NULL;
		}
		vList.clear();
	}
	m_GroupList.clear();
}

void icfg_inifile::icfg_Init(int file_type)
{
	m_FileType = file_type;
	if (m_FileType == SDFS_FILE_TYPE)
	{
#ifndef _SDFS_CFG_
		m_FileType = SYSTEM_FILE_TYPE;
#endif
	}
}

int icfg_inifile::icfg_ReadFile(const char *cfg_name)
{
	int iRet = 0;
	GDF_FILE *fpFILE;
	if (m_FileType == SDFS_FILE_TYPE)
	{
#ifdef _SDFS_CFG_
		fpFILE = new SDFS_GDF_FILE(m_sp,cfg_name);
#endif
	}
	else
	{
		fpFILE = new SYSTEM_GDF_FILE(cfg_name);
	}
	
	char chBuffer[ICFG_BUFFER_MAX_LEN+1];
	char chGroupName[ICFG_NAME_MAX_LEN+1];
	char chKeyName[ICFG_NAME_MAX_LEN+1];
	char chValue[ICFG_VALUE_MAX_LEN+1];
	char *env=NULL;
	
	iRet = fpFILE->Open("r");
	if (iRet != 0)
	{
		printf("FILE[%s]LINE[%d] FILE OPEN IS FAIL!!!\n", __FILE__, __LINE__);
		delete fpFILE;
		fpFILE = NULL;
		return iRet;
	}
	
	memset(chBuffer, 0, sizeof(chBuffer));
	fpFILE->Gets(chBuffer, ICFG_BUFFER_MAX_LEN);
	while(!fpFILE->IsFileEnd())
	{
		switch(*chBuffer)
		{
			/* 注释 */
			case '#' :
				break;
			/* 空行 */
			case ' ' :
			case '\t':
			case '\r':
			case '\n':
				
				if(strcmp(IniFile__Trim(chBuffer), "") != 0)
				{
					printf("FILE[%s]LINE[%d] IniFile__Trim IS FAIL!!!\n", __FILE__, __LINE__);
					delete fpFILE;
					fpFILE = NULL;
					return -3;
				}
				break;
			/* GROUP元素 */
			case '[' :
				
				iRet = IniFile__Analyze1(chBuffer, chGroupName);
				if(iRet)
				{
					printf("FILE[%s]LINE[%d] IniFile__Analyze1 IS FAIL[%d]!!!\n", __FILE__, __LINE__,iRet);
					delete fpFILE;
					fpFILE = NULL;
					return -4;
				}
				break;
			/* KEY元素 */
			default:
				iRet = IniFile__Analyze2(chBuffer, chKeyName, chValue,env);
				if(iRet)
				{
					printf("FILE[%s]LINE[%d] IniFile__Analyze2 IS FAIL[%d]!!!\n", __FILE__, __LINE__,iRet);
					delete fpFILE;
					fpFILE = NULL;
					return -7;
				}
				icfg_SetKey(chGroupName,chKeyName,chValue);
				break;
		}
		
		memset(chBuffer, 0, sizeof(chBuffer));
		fpFILE->Gets(chBuffer, ICFG_BUFFER_MAX_LEN);
	}
	
	if(fpFILE->Close() != 0)
	{
		printf("FILE[%s]LINE[%d] FILE CLOSE IS FAIL!!!\n", __FILE__, __LINE__);
		delete fpFILE;
		fpFILE = NULL;
		return -10;
	}
	delete fpFILE;
	fpFILE = NULL;
	
	return 0;
}

char *icfg_inifile::icfg_Key_GetValue(const char *group_name,const char *key_name)
{
	map<string,map<string,icfg_Map*> >::iterator it = m_GroupList.find(string(group_name));
	if (it!=m_GroupList.end())
	{
		map<string,icfg_Map*>::iterator it1 = it->second.find(string(key_name));
		if (it1 != it->second.end())
		{
			return (it1->second)->chValue;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

char *icfg_inifile::icfg_Key_GetValue(const char *group_name,const char *key_name,const char *result_value)
{
	static char out_value[64+1];
	memset(out_value,0,sizeof(out_value));
	sprintf(out_value,"%s",result_value);
	map<string,map<string,icfg_Map*> >::iterator it = m_GroupList.find(string(group_name));
	if (it!=m_GroupList.end())
	{
		map<string,icfg_Map*>::iterator it1 = it->second.find(string(key_name));
		if (it1 != it->second.end())
		{
			return (it1->second)->chValue;
		}
		else
		{
			return out_value;
		}
	}
	else
	{
		return out_value;
	}
}

bool icfg_inifile::icfg_SetKey(char *group_name,char *key_name,char *value)
{
	icfg_Map *im = new icfg_Map();
	strcpy(im->chName,key_name);
	strcpy(im->chValue,value);
	map<string,map<string,icfg_Map*> >::iterator it = m_GroupList.find(string(group_name));
	if (it != m_GroupList.end())
	{
		map<string,icfg_Map*>::iterator it1 = it->second.find(string(key_name));
		if (it1 == it->second.end())
		{
			(it->second).insert(pair<string,icfg_Map*>(string(key_name),im));
		}
		else
		{
			printf("key[%s]已经存在!!!\n",key_name);
			return false;
		}
	}
	else
	{
		map<string,icfg_Map*> ims;
		ims.insert(pair<string,icfg_Map*>(string(key_name),im));
		m_GroupList.insert(pair<string,map<string,icfg_Map*> >(string(group_name),ims));
	}
	return true;
}

map<string,icfg_Map*> icfg_inifile::icfg_Group_GetKeys(const char *group_name)
{
	map<string,map<string,icfg_Map*> >::iterator it = m_GroupList.find(string(group_name));
	if (it != m_GroupList.end())
	{
		return it->second;
	}
	else
	{
		map<string,icfg_Map*> a;
		return a;
	}
}




