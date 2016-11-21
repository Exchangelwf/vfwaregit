#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include <sys/file.h> //O_RDONLY

#define GMAC_STATUS "/proc/h2gmac_status"
//网线断，自动会保存到文件中 ，0断(ASCII 48)，1正常(ASCII 49)

int mii_diag(char *ethname)
{
	char cNetStatus = 0;
	
	FILE *fp=NULL;
	fp = fopen(GMAC_STATUS,"rb");
	if(NULL == fp)
	{
		//printf(" mii_diag::open  %s error \n", GMAC_STATUS);
		return 0;
	}
	
	while(!feof(fp))
	{
		fread(&cNetStatus, 1, 1,fp);
		break;
	}	
	
	fclose(fp);
	//printf(" mii_diag::net ###  %d ################### \n", cNetStatus);
	if (cNetStatus == 48)
	{
	       //printf(" mii_diag::net error  %d error \n", cNetStatus);
		return 1;
	}
	return 0;
}

