//
//  icfg_tool.cpp
//  ibf
//
//  Created by wangzhia on 2017/6/6.
//  Copyright  2017 wangzhia. All rights reserved.
//

#include "icfg_tool.h"

/******************************************************************
 Function:		int strupper(char *src_str)
 Description:	���ַ����е��ַ�ת��д
 Input:			char * src_str  ��Ҫת�����ַ���
 Return: 		int  0
 Others:
 ********************************************************************/
int strupper(char *src_str)
{
	while(*src_str != '\0')
	{
		*src_str = toupper(*src_str);
		src_str++;
	}
	
	return 0;
}

/*
 function:   cf_file_ifdir()
 description:�����Ƿ�ΪĿ¼
 Input:		fn:Ŀ¼��
 Output:
 Return:		0:�Ϸ�,!0:�Ƿ�
 others:
 */
int cf_file_ifdir(const char *fn)
{
	struct stat filestat;
	
	if(stat(fn, &filestat) != 0)
	{
		return -1;
	}
	
	if(filestat.st_mode & S_IFDIR)
	{
		return 0;
	}
	
	return -1;
}


/*
 function:		IniFile__RTrim()
 description:	�ڲ�������ɾ���ַ��������Ч�ַ�
 Input:      	chBuffer:�ַ���
 Output:
 Return:		��������ַ���
 others:
 */
char *IniFile__RTrim(char *chBuffer)
{
	char *pchBuffer = NULL;
	
	if(*chBuffer == '\0')
	{
		return chBuffer;
	}
	
	pchBuffer = chBuffer;
	while(*pchBuffer == ' ' || *pchBuffer == '\t' || *pchBuffer == '\r' || *pchBuffer == '\n')
	{
		pchBuffer++;
	}
	chBuffer = pchBuffer;
	return chBuffer;
}

/*
 function:		IniFile__LTrim()
 description:	�ڲ�������ɾ���ַ����ұ���Ч�ַ�
 Input:		chBuffer:�ַ���
 Output:
 Return:		��������ַ���
 others:
 */
char* IniFile__LTrim(char *chBuffer)
{
	int iLen;
	
	if(*chBuffer == '\0')
	{
		return chBuffer;
	}
	
	iLen = strlen(chBuffer);
	while(chBuffer[iLen-1] == ' ' || chBuffer[iLen-1] == '\t' || chBuffer[iLen-1] == '\r' || chBuffer[iLen-1] == '\n')
	{
		iLen--;
	}
	chBuffer[iLen] = '\0';
	return chBuffer;
}

/*
 function:		IniFile__Trim()
 description:	�ڲ�������ɾ���ַ���������Ч�ַ�
 Input:		chBuffer:�ַ���
 Output:
 Return:		��������ַ���
 others:
 */
char* IniFile__Trim(char *chBuffer)
{
	IniFile__LTrim(chBuffer);
	return IniFile__RTrim(chBuffer);
}

/*
 function:		IniFile__Analyze1()
 description:	�ڲ�����������GROUP����
 Input:
 chBuffer:ԭʼ��Ϣ
 chResult:����������Ϣ
 Output:
 Return:		0:�ɹ�,!0ʧ��
 others:
 */
int IniFile__Analyze1(char *chBuffer, char *chResult)
{
	char chTemp[ICFG_NAME_MAX_LEN+1];
	char *pchBuffer;
	int  iLen;
	
	if(*chBuffer != '[')
	{
		return -1;
	}
	
	if(strlen(chBuffer) < 3)
	{
		return -2;
	}
	
	strcpy(chTemp, chBuffer+1);
	IniFile__Trim(chTemp);
	if(strlen(chTemp) == 0)
	{
		return -3;
	}
	
	pchBuffer = chTemp + strlen(chTemp) - 1;
	if(*pchBuffer != ']')
	{
		return -4;
	}
	*pchBuffer = '\0';
	IniFile__Trim(chTemp);
	
	iLen = strlen(chTemp);
	if(iLen < 1 || iLen > ICFG_NAME_MAX_LEN)
	{
		return -5;
	}
	
	if(strchr(chTemp, '[') != NULL)
	{
		return -6;
	}
	
	if(strchr(chTemp, ']') != NULL)
	{
		return -7;
	}
	
	strcpy(chResult, chTemp);
	
	return 0;
}

int get_env_value (const char *pSrcPath, char *szDstPath)
{
	char *pEnvHead = NULL, *pEnvTail = NULL;
	char szDealTmp[1024],szEnvName[1024],szEnvValue[1024];
	char *p_env_abm = NULL;
	
	char szSrcPath[1024];
	
	if (pSrcPath == NULL)
	{
		printf("szSrcPath is NULL\n");
		return 1;
	}
	
	memset(szSrcPath, 0, sizeof(szSrcPath));
	strcpy(szSrcPath, pSrcPath);
	
	/*���� ${ABM_HOME}/cfg/abm_cfg.xml */
	while (1)
	{
		if((pEnvHead = strstr(szSrcPath,"${")) != NULL)
		{
			if ((pEnvTail = strchr(szSrcPath, '}')) != NULL)
			{
				*pEnvHead='\0';
				memset(szEnvName,0,sizeof(szEnvName));
				strncpy(szEnvName,pEnvHead+2,pEnvTail-(pEnvHead+2));
				IniFile__Trim(szEnvName);
				p_env_abm = getenv(szEnvName); /*ȡϵͳ��������*/
				if( NULL == p_env_abm)
				{
					printf("[%s] [%s] [%d]getenv([%s]) error\n",__FILE__, __FUNCTION__,__LINE__,szEnvName);
					return 1;
				}
				else
				{
					sprintf(szDstPath,"%s%s%s",pEnvHead,p_env_abm,pEnvTail+1);
					memset(szSrcPath,0,sizeof(szSrcPath));
					sprintf(szSrcPath,szDstPath);
				}
			}
			else
			{
				printf("[%s] [%s] [%d]getenv([%s]) error\n",__FILE__, __FUNCTION__,__LINE__,szEnvName);
				return 1;
			}
		}
		else
		{
			/*���� $ABM_HOME/cfg/abm_cfg.xml */
			if ((pEnvHead = strstr(szSrcPath,"$")) != NULL)
			{
				*pEnvHead='\0';
				if((pEnvTail = strstr(szSrcPath,"/")) != NULL)
				{
					memset(szEnvName,0,sizeof(szEnvName));
					strncpy(szEnvName,pEnvHead+1,pEnvTail-(pEnvHead+1));
					IniFile__Trim(szEnvName);
					p_env_abm = getenv(szEnvName); /*ȡϵͳ��������*/
					if( NULL == p_env_abm)
					{
						printf("[%s] [%s] [%d]getenv([%s]) error\n",__FILE__, __FUNCTION__,__LINE__,szEnvName);
						return 1;
					}
					else
					{
						sprintf(szDstPath,"%s%s%s",pEnvHead,p_env_abm,pEnvTail);
						memset(szSrcPath,0,sizeof(szSrcPath));
						sprintf(szSrcPath,szDstPath);
					}
				}
				else if((pEnvTail=strstr(szSrcPath,"$"))!=NULL)
				{
					memset(szEnvName,0,sizeof(szEnvName));
					strncpy(szEnvName,pEnvHead+1,pEnvTail-(pEnvHead+1));
					IniFile__Trim(szEnvName);
					p_env_abm = getenv(szEnvName); /*ȡϵͳ��������*/
					if( NULL == p_env_abm)
					{
						printf("[%s] [%s] [%d]getenv([%s]) error\n",__FILE__, __FUNCTION__,__LINE__,szEnvName);
						return 1;
					}
					else
					{
						sprintf(szDstPath,"%s%s%s",pEnvHead,p_env_abm,pEnvTail);
						memset(szSrcPath,0,sizeof(szSrcPath));
						sprintf(szSrcPath,szDstPath);
					}
				}
				else
				{
					memset(szEnvName,0,sizeof(szEnvName));
					strcpy(szEnvName,pEnvHead+1);
					IniFile__Trim(szEnvName);
					p_env_abm = getenv(szEnvName); /*ȡϵͳ��������*/
					if( NULL == p_env_abm)
					{
						printf("[%s] [%s] [%d]getenv([%s]) error\n",__FILE__, __FUNCTION__,__LINE__,szEnvName);
						return 1;
					}
					
					sprintf(szDstPath,"%s",p_env_abm);
					
					break;
				}
			}
			else
			{
				sprintf(szDstPath,"%s",szSrcPath);
				break;
			}
		}
	}
	
	return 0;
}

/*
 function:		IniFile__Analyze2()
 description:	�ڲ�����������KEY����
 Input:		chBuffer:ԭʼ��Ϣ
 chResult1:�����������Ϣ
 chResult2:�������ֵ��Ϣ
 Output:
 Return:		0:�ɹ�,!0ʧ��
 others:
 */
int IniFile__Analyze2(char *chBuffer, char *chResult1, char *chResult2,char * env)
{
	int iLen;
	char *pchBuffer;
	char chTemp[ICFG_BUFFER_MAX_LEN];
	
	strcpy(chTemp, chBuffer);
	pchBuffer = strchr(chTemp, '=');
	if(pchBuffer == NULL)
	{
		return -1;
	}
	
	*pchBuffer = '\0';
	IniFile__Trim(chTemp);
	
	iLen = strlen(chTemp);
	if(iLen < 1 || iLen > ICFG_NAME_MAX_LEN)
	{
		return 2;
	}
	strcpy(chResult1, chTemp);
	
	pchBuffer = strchr(chBuffer, '=');
	pchBuffer ++;
	strcpy(chTemp, pchBuffer);
	IniFile__Trim(chTemp);
	
	iLen = strlen(chTemp);
	if(iLen > ICFG_VALUE_MAX_LEN)
	{
		return -3;
	}
	
	if(strchr(chTemp,'/') != NULL)
	{
		if (get_env_value (chTemp, chResult2))
		{
			return -4;
		}
	}
	else
	{
		strcpy(chResult2, chTemp);
	}
	
	return 0;
}
