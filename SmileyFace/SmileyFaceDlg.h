
// SmileyFaceDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
//////////////////////////////////////////�����������沿ʶ�𲿷�
//#include <opencv2/highgui/highgui_c.h>
//#include "cv.h"
//#include "highgui.h"
#include "CvvImage.h"
#include "detect_recog.h"
//#include <conio.h>

////////////////////////////////////////////////////////////////////
// CSmileyFaceDlg �Ի���
class CSmileyFaceDlg : public CDialogEx
{
// ����
public:
	CSmileyFaceDlg(CWnd* pParent = NULL);	// ��׼���캯��
//	static bool takeSample;
// �Ի�������
	enum { IDD = IDD_SMILEYFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
//	virtual ~CSmileyFaceDlg();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	static CvCapture * pCapture;
	afx_msg void OnBnClickedOpenPicButton();
	CStatic m_Picture;
	afx_msg void OnBnClickedStartButton();
	CStatic m_Face_Image;
	CString m_Result_Text;
	afx_msg void OnBnClickedExitButton();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCamButton();
	CButton m_CheckBox;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedRecognizeButton();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//UINT ThreadFun(); //���߳�
	static UINT WINAPI ThreadFun(LPVOID lpParam);
	afx_msg void OnBnClickedSampleButton();
	afx_msg void OnBnClickedClearButton();
	CEdit pEdtConfidence;
};

