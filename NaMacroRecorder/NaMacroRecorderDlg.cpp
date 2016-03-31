
// NaMacroRecorderDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "NaMacroRecorder.h"
#include "NaMacroRecorderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNaMacroRecorderDlg ��ȭ ����



CNaMacroRecorderDlg::CNaMacroRecorderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NAMACRORECORDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bRecording = FALSE;
}

void CNaMacroRecorderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNaMacroRecorderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_BTN_REC, &CNaMacroRecorderDlg::OnBnClickedBtnRec)
	ON_BN_CLICKED(IDC_BTN_STOP, &CNaMacroRecorderDlg::OnBnClickedBtnStop)
	ON_WM_INPUT()
END_MESSAGE_MAP()


// CNaMacroRecorderDlg �޽��� ó����

BOOL CNaMacroRecorderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// Custom
	RegisterHotKey(m_hWnd, 0, 0, VK_F7);
	RegisterHotKey(m_hWnd, 1, 0, VK_F8);
	((CButton*)GetDlgItem(IDC_CHK_REC_DELAY))->SetCheck(TRUE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CNaMacroRecorderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CNaMacroRecorderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CNaMacroRecorderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNaMacroRecorderDlg::OnClose()
{
	// Custom
	UnregisterHotKey(m_hWnd, 0);
	UnregisterHotKey(m_hWnd, 1);

	CDialogEx::OnClose();
}


void CNaMacroRecorderDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	TRACE(_T("OnHotKey: %d\n"), nHotKeyId);
	switch(nHotKeyId) 
	{
		case 0:
			OnBnClickedBtnRec();
			break;
		case 1:
			OnBnClickedBtnStop();
			break;
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CNaMacroRecorderDlg::OnBnClickedBtnRec()
{
	RecordStart();
}


void CNaMacroRecorderDlg::OnBnClickedBtnStop()
{
	RecordStop();
}

void CNaMacroRecorderDlg::RecordStart()
{
	if (m_bRecording)
		return;

	TRACE(L"Start Recording.\n");

	RAWINPUTDEVICE rawInputDev[2];
	ZeroMemory(rawInputDev, sizeof(RAWINPUTDEVICE) * 2);

	// key
	rawInputDev[0].usUsagePage = 0x01;
	rawInputDev[0].usUsage = 0x06;
	rawInputDev[0].dwFlags = RIDEV_INPUTSINK;
	rawInputDev[0].hwndTarget = m_hWnd;

	// mouse
	rawInputDev[1].usUsagePage = 0x01;
	rawInputDev[1].usUsage = 0x02;
	rawInputDev[1].dwFlags = RIDEV_INPUTSINK;
	rawInputDev[1].hwndTarget = m_hWnd;

	if (RegisterRawInputDevices(rawInputDev, 2, sizeof(RAWINPUTDEVICE)) == FALSE)
	{
		/*
		CString str;
		str.Format(_T("RegisterRawInputDevices Error %d"), GetLastError());
		MessageBox(str);
		*/
	}

	// for check relative
	GetCursorPos(&m_ptFirstMousePos);
	m_ptCurMousePos = m_ptFirstMousePos;
	m_ptLastMousePos = m_ptCurMousePos;

	m_bRecording = TRUE;
	m_bRecordJustMove = ((CButton*)GetDlgItem(IDC_CHK_REC_MOUSEMOVE))->GetCheck();
	m_bRecordDelay = ((CButton*)GetDlgItem(IDC_CHK_REC_DELAY))->GetCheck();

	ToggleUIEnable(m_bRecording);
}

void CNaMacroRecorderDlg::RecordStop()
{
	if (!m_bRecording)
		return;

	TRACE(L"Stop Recording.\n");

	RAWINPUTDEVICE rawInputDev[2];
	ZeroMemory(rawInputDev, sizeof(RAWINPUTDEVICE) * 2);

	// key
	rawInputDev[0].usUsagePage = 0x01;
	rawInputDev[0].usUsage = 0x06;
	rawInputDev[0].dwFlags = RIDEV_REMOVE;
	rawInputDev[0].hwndTarget = NULL;

	// mouse
	rawInputDev[1].usUsagePage = 0x01;
	rawInputDev[1].usUsage = 0x02;
	rawInputDev[1].dwFlags = RIDEV_REMOVE; 
	rawInputDev[1].hwndTarget = NULL;

	if (RegisterRawInputDevices(rawInputDev, 2, sizeof(RAWINPUTDEVICE)) == FALSE)
	{
		/*
		CString str;
		str.Format(_T("RegisterRawInputDevices Error %d"), GetLastError());
		MessageBox(str);
		*/
	}

	m_bRecording = FALSE;
	ToggleUIEnable(m_bRecording);

	// For Test
	POINT ptLastPos;
	ptLastPos.x = -1;
	ptLastPos.y = -1;

	DWORD dwLastTick = -1;

	CString strOutput;
	std::vector<ActionRecord>::iterator it = m_vecActionRecords.begin();
	for (; it != m_vecActionRecords.end(); ++it)
	{
		ActionRecord ar = *it;

		CString str;
		CString strJs;
		str.Format(L"TimeStamp: %ld, Action: %d, ", ar.dwTimeStamp, ar.enType);

		if (m_bRecordDelay &&
			dwLastTick != -1 && dwLastTick != ar.dwTimeStamp)
		{
			strJs.Format(L"sleep(%d);\n", ar.dwTimeStamp - dwLastTick);
			strOutput += strJs;
		}
		dwLastTick = ar.dwTimeStamp;

		if (ar.enType >= ACTION_MOUSEBEGIN && ar.enType <= ACTION_MOUSELAST)
		{
			str.Format(L"%sPos: %d, %d", str, ar.ptPos.x, ar.ptPos.y);

			BOOL bMoved = FALSE;
			if (ar.enType == ACTION_MOUSEMOVE || (!m_bRecordJustMove))
			{
				if (ar.ptPos.x != ptLastPos.x || ar.ptPos.y != ptLastPos.y)
					bMoved = TRUE;
			}

			if (bMoved)
			{
				strJs.Format(L"system.mouse.move(%d,%d);\n", ar.ptPos.x, ar.ptPos.y);
				strOutput += strJs;

				ptLastPos = ar.ptPos;
			}

			switch (ar.enType)
			{
			case ACTION_MOUSEMOVE:
				break;
			case ACTION_MOUSELBUTTONDOWN:
				strJs.Format(L"system.mouse.lbuttonDown();\n");
				strOutput += strJs;
				break;
			case ACTION_MOUSELBUTTONUP:
				strJs.Format(L"system.mouse.lbuttonUp();\n");
				strOutput += strJs;
				break;
			}
		}
		else if (ar.enType >= ACTION_KEYBEGIN && ar.enType <= ACTION_KEYLAST)
		{
			str.Format(L"%sKeyCode: %d (%c)", str, ar.nKeyCode, ar.nKeyCode);
			strJs.Format(L"system.keyboard.%s(%d);\n", 
				(ar.enType == ACTION_KEYDOWN) ? L"down" : L"up",
				ar.nKeyCode);
			strOutput += strJs;
		}
		str.Format(L"%s\n", str);
		TRACE(str);
	}

	m_vecActionRecords.clear();
	MessageBox(strOutput);

	// Copy to clipboard
	{
		strOutput.Replace(L"\n", L"\r\n");

		if (!OpenClipboard())
		{
			AfxMessageBox(_T("Ŭ�����带 �� �� �����ϴ�."));
			return;
		}
		EmptyClipboard();

		HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, strOutput.GetLength() + 1);
		char *pchData = (char *)GlobalLock(hClipboardData);
		int nLen = WideCharToMultiByte(CP_ACP, 0, strOutput.GetBuffer(0), -1, NULL, 0, NULL, NULL);

		WideCharToMultiByte(CP_ACP, 0, strOutput.GetBuffer(0), -1, pchData, nLen, NULL, NULL);
		
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_TEXT, hClipboardData);
		CloseClipboard();
	}
}

void CNaMacroRecorderDlg::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	// �� ����� ����Ϸ��� Windows XP �̻��� �ʿ��մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0501���� ũ�ų� ���ƾ� �մϴ�.

	RAWINPUT RawInput;
	unsigned int unSzRawInput = sizeof(RAWINPUT);
	GetRawInputData(hRawInput, RID_INPUT, &RawInput, &unSzRawInput, sizeof(RAWINPUTHEADER));

	BOOL bIgnore = FALSE;
	ActionRecord ar;

	switch (RawInput.header.dwType)
	{
	case RIM_TYPEKEYBOARD:
		{
			RAWKEYBOARD &rk = RawInput.data.keyboard;
			TRACE(L"OnRawInput.Keyboard) %d\n", rk.Flags);
			
			if (rk.Flags == RI_KEY_MAKE) // 0
			{
				ar.enType = ACTION_KEYDOWN;
				ar.nKeyCode = rk.VKey;
			}
			else if (rk.Flags & RI_KEY_BREAK)
			{
				ar.enType = ACTION_KEYUP;
				ar.nKeyCode = rk.VKey;
			}
		}
		break;
	case RIM_TYPEMOUSE:
		{
			RAWMOUSE &rm = RawInput.data.mouse;

			// RAWMOUSE Input does not check screen bound.
			// -> cannot check real position

			GetCursorPos(&m_ptCurMousePos);

			TRACE(L"OnRawInput.Mouse) flag: %d, btn-flag: %d, rel: (%d,%d), abs: (%d,%d)\n",
				rm.usFlags, rm.usButtonFlags,
				rm.lLastX, rm.lLastY,
				m_ptCurMousePos.x, m_ptCurMousePos.y
				);

			if (rm.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
			{
				ar.enType = ACTION_MOUSELBUTTONDOWN;
			}
			else if (rm.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
			{
				ar.enType = ACTION_MOUSELBUTTONUP;
			}
			else if (rm.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
			{
				ar.enType = ACTION_MOUSERBUTTONDOWN;
			}
			else  if (rm.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
			{
				ar.enType = ACTION_MOUSERBUTTONUP;
			}
			/*
			else if (...)
			{

			}
			*/
			else
			{
				ar.enType = ACTION_MOUSEMOVE;
				
				if (m_ptLastMousePos.x == m_ptCurMousePos.x &&
					m_ptLastMousePos.y == m_ptCurMousePos.y)
					bIgnore = TRUE;

				if (!m_bRecordJustMove)
					bIgnore = TRUE;

				m_ptLastMousePos = m_ptCurMousePos;
			}

			ar.ptPos = m_ptCurMousePos;
		}
		break;
	}

	if (!bIgnore)
	{
		DWORD dwTick = GetTickCount();

		ar.dwTimeStamp = dwTick;
		m_vecActionRecords.push_back(ar);
	}

	CDialogEx::OnRawInput(nInputcode, hRawInput);
}

void CNaMacroRecorderDlg::ToggleUIEnable(BOOL bRecording)
{
	GetDlgItem(IDC_BTN_REC)->EnableWindow(!bRecording);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(bRecording);

	GetDlgItem(IDC_CHK_REC_MOUSEMOVE)->EnableWindow(!bRecording);
	GetDlgItem(IDC_CHK_REC_DELAY)->EnableWindow(!bRecording);
}
