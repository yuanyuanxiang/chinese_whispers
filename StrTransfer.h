/************************************************************************/
/* �ַ�����ʽת��                                                       */
/************************************************************************/

#pragma once

#include "StrTransfer.h"
#include "StdAfx.h"

//	���ֽ�ת���ɿ��ֽ�
WCHAR* MByteToWChar(const char *pszStr);

//	���ֽ�ת����UTF8
char* WCharToUtf8(const WCHAR *pcwszStr);

//	���ֽ�ת����UTF8
char* MByteToUtf8(const char *pszStr);
