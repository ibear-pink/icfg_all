#include <string.h>
#include "icfg_inifile.h"
#include "icfg_sinifile.h"

int main(int argc, const char * argv[])
{
	char file_name[1024] = {0};
	strcpy(file_name,"demo_ini.conf");
	icfg_inifile *ii=new icfg_inifile();
	ii->icfg_Init(0);
	if (0!=ii->icfg_ReadFile(file_name))
	{
		delete ii;
		ii =NULL;
		return -1;
	}
	printf("%s\n",ii->icfg_Key_GetValue("LOG","log_name"));
	printf("%s\n",ii->icfg_Key_GetValue("LOG","log_ccc","1"));

	strcpy(file_name,"demo_sini.conf");
	icfg_sinifile *si=new icfg_sinifile();
	si->icfg_Init(0);
	if (0!=si->icfg_ReadFile(file_name))
	{
		delete si;
		si=NULL;
		return -1;
	}
	printf("%s\n",si->icfg_Key_GetValue("log_path"));
	delete ii;
	ii =NULL;
	delete si;
	si=NULL;
	return 0;
}
