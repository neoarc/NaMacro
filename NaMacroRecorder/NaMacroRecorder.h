
// NaMacroRecorder.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CNaMacroRecorderApp:
// �� Ŭ������ ������ ���ؼ��� NaMacroRecorder.cpp�� �����Ͻʽÿ�.
//

class CNaMacroRecorderApp : public CWinApp
{
public:
	CNaMacroRecorderApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CNaMacroRecorderApp theApp;