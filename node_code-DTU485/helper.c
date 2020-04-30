
#include "helper.h"
#include "stm8l15x.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>





char *helper_strsep (char **stringp, const char *delim)
{
  char *begin, *end;
 
  begin = *stringp;
  if (begin == NULL)
    return NULL;
 
  /* delim分隔符是单个字符的情况是非常频繁的，因此不需要使用代价昂贵的 strpbrk 函数
     而只需要调用 strchr 就能解决  */
  if (delim[0] == '\0' || delim[1] == '\0')
    {
      char ch = delim[0];
 
      if (ch == '\0')
	end = NULL;
      else
	{
	  if (*begin == ch)
	    end = begin;
	  else if (*begin == '\0')
	    end = NULL;
	  else
	    end = strchr (begin + 1, ch);
	}
    }
  else
    /* delim 有两个字符以上，才调用strpbrk  */
    end = strpbrk (begin, delim);
 
  if (end)
    {
      /* 用0封闭这个token；返回stringp，指向一个null指针 */
      *end++ = '\0';
      *stringp = end;
    }
  else
    /* 没有出现delim，这是最后一个token  */
    *stringp = NULL;
 
  return begin;
}











void helper_printTitleHex(uint8_t *title, uint8_t *pin, uint16_t Length)
{
	printf("\r\n%s \r\n", title);
	
		if (Length == 0)
	{
		return;
	}

	for (uint16_t i = 0; i < Length; ++i)
	{
		printf(" %02x", pin[i]);
	}
	printf("\r\n");
}


void helper_printTitleDec(uint8_t *title, uint8_t *pin, uint16_t Length)
{
	printf("\r\n%s \r\n", title);
	
		if (Length == 0)
	{
		return;
	}

	for (uint16_t i = 0; i < Length; ++i)
	{
		printf(" %02d", pin[i]);
	}
	printf("\r\n");
}



/*计算数据的校验和*/
uint8_t GetCrc(uint8_t *buf, uint8_t len) 
{
	uint8_t crcValue =0;
	uint8_t i;
	for (i=0; i<len; i++) {
		crcValue = crcValue + buf[i];
	}
	return crcValue;
}

/*计算数据的校验和并增加至SendBuffer  */
void helper_AddCrc(uint8_t *buf, uint8_t len) 
{
	uint8_t crcValue =0;
	uint8_t i;
	for (i=0; i<len; i++) {
		crcValue = crcValue + buf[i];
	}
	*(buf + len) = crcValue;
	return;
}

/*校验和检查：最后一个是校验    	     		*/
uint8_t CheckCrc(uint8_t *buf, uint8_t len) 
{
	uint8_t crcValue =0;
	uint8_t i;
	for (i=0; i<len-1; i++) {
		crcValue = crcValue + buf[i];
	}
	return (crcValue == buf[len -1]);
}


void helper_printHex(uint8_t * pIn, uint16_t Length)
{
	if (Length == 0)
	{
		return;
	}

	for (uint16_t i = 0; i < Length; ++i)
	{
		printf(" %02x", pIn[i]);
	}
	printf("\n");
}

void helper_printDec(uint8_t * pIn, uint16_t Length)
{
	if (Length == 0)
	{
		return;
	}

	for (uint16_t i = 0; i < Length; ++i)
	{
	    pIn[i] = pIn[i];
		printf(" %d", pIn[i]);
	}
	printf("\n");
}

















