//
//  icfg_sinifile.cpp
//  ibf
//
//  Created by wangzhia on 2017/6/7.
//  Copyright 2017 All rights reserved.
//


#include "icfg_sinifile.h"

icfg_sinifile::icfg_sinifile()
{
	m_FileType = 0;
}

#ifdef _SDFS_CFG_
icfg_sinifile::icfg_sinifile(SDFS *sp)
{
	m_sp = sp;
	m_FileType = 0;
}
#endif

icfg_sinifile::~icfg_sinifile()
{
	if (m_KeyValueList.size() == 0)
	{
		return;
	}
	
	map<string,icfg_Map*>::iterator iter;
	
	//内存释放
	for (iter = m_KeyValueList.begin(); iter != m_KeyValueList.end(); iter++)
	{
		icfg_Map *pNode = iter->second;
		delete pNode;
		pNode = NULL;
	}
	
	m_KeyValueList.clear();
}

void icfg_sinifile::icfg_Init(int file_type)
{
	m_FileType = file_type;
	if (m_FileType == SDFS_FILE_TYPE)
	{
#ifndef _SDFS_CFG_
		m_FileType = SYSTEM_FILE_TYPE;
#endif
	}
}

int icfg_sinifile::icfg_ReadFile(const char *cfg_name)
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
				icfg_SetKey(chKeyName,chValue);
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

char *icfg_sinifile::icfg_Key_GetValue(char *key_name)
{
	map<string,icfg_Map*>::iterator it = m_KeyValueList.find(string(key_name));
	if (it!=m_KeyValueList.end())
	{
		icfg_Map *map = it->second;
		return map->chValue;
	}
	else
	{
		return NULL;
	}
}

char *icfg_sinifile::icfg_Key_GetValue(char *key_name,char *result_value)
{
	map<string,icfg_Map*>::iterator it = m_KeyValueList.find(string(key_name));
	if (it!=m_KeyValueList.end())
	{
		icfg_Map *map = it->second;
		return map->chValue;
	}
	else
	{
		return result_value;
	}
}

bool icfg_sinifile::icfg_SetKey(char *key_name,char *value)
{
	icfg_Map *im = new icfg_Map();
	strcpy(im->chName,key_name);
	strcpy(im->chValue,value);
	map<string,icfg_Map*>::iterator it = m_KeyValueList.find(string(key_name));
	if (it != m_KeyValueList.end())
	{
		printf("key[%s]已经存在!!!\n",key_name);
		return false;
	}
	else
	{
		m_KeyValueList.insert(pair<string,icfg_Map*>(string(key_name),im));
	}
	return true;
}




