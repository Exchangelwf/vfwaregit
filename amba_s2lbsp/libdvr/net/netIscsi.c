#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    "Cmos.h"
#include    "Net.h"

#define ISCSI_CONFIG1 "/mnt/mtd/Config/iscsi/accountinfo"
#define ISCSI_CONFIG2 "/mnt/mtd/Config/iscsi/serverinfo"
#define ISCSI_ENABLE  "/mnt/mtd/Config/iscsi/iscsi-enable"
#define ISCSI_AUTH	  "/mnt/mtd/Config/iscsi/iscsi-auth"

int NetGetIscsiConfig(NET_ISCSI_CONFIG *m_pConfig)
{
	FILE *fp;

	if(m_pConfig == NULL){

		ERR_PRINT("NULL m_pConfig\n");
		return -1;
	}

	memset((void *)m_pConfig,0,sizeof(NET_ISCSI_CONFIG));
	
	fp = fopen(ISCSI_CONFIG1,"rb");

	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",ISCSI_CONFIG1);
		return -1;
	}

	fscanf(fp,"%s %s\n",m_pConfig->username,m_pConfig->password);
	fclose(fp);

	fp = fopen(ISCSI_CONFIG2,"rb");

	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",ISCSI_CONFIG2);
		return -1;
	}

	fscanf(fp,"%s %s\n",m_pConfig->ip,m_pConfig->port);
	fclose(fp);

	
	fp = fopen(ISCSI_ENABLE,"rb");
	
	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",ISCSI_ENABLE);
		return -1;
	}

	fscanf(fp,"%d\n",&(m_pConfig->enable));
	fclose(fp);
	
	fp = fopen(ISCSI_AUTH,"rb");

	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",ISCSI_AUTH);
		return -1;
	}

	fscanf(fp,"%d\n",&(m_pConfig->auth)); 
	fclose(fp);
	
	return 0;
}

int NetSetIscsiConfig(NET_ISCSI_CONFIG *m_pConfig)
{
	FILE *fp;

	if(m_pConfig == NULL){

		ERR_PRINT("NULL m_pConfig\n");
		return -1;
	}
	
	fp = fopen(ISCSI_CONFIG1,"w+");

	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",ISCSI_CONFIG1);
		return -1;
	}

	fprintf(fp,"%s %s\n",m_pConfig->username,m_pConfig->password);
	fclose(fp);

	fp = fopen(ISCSI_CONFIG2,"w+");

	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",ISCSI_CONFIG2);
		return -1;
	}

	fprintf(fp,"%s %s\n",m_pConfig->ip,m_pConfig->port); 
	fclose(fp);

	fp = fopen(ISCSI_ENABLE,"w+");

	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",ISCSI_ENABLE);
		return -1;
	}

	fprintf(fp,"%d\n",m_pConfig->enable); 
	fclose(fp);

	fp = fopen(ISCSI_AUTH,"w+");

	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",ISCSI_AUTH);
		return -1;
	}

	fprintf(fp,"%d\n",m_pConfig->auth); 
	fclose(fp);
	
	return 0;
}

