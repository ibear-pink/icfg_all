//
//  icfg_inifile.h
//  ibf
//
//  Created by wangzhia on 2017/6/7.
//  Copyright 2017 All rights reserved.
//


/**
 *description:
 *			ר�����ڴ���INI�����ļ�-H�ļ�
 *�ļ���ʽ:
 *			[group1]				//�鶨��, ��������Ϊ��
 *			key1=value1				//"key1" ����(����Ϊ��), "=" �ָ���(����Ϊ��), "value1" ��ֵ(����Ϊ��)
 *			key2=value2				//���壬���ܵ������ڣ��������鶨�����ڡ�
 *			[group2]
 *			key1=value1
 *			[group3]
 *			------------------------------
 *			[�ڲ����ݽṹ��ϵ]
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
	//������ֵΪ��ʱ������result_value
	virtual char *icfg_Key_GetValue(const char *group_name,const char *key_name,const char *result_value);
	virtual bool icfg_SetKey(char *group_name,char *key_name,char *value);
	virtual map<string,icfg_Map*> icfg_Group_GetKeys(const char *group_name);
	
private:
	int m_FileType;//�ļ�ϵͳ���� 0:���� 1:sdfs��Ĭ��0��
	map<string,map<string,icfg_Map*> > m_GroupList;//����Ϊ��λ���
#ifdef _SDFS_CFG_
	SDFS *m_sp;
#endif
};

#endif /* icfg_inifile_h */
