
// SmileyFace.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSmileyFaceApp:
// �йش����ʵ�֣������ SmileyFace.cpp
//

class CSmileyFaceApp : public CWinApp
{
public:
	CSmileyFaceApp();
	~CSmileyFaceApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSmileyFaceApp theApp;