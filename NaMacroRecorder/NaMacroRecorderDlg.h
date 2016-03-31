
// NaMacroRecorderDlg.h : ��� ����
//

#pragma once

#include <vector>

enum ActionTypes {
	ACTION_MOUSEBEGIN,
	ACTION_MOUSEMOVE,
	ACTION_MOUSELBUTTONDOWN,
	ACTION_MOUSELBUTTONUP,
	ACTION_MOUSERBUTTONDOWN,
	ACTION_MOUSERBUTTONUP,
	ACTION_MOUSELAST,

	ACTION_KEYBEGIN,
	ACTION_KEYDOWN,
	ACTION_KEYUP,
	ACTION_KEYLAST,
};

union ActionRecord {
	// mouse action
	struct {
		ActionTypes enType;
		POINT ptPos;
		DWORD dwTimeStamp;
	};
	// key action
	struct {
		ActionTypes enType;
		int nKeyCode;
		DWORD dwTimeStamp;
	};
};

// CNaMacroRecorderDlg ��ȭ ����
class CNaMacroRecorderDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CNaMacroRecorderDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAMACRORECORDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnBnClickedBtnRec();
	afx_msg void OnBnClickedBtnStop();

	void RecordStart();
	void RecordStop();
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);

	// recording state
	BOOL m_bRecording;

	// recording options
	BOOL m_bRecordJustMove;
	BOOL m_bRecordDelay;

	// recording data
	POINT m_ptFirstMousePos;
	POINT m_ptCurMousePos;
	POINT m_ptLastMousePos;
	std::vector<ActionRecord> m_vecActionRecords;
	
	void ToggleUIEnable(BOOL bRecording);
};
