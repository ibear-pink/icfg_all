//
//  icfg_sinifile.h
//  ibf
//
//  Created by wangzhia on 2017/6/7.
//  Copyright 2017 All rights reserved.
//


/**
 *description:
 *			ר�����ڴ���INI�����ļ�-H�ļ�
 *�ļ���ʽ:
 *			key1=value1				//"key1" ����(����Ϊ��), "=" �ָ���(����Ϊ��), "value1" ��ֵ(����Ϊ��)
 *			key2=value2				//���壬���ܵ������ڣ��������鶨�����ڡ�
 *			key3=value3
 *			------------------------------
 *			[�ڲ����ݽṹ��ϵ]
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
	map<string,icfg_Map*> m_KeyValueList;//���ȫ����Key,Value
	int m_FileType;//�ļ�ϵͳ���� 0:���� 1:sdfs��Ĭ��0��
};

#endif /* icfg_sinifile_h */
