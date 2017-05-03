
// NaMacroRecorderDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "NaMacroRecorder.h"
#include "NaMacroRecorderDlg.h"
#include "afxdialogex.h"

#include <NaLib\NaString.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CNaMacroRecorderDlg 대화 상자



CNaMacroRecorderDlg::CNaMacroRecorderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NAMACRORECORDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bRecording = FALSE;
	m_hActiveWnd = nullptr;
}

void CNaMacroRecorderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILES, m_listFiles);
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
	ON_BN_CLICKED(IDC_CHK_REC_MOUSEMOVE, &CNaMacroRecorderDlg::OnBnClickedChkRecMousemove)
	ON_BN_CLICKED(IDC_CHK_REC_CLICKMOVE, &CNaMacroRecorderDlg::OnBnClickedChkRecClickmove)
	ON_BN_CLICKED(IDC_BTN_RUN, &CNaMacroRecorderDlg::OnBnClickedBtnRun)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILES, &CNaMacroRecorderDlg::OnLvnItemchangedListFiles)
END_MESSAGE_MAP()


// CNaMacroRecorderDlg 메시지 처리기

BOOL CNaMacroRecorderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// Custom
	RegisterHotKey(m_hWnd, 0, 0, VK_F7);
	RegisterHotKey(m_hWnd, 1, 0, VK_F8);
	((CButton*)GetDlgItem(IDC_CHK_REC_CLICKMOVE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHK_REC_DELAY))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHK_REC_COMMENT_WINDOW_INFO))->SetCheck(TRUE);

	m_listFiles.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_listFiles.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERINALLVIEWS);
	RECT rcList;
	m_listFiles.GetClientRect(&rcList);
	m_listFiles.InsertColumn(0, L"File", 0, rcList.right - rcList.left, -1);

	LoadFiles();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CNaMacroRecorderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CNaMacroRecorderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNaMacroRecorderDlg::OnClose()
{
	// Custom
	UnregisterHotKey(m_hWnd, 0);
	UnregisterHotKey(m_hWnd, 1);

	// Clear FileData
	// Ref: m_listFiles.SetItemData(nListIdx, (DWORD)pInfo);
	// TODO

	// Clear action record
	std::vector<ActionRecord*>::iterator it;
	for (it = m_vecActionRecords.begin(); it != m_vecActionRecords.end(); )
	{
		delete *it;
		it = m_vecActionRecords.erase(it);
	}
	m_vecActionRecords.clear();

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
	StartRecord();
}


void CNaMacroRecorderDlg::OnBnClickedBtnStop()
{
	StopRecord();
}

// all mousemove
void CNaMacroRecorderDlg::OnBnClickedChkRecMousemove()
{
	if (((CButton*)GetDlgItem(IDC_CHK_REC_CLICKMOVE))->GetCheck())
		((CButton*)GetDlgItem(IDC_CHK_REC_CLICKMOVE))->SetCheck(0);
}

// clicked mousemove only
void CNaMacroRecorderDlg::OnBnClickedChkRecClickmove()
{
	if (((CButton*)GetDlgItem(IDC_CHK_REC_MOUSEMOVE))->GetCheck())
		((CButton*)GetDlgItem(IDC_CHK_REC_MOUSEMOVE))->SetCheck(0);
}

BOOL CNaMacroRecorderDlg::IsMouseClicked()
{
	if (GetKeyState(VK_LBUTTON) & 0x8000 ||
		GetKeyState(VK_RBUTTON) & 0x8000 ||
		GetKeyState(VK_MBUTTON) & 0x8000)
		return TRUE;

	return FALSE;
}

void CNaMacroRecorderDlg::StartRecord()
{
	if (m_bRecording)
		return;

	TRACE(L"Start Recording.\n");

	StartCaptureInputDevice();

	// for check relative
	GetCursorPos(&m_ptFirstMousePos);
	m_ptCurMousePos = m_ptFirstMousePos;
	m_ptLastMousePos = m_ptCurMousePos;

	if (((CButton*)GetDlgItem(IDC_CHK_REC_MOUSEMOVE))->GetCheck())
		m_enRecordMove = RECORD_MOUSEMOVE_ALL;
	else if (((CButton*)GetDlgItem(IDC_CHK_REC_CLICKMOVE))->GetCheck())
		m_enRecordMove = RECORD_MOUSEMOVE_CLICKED;
	else
		m_enRecordMove = RECORD_MOUSEMOVE_NONE;

	m_bRecordDelay = ((CButton*)GetDlgItem(IDC_CHK_REC_DELAY))->GetCheck();

	m_bRecording = TRUE;
	ToggleUIEnable(m_bRecording);
}

bool CNaMacroRecorderDlg::CopyToClipboard(CString& s)
{
	if (!OpenClipboard())
	{
		return false;
	}

	EmptyClipboard();

	HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, s.GetLength() + 1);
	char *pchData = (char *)GlobalLock(hClipboardData);
	int nLen = WideCharToMultiByte(CP_ACP, 0, s.GetBuffer(0), -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, s.GetBuffer(0), -1, pchData, nLen, NULL, NULL);

	GlobalUnlock(hClipboardData);
	SetClipboardData(CF_TEXT, hClipboardData);
	CloseClipboard();

	// TODO check Clipboard is too big?

	return true;
}

void CNaMacroRecorderDlg::StopRecord()
{
	if (!m_bRecording)
		return;

	TRACE(L"Stop Recording.\n");

	StopCaptureInputDevice();

	m_bRecording = FALSE;
	ToggleUIEnable(m_bRecording);

	// There is a bug that makes the message box strange if the string is too long.
	/*
	CString previewJs = recordedJs;
	previewJs = previewJs.Left(100);
	previewJs += "\n ....";
	MessageBox(recordedJs);
	*/

	// recordedJs.Replace(L"\n", L"\r\n");

	/*
	if (!CopyToClipboard(recordedJs))
	{
		AfxMessageBox(_T("Failed to open clipboard."));
		return;
	}
	else
	{
		AfxMessageBox(L"Script has been copied to clipboard.");
	}
	*/

	// Find next filename.
	CString filename;
	CFileFind filefind;
	for (int i=0; ; i++)
	{
		filename.Format(L"Recorded%04d.js", i);
		if (!filefind.FindFile(filename))
			break;
	}	

	SaveRecordToNaMacroScript(filename);

	CString msg;
	msg.Format(L"Script has been saved to file: %s", filename);
	AfxMessageBox(msg);
}

void CNaMacroRecorderDlg::StartCaptureInputDevice()
{
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
}

void CNaMacroRecorderDlg::StopCaptureInputDevice()
{
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
}

bool CNaMacroRecorderDlg::IsAddWindowInfo()
{
	return ((CButton*)GetDlgItem(IDC_CHK_REC_COMMENT_WINDOW_INFO))->GetCheck() == TRUE;
}

bool CNaMacroRecorderDlg::IsUseRelativeCoord()
{
	// Use Application Coordinates.
	return ((CButton*)GetDlgItem(IDC_CHK_REC_WINDOW_COORD_RELATIVE))->GetCheck() == TRUE;
}

void CNaMacroRecorderDlg::SaveRecordToNaMacroScript(IN CString filename)
{
	//
	// TODO Must save to UTF-8 !!!
	//      BOM + buf
	CFile file;
	file.Open(filename, CFile::modeCreate | CFile::modeWrite, nullptr);

	NaString recordedJs = L"";

#define WRITE_BUFFER \
	{ \
		file.Write(recordedJs.cstr(), strlen(recordedJs.cstr())); \
		recordedJs = L""; \
	}

	DWORD lastTick = DWORD_MAX;
	BOOL useMouse = FALSE, useKey = FALSE;

	for (const auto& ar : m_vecActionRecords)
	{
		if (ar->enType >= ACTION_MOUSEBEGIN && ar->enType <= ACTION_MOUSELAST)
			useMouse = TRUE;
		else if (ar->enType >= ACTION_KEYBEGIN && ar->enType <= ACTION_KEYLAST)
			useKey = TRUE;

		if (useKey && useMouse)
			break;
	}

	CTime time = CTime::GetCurrentTime();
	recordedJs.Format(
		L"// Auto generated script by NaMacroRecorder\n"
		L"// %04d.%02d.%02d\n"
		L"function main() {\n",
		time.GetYear(), time.GetMonth(), time.GetDay()
	);

#define VAR_MOUSE		L"_m"
#define VAR_KEYBOARD	L"_k"
#define VAR_WINDOW		L"_win"
#define STR_TAB			L"   "

	CString newJs;
	if (useMouse)
	{
		newJs.Format(L"%svar %s = system.mouse;\n", STR_TAB, VAR_MOUSE);
		recordedJs += newJs;
	}

	if (useKey)
	{
		newJs.Format(L"%svar %s = system.keyboard;\n", STR_TAB, VAR_KEYBOARD);
		recordedJs += newJs;
	}

	if (IsUseRelativeCoord())
	{
		newJs.Format(L"%svar %s = null;\n", STR_TAB, VAR_WINDOW);
		recordedJs += newJs;

		newJs.Format(
			L"%sfunction _findFirstWindow(text) { "
			L"var w = findWindows(text); "
			L"if (w.length == 0) throw 'Cannot find window:' + text; "
			L"return w[0]; "
			L"}; \n",
			STR_TAB);
		recordedJs += newJs;
	}

	WRITE_BUFFER;
	POINT lastPos = { -1, -1 };
	RECT rcActiveWindow = { -1, -1, -1, -1 };

	for (const auto& ar : m_vecActionRecords)
	{
		// for debug
		/*
		CString str;
		str.Format(L"TimeStamp: %ld, Action: %d, ", ar.dwTimeStamp, ar.enType);
		if (ar.enType >= ACTION_MOUSEBEGIN && ar.enType <= ACTION_MOUSELAST)
		{
		str.Format(L"%sPos: %d, %d", str, ar.ptPos.x, ar.ptPos.y);
		}
		else if (ar.enType >= ACTION_KEYBEGIN && ar.enType <= ACTION_KEYLAST)
		{
		str.Format(L"%sKeyCode: %d (%c)", str, ar.nKeyCode, ar.nKeyCode);
		}
		str.Format(L"%s\n", str);
		TRACE(str);
		*/

		if (m_bRecordDelay && lastTick != DWORD_MAX && lastTick != ar->dwTimeStamp)
		{
			newJs.Format(L"%ssleep(%d);\n", STR_TAB, ar->dwTimeStamp - lastTick);
			recordedJs += newJs;
		}
		lastTick = ar->dwTimeStamp;

		if (ar->enType >= ACTION_MOUSEBEGIN && ar->enType <= ACTION_MOUSELAST)
		{
			MouseActionRecord *mar = (MouseActionRecord*)ar;

			BOOL moved = FALSE;
			if (ar->enType == ACTION_MOUSEMOVE || (m_enRecordMove != RECORD_MOUSEMOVE_ALL))
			{
				if (mar->ptPos.x != lastPos.x || mar->ptPos.y != lastPos.y)
					moved = TRUE;
			}

			if (moved)
			{
				if (!IsUseRelativeCoord())
					newJs.Format(L"%s%s.move(%d, %d);\n", STR_TAB, VAR_MOUSE, mar->ptPos.x, mar->ptPos.y);
				else
				{
					if (mar->ptRelativePos.x == -1 && mar->ptRelativePos.y == -1)
					{
						newJs.Format(L"%s%s.move(%d, %d);\n", STR_TAB, VAR_MOUSE, mar->ptPos.x, mar->ptPos.y);
					}
					else
					{
						newJs.Format(L"%s%s.move(%s.x + %d, %s.y + %d);\n",
							STR_TAB, VAR_MOUSE,
							VAR_WINDOW, mar->ptRelativePos.x,
							VAR_WINDOW, mar->ptRelativePos.y
						);
					}
				}
				recordedJs += newJs;

				lastPos = mar->ptPos;
			}

			switch (ar->enType)
			{
			case ACTION_MOUSEMOVE:
				break;
			case ACTION_MOUSELBUTTONDOWN:
				newJs.Format(L"%s%s.lbuttonDown();\n", STR_TAB, VAR_MOUSE);
				recordedJs += newJs;
				break;
			case ACTION_MOUSELBUTTONUP:
				newJs.Format(L"%s%s.lbuttonUp();\n", STR_TAB, VAR_MOUSE);
				recordedJs += newJs;
				break;
			}
		}
		else if (ar->enType >= ACTION_KEYBEGIN && ar->enType <= ACTION_KEYLAST)
		{
			KeyActionRecord *kar = (KeyActionRecord*)ar;

			newJs.Format(L"%s%s.%s(%d); // '%s'\n",
				STR_TAB, VAR_KEYBOARD,
				(ar->enType == ACTION_KEYDOWN) ? L"down" : L"up", kar->nKeyCode,
				GetKeyName(kar->nKeyCode)
			);
			recordedJs += newJs;
		}
		else if (ar->enType == ACTION_WINDOWINFO)
		{
			WindowInfoActionRecord *war = (WindowInfoActionRecord*)ar;
			rcActiveWindow = war->rcRect;

			if (IsAddWindowInfo())
			{
				recordedJs += L"\n";

				newJs.Format(L"%s// Window: %s (Class: %s)\n",
					STR_TAB, war->strText, war->strClass);
				recordedJs += newJs;

				newJs.Format(L"%s// Position: %d, %d (%d x %d)\n",
					STR_TAB,
					war->rcRect.left, war->rcRect.top,
					war->rcRect.right - war->rcRect.left, war->rcRect.bottom - war->rcRect.top
				);
				recordedJs += newJs;
			}

			if (IsUseRelativeCoord())
			{
				newJs.Format(L"%s%s = _findFirstWindow('%s');\n",
					STR_TAB, VAR_WINDOW, war->strText
				);
				recordedJs += newJs;
			}
		}

		WRITE_BUFFER;
	}

	recordedJs += STR_TAB;
	recordedJs += L"exit();\n";
	recordedJs += L"}\n";
	WRITE_BUFFER;

	// Clear record
	std::vector<ActionRecord*>::iterator it;
	for (it = m_vecActionRecords.begin(); it != m_vecActionRecords.end(); )
	{
		delete *it;
		it = m_vecActionRecords.erase(it);
	}
	m_vecActionRecords.clear();

	file.Close();
}

CString CNaMacroRecorderDlg::GetKeyName(unsigned int nKey)
{
	unsigned int scanCode = MapVirtualKey(nKey, MAPVK_VK_TO_VSC);

	// because MapVirtualKey strips the extended bit for some keys
	switch (nKey)
	{
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: // arrow keys
	case VK_PRIOR: case VK_NEXT: // page up and page down
	case VK_END: case VK_HOME:
	case VK_INSERT: case VK_DELETE:
	case VK_DIVIDE: // numpad slash
	case VK_NUMLOCK:
		scanCode |= 0x100; // set extended bit
		break;
	}

	wchar_t keyName[50];
	if (GetKeyNameText(scanCode << 16, keyName, sizeof(keyName)) != 0)
	{
		return keyName;
	}
	else
	{
		return L"[Error]";
	}
}

void CNaMacroRecorderDlg::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	// 이 기능을 사용하려면 Windows XP 이상이 필요합니다.
	// _WIN32_WINNT 기호는 0x0501보다 크거나 같아야 합니다.

	RAWINPUT RawInput;
	unsigned int unSzRawInput = sizeof(RAWINPUT);
	GetRawInputData(hRawInput, RID_INPUT, &RawInput, &unSzRawInput, sizeof(RAWINPUTHEADER));

	BOOL bIgnore = FALSE;
	ActionRecord *ar = nullptr;

	switch (RawInput.header.dwType)
	{
	case RIM_TYPEKEYBOARD:
		{
			ar = new KeyActionRecord;
			KeyActionRecord *kar = (KeyActionRecord*)ar;

			RAWKEYBOARD &rk = RawInput.data.keyboard;
			if (rk.Flags == RI_KEY_MAKE) // 0
			{
				kar->enType = ACTION_KEYDOWN;
				kar->nKeyCode = rk.VKey;
			}
			else if (rk.Flags & RI_KEY_BREAK)
			{
				kar->enType = ACTION_KEYUP;
				kar->nKeyCode = rk.VKey;
			}
		}
		break;
	case RIM_TYPEMOUSE:
		{
			ar = new MouseActionRecord;
			MouseActionRecord *mar = (MouseActionRecord*)ar;

			RAWMOUSE &rm = RawInput.data.mouse;

			// RAWMOUSE Input does not check screen bound.
			// -> cannot check real position

			GetCursorPos(&m_ptCurMousePos);

			if (rm.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
			{
				ar->enType = ACTION_MOUSELBUTTONDOWN;
			}
			else if (rm.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
			{
				ar->enType = ACTION_MOUSELBUTTONUP;
			}
			else if (rm.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
			{
				ar->enType = ACTION_MOUSERBUTTONDOWN;
			}
			else  if (rm.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
			{
				ar->enType = ACTION_MOUSERBUTTONUP;
			}
			/*
			else if (...)
			{

			}
			*/
			else
			{
				ar->enType = ACTION_MOUSEMOVE;

				if (m_ptLastMousePos.x == m_ptCurMousePos.x &&
					m_ptLastMousePos.y == m_ptCurMousePos.y)
					bIgnore = TRUE;

				if (m_enRecordMove == RECORD_MOUSEMOVE_NONE ||
					(m_enRecordMove == RECORD_MOUSEMOVE_CLICKED && !IsMouseClicked()))
					bIgnore = TRUE;

				m_ptLastMousePos = m_ptCurMousePos;
			}

			if (IsUseRelativeCoord())
			{
				if (m_hActiveWnd)
				{
					RECT rcWnd;
					::GetWindowRect(m_hActiveWnd, &rcWnd);

					mar->ptPos = m_ptCurMousePos;
					mar->ptRelativePos = m_ptCurMousePos;
					mar->ptRelativePos.x -= rcWnd.left;
					mar->ptRelativePos.y -= rcWnd.top;
				}
				else
					mar->ptPos = m_ptCurMousePos;
			}
			else
				mar->ptPos = m_ptCurMousePos;
		}
		break;
	}

	if (bIgnore)
	{
		delete ar;
		ar = nullptr;
	}
	else
	{
		DWORD dwTick = GetTickCount();
		ar->dwTimeStamp = dwTick;

		// Insert window info before lbuttondown
		switch (ar->enType)
		{
		case ACTION_MOUSELBUTTONDOWN:
		case ACTION_MOUSERBUTTONDOWN:
			{
				HWND hDesktop = ::GetDesktopWindow();

				// Find real application window (not control)
				HWND hWnd = ::WindowFromPoint(m_ptCurMousePos);
				for ( ; ; )
				{
					HWND hParent = ::GetParent(hWnd);
					if (hParent == nullptr)
						break;
					if (hParent == hDesktop)
						break;
					if (hParent == hWnd)
						break; // Strange condition
					hWnd = hParent;
				}

				// Using this info for relative coord too.
				WindowInfoActionRecord *war = new WindowInfoActionRecord;
				war->enType = ACTION_WINDOWINFO;
				war->dwTimeStamp = dwTick;

				wchar_t strText[256];
				::GetWindowText(hWnd, strText, 256);
				war->strText = strText;
				war->strText.Replace(L"\r", L"");
				war->strText.Replace(L"\n", L"");

				RECT rcWnd;
				::GetWindowRect(hWnd, &rcWnd);
				war->rcRect.left = rcWnd.left;
				war->rcRect.top = rcWnd.top;
				war->rcRect.right = rcWnd.right;
				war->rcRect.bottom = rcWnd.bottom;

				wchar_t strClass[1024];
				::RealGetWindowClass(hWnd, strClass, 1024);
				war->strClass = strClass;

				// ... more info

				m_hActiveWnd = hWnd;
				m_vecActionRecords.push_back(war);
			}
			break;
		}

		m_vecActionRecords.push_back(ar);
	}

	CDialogEx::OnRawInput(nInputcode, hRawInput);
}

void CNaMacroRecorderDlg::ToggleUIEnable(BOOL bRecording)
{
	GetDlgItem(IDC_BTN_REC)->EnableWindow(!bRecording);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(bRecording);

	POSITION pos = m_listFiles.GetFirstSelectedItemPosition();
	int nSelectedIdx = m_listFiles.GetNextSelectedItem(pos);
	if (nSelectedIdx < 0)
	{
		GetDlgItem(IDC_BTN_RUN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_RUN)->EnableWindow(TRUE);
	}

	GetDlgItem(IDC_CHK_REC_MOUSEMOVE)->EnableWindow(!bRecording);
	GetDlgItem(IDC_CHK_REC_CLICKMOVE)->EnableWindow(!bRecording);
	GetDlgItem(IDC_CHK_REC_DELAY)->EnableWindow(!bRecording);
	GetDlgItem(IDC_CHK_REC_COMMENT_WINDOW_INFO)->EnableWindow(!bRecording);
	GetDlgItem(IDC_CHK_REC_IMAGE_BASE)->EnableWindow(!bRecording);
	GetDlgItem(IDC_CHK_REC_WINDOW_COORD_RELATIVE)->EnableWindow(!bRecording);

}

void CNaMacroRecorderDlg::LoadFiles()
{
	wchar_t buf[1024];
	GetCurrentDirectory(1024, buf);
	CString strConfigFile = buf;
	strConfigFile += L"\\";
	strConfigFile += L"NaMacroRecorder.cfg";

	//m_listFiles.InsertItem(0, L"File.js", 0);
	CStdioFile file;
	CFileException e;
	if (file.Open(strConfigFile, CFile::modeRead | CFile::typeText, &e) == FALSE)
	{
		return;
	}

	CString str;
	while (file.ReadString(str))
	{
		int nIdx = str.Find(L"=");
		CString strId = str.Left(nIdx);
		CString strVal = str.Right(str.GetLength() - nIdx - 1);

		LoadConfig(strId, strVal);
	}

	file.Close();
}

void CNaMacroRecorderDlg::LoadConfig(CString & strId, CString & strVal)
{
	if (strId.CompareNoCase(L"file") == 0)
	{
		AddFile(strVal);
	}
	/*
	else if (...)
	{
	}
	*/
	else
	{
		TRACE(L"Invalid Config Property: %s\n", strId);
	}
}

void CNaMacroRecorderDlg::AddFile(CString & strFullPath)
{
	int nListIdx = m_listFiles.GetItemCount();

	FileInfo *pInfo = new FileInfo;
	pInfo->strFullPath = strFullPath;
	int nIdx = strFullPath.ReverseFind(L'\\');
	if (nIdx != -1)
	{
		pInfo->strPath = strFullPath.Left(nIdx);
		pInfo->strFileName = strFullPath.Right(strFullPath.GetLength() - nIdx - 1);
	}

	m_listFiles.InsertItem(nListIdx, pInfo->strFileName);
	m_listFiles.SetItemData(nListIdx, (DWORD)pInfo);
}

void CNaMacroRecorderDlg::SaveFiles()
{
	// Not Impl
}

void CNaMacroRecorderDlg::OnBnClickedBtnRun()
{
	CString strFile;
	POSITION pos = m_listFiles.GetFirstSelectedItemPosition();
	int nIdx = m_listFiles.GetNextSelectedItem(pos);
	if (nIdx < 0)
	{
		ASSERT(0);
		return;
	}
	strFile = m_listFiles.GetItemText(nIdx, 0);

	// Must placed on same folder
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
	sei.hwnd = ::GetActiveWindow();
	sei.lpVerb = _T("runas");
#if defined(_DEBUG)
	sei.lpFile = L"NaMacro.exe";
#else
	sei.lpFile = L"NaMacroD.exe";
#endif
	sei.lpParameters = strFile;
	sei.nShow = SW_SHOWDEFAULT;

	ShellExecuteEx(&sei);
}

void CNaMacroRecorderDlg::OnLvnItemchangedListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	//TRACE(L"LvnItemChanged: %d\n", pNMLV->iItem);

	int nIdx = pNMLV->iItem;
	GetDlgItem(IDC_BTN_RUN)->EnableWindow(nIdx < 0 ? FALSE : TRUE);

	*pResult = 0;
}
