
// PersonDetectionDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "vvImage.h"
#include <string.h>
//#include "HOGProcessor.h"
#include "HOGClassifier.h"
#include "PersonDetector.h"
using namespace std;

// CPersonDetectionDlg dialog
class CPersonDetectionDlg : public CDialogEx
{
// Construction
public:
	CPersonDetectionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PersonDetection_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	HOGClassifier hogClassifier;
	PersonDetector detector;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strPath;
	CString m_strTrainPath;
	CString m_strTestPath;
	CString m_strSVMTrainPath;
	char m_strSVMTrain[400];
	CString m_strResPath;
	CStatic m_cPicView;
	CStatic m_cPicRes;
	IplImage* m_pImage;
	IplImage* m_resImage;
	int m_bLoad;
	int m_resLoad;
	int m_bCalc;
	afx_msg void OnBnClickedBrowse();
	CString m_type;
	CString m_save;
	int m_bins;
	int m_wcell;
	int m_hcell;
	int m_wblock;
	int m_hblock;
	UINT m_wwindow;
	UINT m_hwindow;

	// Cac ham tien ich
	bool GetFolder(std::string& folderpath, const char* szCaption = NULL, HWND hOwner = NULL);
	int readFilesInFolder(string dir);
	char* ConverterStringToChar(const CString &s);
	string convertCStringToString(CString CStr);
	void StringExplode(string str, string separator, vector<string>* results);

	// Cac ham xu ly su kien tu giao dien
	afx_msg void OnBnClickedDoopen();
	afx_msg void OnBnClickedDocalc();
	float m_overlap;
	afx_msg void OnDestroy();
	afx_msg void OnStnClickedPicreview();
	afx_msg void OnBnClickedBrowseTrain();
	afx_msg void OnBnClickedStartTraining();
	afx_msg void OnBnClickedStartDetecting();
	afx_msg void OnBnClickedBrowseTest();
	afx_msg void OnBnClickedBrowseRes();
	afx_msg void OnBnClickedBrowseSvmTrain();
	afx_msg void OnBnClickedDodetect();
	afx_msg void OnStnClickedPicResult();
	afx_msg void OnBnClickedDosaveresult();
};
