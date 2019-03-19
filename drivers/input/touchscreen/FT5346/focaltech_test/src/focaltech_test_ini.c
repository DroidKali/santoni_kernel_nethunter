/************************************************************************
* Copyright (C) 2012-2015, Focaltech Systems (R)\A3\ACAll Rights Reserved.
* Copyright (C) 2018 XiaoMi, Inc.
*
* File Name: focaltech_test_ini.c
*
* Author: Software Development Team, AE
*
* Created: 2016-08-01
*
* Abstract: parsing function of INI file
*
************************************************************************/
#include <linux/string.h>
#include <linux/kernel.h>
#include <asm/unistd.h>
#include <linux/slab.h>

#include "../include/focaltech_test_ini.h"
#include "../include/focaltech_test_main.h"


char CFG_SSL = '[';  /* Section Symbol --Can be defined according to the special need to change. For example {}  */
char CFG_SSR = ']';  /*  Section Symbol --Can be defined according to the special need to change. For example {} */
char CFG_NIS = ':';  /* Separator between name and index */
char CFG_NTS = '#';  /* annotator */
char CFG_EQS = '=';  /* The equal sign */


ST_INI_FILE_DATA *g_st_ini_file_data = NULL;
int g_used_key_num = 0;


static int ini_file_get_line(char *filedata, char *buffer, int maxlen);
static long fts_atol(char *nptr);


/* Works only for digits and letters, but small and fast */
#define TOLOWER(x) ((x) | 0x20)

int fts_strncmp(const char *cs, const char *ct, size_t count)
{
	unsigned char c1 = 0, c2 = 0;

	while (count) {
		c1 = TOLOWER(*cs++);
		c2 = TOLOWER(*ct++);
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
		count--;
	}
	return 0;
}

/*************************************************************
Function:  Get the value of key
Input: char * filedata; char * section; char * key
Output: char * value\A1\A1key
Return: 0       SUCCESS
		-1      can not find section
		-2      can not find key
		-10     File open failed
		-12     File read  failed
		-14     File format error
		-22     Out of buffer size
Note:
*************************************************************/
int ini_get_key(char *filedata, char *section, char *key, char *value)
{
	int i = 0;
	int ret = -2;
	for (i = 0; i < g_used_key_num; i++) {
		if (fts_strncmp(section, g_st_ini_file_data[i].pSectionName,
					    g_st_ini_file_data[i].iSectionNameLen) != 0)
			continue;

		if (strlen(key) == g_st_ini_file_data[i].iKeyNameLen) {
			if (fts_strncmp(key, g_st_ini_file_data[i].pKeyName,  g_st_ini_file_data[i].iKeyNameLen) == 0) {
				memcpy(value, g_st_ini_file_data[i].pKeyValue, g_st_ini_file_data[i].iKeyValueLen);
				ret = 0;
				break;
			}
		}
	}

	return ret;
}

/*************************************************************
Function: Remove empty character on the right side of the string
Input:  char * buf --String pointer
Output:
Return: String pointer
Note:
*************************************************************/
char *ini_str_trim_r(char *buf)
{
	int len, i;
	char tmp[512];

	memset(tmp, 0, sizeof(tmp));
	len = strlen(buf);


	memset(tmp, 0x00, len);
	for (i = 0; i < len; i++) {
		if (buf[i] != ' ')
			break;
	}
	if (i < len) {
		strncpy(tmp, (buf+i), (len-i));
	}
	strncpy(buf, tmp, len);

	return buf;
}

/*************************************************************
Function: Remove empty character on the left side of the string
Input: char * buf --String pointer
Output:
Return: String pointer
Note:
*************************************************************/
char *ini_str_trim_l(char *buf)
{
	int len, i;
	char tmp[512];

	memset(tmp, 0, sizeof(tmp));
	len = strlen(buf);


	memset(tmp, 0x00, len);

	for (i = 0; i < len; i++) {
		if (buf[len-i-1] != ' ')
			break;
	}
	if (i < len) {
		strncpy(tmp, buf, len-i);
	}
	strncpy(buf, tmp, len);

	return buf;
}

/*************************************************************
Function: Read a line from file
Input:  FILE *fp; int maxlen-- Maximum length of buffer
Output: char *buffer --  A string
Return: >0      Actual read length
		-1      End of file
		-2      Error reading file
Note:
*************************************************************/
static int ini_file_get_line(char *filedata, char *buffer, int maxlen)
{
	int i = 0;
	int j = 0;
	int iRetNum =  -1;
	char ch1 = '\0';

	for (i = 0, j = 0; i < maxlen; j++) {
		ch1 = filedata[j];
		iRetNum = j+1;
		if (ch1 == '\n' || ch1 == '\r') {
			ch1 = filedata[j+1];
			if (ch1 == '\n' || ch1 == '\r') {
				iRetNum++;
			}

			break;
		} else if (ch1 == 0x00) {
			iRetNum = -1;
			break;
		} else {
			buffer[i++] = ch1;
		}
	}
	buffer[i] = '\0';

	return iRetNum;
}

int my_fts_atoi(const char *str)
{
	int result = 0;
	int signal = 1;
	if ((*str >= '0'&&*str <= '9') || *str == '-' || *str == '+') {
		if (*str == '-' || *str == '+') {
			if (*str == '-')
				signal = -1;
			str++;
		}
	} else
		return 0;

	while (*str >= '0' && *str <= '9')
		result = result*10 + (*str++ - '0');

	return signal*result;
}

int isspace(int x)
{
	if (x == ' ' || x == '\t' || x == '\n' || x == '\f' || x == '\b' || x == '\r')
		return 1;
	else
		return 0;
}

int isdigit(int x)
{
	if (x <= '9' && x >= '0')
		return 1;
	else
		return 0;

}

static long fts_atol(char *nptr)
{
	int c;
	long total;
	int sign;

	while (isspace((int)(unsigned char)*nptr))
		++nptr;
	c = (int)(unsigned char)*nptr++;
	sign = c;
	if (c == '-' || c == '+')
		c = (int)(unsigned char)*nptr++;
	total = 0;
	while (isdigit(c)) {
		total = 10 * total + (c - '0');
		c = (int)(unsigned char)*nptr++;
	}
	if (sign == '-')
		return -total;
	else
		return total;
}

int fts_atoi(char *nptr)
{
	return (int)fts_atol(nptr);
}

int init_key_data(void)
{
	int i = 0;

	FTS_TEST_FUNC_ENTER();

	g_used_key_num = 0;

	g_st_ini_file_data = NULL;
	if (NULL == g_st_ini_file_data)
		g_st_ini_file_data = fts_malloc(sizeof(ST_INI_FILE_DATA)*MAX_KEY_NUM);
	if (NULL == g_st_ini_file_data) {
		FTS_TEST_ERROR("fts_malloc failed in function.");
		return -EPERM;
	}
	for (i = 0; i < MAX_KEY_NUM; i++) {
		memset(g_st_ini_file_data[i].pSectionName, 0, MAX_KEY_NAME_LEN);
		memset(g_st_ini_file_data[i].pKeyName, 0, MAX_KEY_NAME_LEN);
		memset(g_st_ini_file_data[i].pKeyValue, 0, MAX_KEY_VALUE_LEN);
		g_st_ini_file_data[i].iSectionNameLen = 0;
		g_st_ini_file_data[i].iKeyNameLen = 0;
		g_st_ini_file_data[i].iKeyValueLen = 0;
	}

	FTS_TEST_FUNC_EXIT();
	return 1;
}

int release_key_data(void)
{
	if (NULL != g_st_ini_file_data)
		fts_free(g_st_ini_file_data);

	return 0;
}
int print_key_data(void)
{
	int i = 0;



	FTS_TEST_DBG("g_used_key_num = %d",  g_used_key_num);
	for (i = 0; i < MAX_KEY_NUM; i++) {

		FTS_TEST_DBG("pSectionName_%d:%s, pKeyName_%d:%s\n,pKeyValue_%d:%s",
				i, g_st_ini_file_data[i].pSectionName,
				i, g_st_ini_file_data[i].pKeyName,
				i, g_st_ini_file_data[i].pKeyValue
				);

	}

	return 1;
}
/*************************************************************
Function: Read all the parameters and values to the structure.
Return: Returns the number of key. If you go wrong, return a negative number.
		-10         File open failed
		-12         File read  failed
		-14         File format error
Note:
*************************************************************/
int ini_get_key_data(char *filedata)
{

	char buf1[MAX_CFG_BUF + 1] = {0};
	int n = 0;
	int ret = 0;
	int dataoff = 0;
	int iEqualSign = 0;
	int i = 0;
	char tmpSectionName[MAX_CFG_BUF + 1] = {0};



	FTS_TEST_FUNC_ENTER();

	ret = init_key_data();
	if (ret < 0) {
		return -EPERM;
	}

	g_used_key_num = 0;
	while (1) {
		ret = CFG_ERR_READ_FILE;
		n = ini_file_get_line(filedata+dataoff, buf1, MAX_CFG_BUF);
		if (n < -1)
			goto cfg_scts_end;
		if (n < 0)
			break;
		if (n >= MAX_CFG_BUF) {
			FTS_TEST_ERROR("Error Length:%d\n",  n);
			goto cfg_scts_end;
		}

		dataoff += n;

		n = strlen(ini_str_trim_l(ini_str_trim_r(buf1)));
		if (n == 0 || buf1[0] == CFG_NTS)
			continue;
		ret = CFG_ERR_FILE_FORMAT;

		if (n > 2 && ((buf1[0] == CFG_SSL && buf1[n-1] != CFG_SSR))) {
			FTS_TEST_ERROR("Bad Section:%s\n",  buf1);
			goto cfg_scts_end;
		}


		if (buf1[0] == CFG_SSL) {
			g_st_ini_file_data[g_used_key_num].iSectionNameLen = n-2;
			if (MAX_KEY_NAME_LEN < g_st_ini_file_data[g_used_key_num].iSectionNameLen) {
				ret = CFG_ERR_OUT_OF_LEN;
				FTS_TEST_ERROR("MAX_KEY_NAME_LEN: CFG_ERR_OUT_OF_LEN\n");
				goto cfg_scts_end;
			}

			buf1[n-1] = 0x00;
			strcpy((char *)tmpSectionName, buf1+1);


			continue;
		}


		strcpy(g_st_ini_file_data[g_used_key_num].pSectionName, tmpSectionName);
		g_st_ini_file_data[g_used_key_num].iSectionNameLen = strlen(tmpSectionName);

		iEqualSign = 0;
		for (i = 0; i < n; i++) {
			if (buf1[i] == CFG_EQS) {
				iEqualSign = i;
				break;
			}
		}
		if (0 == iEqualSign)
			continue;

		g_st_ini_file_data[g_used_key_num].iKeyNameLen = iEqualSign;
		if (MAX_KEY_NAME_LEN < g_st_ini_file_data[g_used_key_num].iKeyNameLen) {
			ret = CFG_ERR_OUT_OF_LEN;
			FTS_TEST_ERROR("MAX_KEY_NAME_LEN: CFG_ERR_OUT_OF_LEN\n");
			goto cfg_scts_end;
		}
		memcpy(g_st_ini_file_data[g_used_key_num].pKeyName,
			   buf1, g_st_ini_file_data[g_used_key_num].iKeyNameLen);


		g_st_ini_file_data[g_used_key_num].iKeyValueLen = n-iEqualSign-1;
		if (MAX_KEY_VALUE_LEN < g_st_ini_file_data[g_used_key_num].iKeyValueLen) {
			ret = CFG_ERR_OUT_OF_LEN;
			FTS_TEST_ERROR("MAX_KEY_VALUE_LEN: CFG_ERR_OUT_OF_LEN\n");
			goto cfg_scts_end;
		}
		memcpy(g_st_ini_file_data[g_used_key_num].pKeyValue,
			   buf1 + iEqualSign+1, g_st_ini_file_data[g_used_key_num].iKeyValueLen);


		ret = g_used_key_num;

		g_used_key_num++;
		if (MAX_KEY_NUM < g_used_key_num) {
			ret = CFG_ERR_TOO_MANY_KEY_NUM;
			FTS_TEST_ERROR("MAX_KEY_NUM: CFG_ERR_TOO_MANY_KEY_NUM\n");
			goto cfg_scts_end;
		}
	}



	FTS_TEST_FUNC_EXIT();

	return 0;

cfg_scts_end:

	FTS_TEST_FUNC_EXIT();
	return ret;
}

