
// PersonDetectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PersonDetectionApp.h"
#include "PersonDetectionDlg.h"
#include "afxdialogex.h"
#include <time.h>
#include "vvImage.h"
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPersonDetectionDlg dialog



///"C:/Users/DuyQuang/Desktop/My Projects/Computer Vision/PersonDetectionMultiScale/PersonDetection/PersonDetection/Dataset/Train/TrainSVM.xml"
CPersonDetectionDlg::CPersonDetectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPersonDetectionDlg::IDD, pParent)
	, m_strPath(_T(""))
	, m_strResPath(_T(""))
	, m_strTrainPath(_T(""))
	, m_strTestPath(_T(""))
	, m_strSVMTrainPath(_T("Dataset/Train/TrainSVM.xml"))	
	, m_pImage(NULL)
	, m_resImage(NULL)
	, m_bLoad(0)
	, m_resLoad(0)
	, m_bCalc(0)
	, m_type(_T("R-HOG"))
	, m_save(_T(""))
	, m_bins(9)
	, m_wcell(8)
	, m_hcell(8)
	, m_wblock(2)
	, m_hblock(2)
	, m_wwindow(64)
	, m_hwindow(128)
	, m_overlap(0.5)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_strSVMTrain, 0, sizeof(m_strSVMTrain));
}

void CPersonDetectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PATH, m_strPath);
	DDX_Text(pDX, IDC_PATH_RES, m_strResPath);
	DDX_Text(pDX, IDC_PATH_TRAIN, m_strTrainPath);
	DDX_Text(pDX, IDC_PATH_TEST, m_strTestPath);
	DDX_Text(pDX, IDC_PATH_SVM_TRAIN, m_strSVMTrainPath);
	DDX_Control(pDX, IDC_PICREVIEW, m_cPicView);
	DDX_Control(pDX, IDC_PIC_RESULT, m_cPicRes);
	DDX_Text(pDX, IDC_TYPE, m_type);
	DDX_Text(pDX, IDC_BINS, m_bins);
	DDX_Text(pDX, IDC_WCELL, m_wcell);
	DDX_Text(pDX, IDC_HCELL, m_hcell);
	DDX_Text(pDX, IDC_WBLOCK, m_wblock);
	DDX_Text(pDX, IDC_HBLOCK, m_hblock);
	DDX_Text(pDX, IDC_WWINDOW, m_wwindow);
	DDX_Text(pDX, IDC_HWINDOW, m_hwindow);
	DDX_Text(pDX, IDC_OVERLAP, m_overlap);
}

BEGIN_MESSAGE_MAP(CPersonDetectionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE, &CPersonDetectionDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_DoOPEN, &CPersonDetectionDlg::OnBnClickedDoopen)
	ON_BN_CLICKED(IDC_DoCALC, &CPersonDetectionDlg::OnBnClickedDocalc)
	ON_WM_DESTROY()
	ON_STN_CLICKED(IDC_PICREVIEW, &CPersonDetectionDlg::OnStnClickedPicreview)
	ON_BN_CLICKED(IDC_BROWSE_TRAIN, &CPersonDetectionDlg::OnBnClickedBrowseTrain)
	ON_BN_CLICKED(IDC_START_TRAINING, &CPersonDetectionDlg::OnBnClickedStartTraining)
	ON_BN_CLICKED(IDC_START_DETECTING, &CPersonDetectionDlg::OnBnClickedStartDetecting)
	ON_BN_CLICKED(IDC_BROWSE_TEST, &CPersonDetectionDlg::OnBnClickedBrowseTest)
	ON_BN_CLICKED(IDC_BROWSE_RES, &CPersonDetectionDlg::OnBnClickedBrowseRes)
	ON_BN_CLICKED(IDC_BROWSE_SVM_TRAIN, &CPersonDetectionDlg::OnBnClickedBrowseSvmTrain)
	ON_BN_CLICKED(IDC_DoDETECT, &CPersonDetectionDlg::OnBnClickedDodetect)
	ON_STN_CLICKED(IDC_PIC_RESULT, &CPersonDetectionDlg::OnStnClickedPicResult)
	ON_BN_CLICKED(IDC_DoSaveResult, &CPersonDetectionDlg::OnBnClickedDosaveresult)
END_MESSAGE_MAP()


// CPersonDetectionDlg message handlers

BOOL CPersonDetectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPersonDetectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPersonDetectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	if ( m_bLoad == true )
	{
		CRect rct;
		// Pic 1
		m_cPicView.GetClientRect(&rct);

		CvvImage vImg;
		vImg.CopyOf(m_pImage,1);
		vImg.DrawToHDC(m_cPicView.GetDC()->m_hDC,rct);
	}
	if (m_resLoad == true)
	{
		CRect rct;
		// Pic 1
		m_cPicRes.GetClientRect(&rct);

		CvvImage vImg;
		vImg.CopyOf(m_resImage,1);
		vImg.DrawToHDC(m_cPicRes.GetDC()->m_hDC,rct);
	}
	else
	{
		CRect rct;
		// Pic 1
		m_cPicRes.GetClientRect(&rct);

		CvvImage vImg;
		vImg.CopyOf(m_pImage,1);
		vImg.DrawToHDC(m_cPicRes.GetDC()->m_hDC,rct);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPersonDetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPersonDetectionDlg::OnBnClickedBrowse()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,NULL,OFN_EXTENSIONDIFFERENT,_T("Image (*.jpg, *.jpeg, *.png, *.bmp)|*.jpg;*.jpeg;*.png;*.bmp|All Files (*.*)|*.*|"),0);

	if ( dlg.DoModal() == IDOK )
	{
		m_strPath = dlg.GetPathName();
		UpdateData(FALSE);
		// Kiem tra da co du lieu chua
		if ( m_pImage != NULL )
			cvReleaseImage(&m_pImage);
		if (m_bCalc == true )
			m_bCalc = false;
		if ( !m_save.IsEmpty())
			m_save.Empty();
		/////////////////////
		char* str = (char*) (LPCTSTR) m_strPath;
		m_pImage = cvLoadImage(str );
		m_bLoad = true;
		this->Invalidate(FALSE);		
	}
}

void CPersonDetectionDlg::OnBnClickedDosaveresult()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if (m_resImage == NULL)
	{
		MessageBox("Please do detect a source image before saving",_T("Failed!!!"),MB_OK | MB_ICONINFORMATION);
		return;	
	}
	CFileDialog dlg(FALSE,NULL,NULL,OFN_EXTENSIONDIFFERENT,_T("Image (*.jpg, *.jpeg, *.png, *.bmp)|*.jpg;*.jpeg;*.png;*.bmp|All Files (*.*)|*.*|"),0);
	if ( dlg.DoModal() == IDOK )
	{
		
		cvSaveImage(dlg.GetPathName(), m_resImage);
		MessageBox("Save Completed", _T("Notification"),MB_OK | MB_ICONINFORMATION);
	}
}

void CPersonDetectionDlg::OnStnClickedPicResult()
{
	
}


void CPersonDetectionDlg::OnBnClickedDoopen()
{
	// TODO: Add your control notification handler code here
	//ShellExecute( NULL, _T("open"), _T("c:\\windows\\notepad.exe"), _T("C:\\setup.log"), _T(""), SW_SHOWNORMAL) ;
	if ( m_bLoad == true && m_bCalc == true)
		ShellExecute( NULL, _T("open"), _T("c:\\windows\\notepad.exe"),m_save, _T(""), SW_SHOWNORMAL) ;
	else
		MessageBox(_T("Chua tinh features cho anh!"),_T("Thong bao"),MB_OK | MB_ICONEXCLAMATION);
}


void CPersonDetectionDlg::OnBnClickedDocalc()
{
	// TODO: Add your control notification handler code here
	if ( m_bLoad == true)
	{
		UpdateData(TRUE);
		
		if ( m_wwindow < m_wcell * m_wblock || m_hwindow < m_hcell * m_hblock )
		{
			MessageBox(_T("Size cua window >= size cua block"),_T("Thong bao"),MB_OK | MB_ICONEXCLAMATION);
			m_wwindow = 64;
			m_hwindow = 128;
			UpdateData(FALSE);
		}
		char* imgname = (char*) (LPCTSTR) m_strPath;
		char fileFeaturesName[400];
		strcpy(fileFeaturesName,imgname);
		strcat(fileFeaturesName,"_features.txt");
		ofstream fileFeatures(fileFeaturesName);
		HOGProcessor hog;
		hog.SetParams(cvSize(m_wcell,m_hcell),cvSize(m_wblock,m_hblock),m_overlap);
		CvMat* features;
		features = hog.calcHOGFromImage(imgname,cvSize(m_wwindow,m_hwindow),1);
		hog.writeFeatureVector(features,"test",fileFeatures);
		m_save = fileFeaturesName;
		m_bCalc = true;
		MessageBox(_T("Da tinh xong features cho anh!"),_T("Thong bao"),MB_OK | MB_ICONINFORMATION);
	}
	else
		MessageBox(_T("Chua load anh!"),_T("Thong bao"),MB_OK | MB_ICONEXCLAMATION);
}


void CPersonDetectionDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	if (m_pImage != NULL)
		cvReleaseImage(&m_pImage);
	if (m_resImage != NULL)
		cvReleaseImage(&m_resImage);
}


void CPersonDetectionDlg::OnStnClickedPicreview()
{
	// TODO: Add your control notification handler code here
}

//char* CPersonDetectionDlg::ConverterStringToChar(const CString &s)
//{	
//	int nSize = s.GetLength();
//	char *pAnsiString = new char[nSize+1];
//	memset(pAnsiString,0,nSize+1);
//	wcstombs(pAnsiString, s, nSize+1);
//	return pAnsiString;	 
//}

string CPersonDetectionDlg::convertCStringToString(CString CStr)
{
	 CT2CA pszConvertedAnsiString(CStr);
	 string str(pszConvertedAnsiString);
	 return str;
}

void CPersonDetectionDlg::StringExplode(string str, string separator, vector<string>* results)   //Extra function to spit string by separators
{
	int found;
	found = str.find_first_of(separator);
	while(found != string::npos){
		if(found > 0){
			results->push_back(str.substr(0,found));
		}
		str = str.substr(found+1);
		found = str.find_first_of(separator);
	}
	if(str.length() > 0){
		results->push_back(str);
	}
}


bool CPersonDetectionDlg::GetFolder(std::string& folderpath, 
						const char* szCaption, HWND hOwner)
{
   bool retVal = false;

   // The BROWSEINFO struct tells the shell 
   // how it should display the dialog.
   BROWSEINFO bi;
   memset(&bi, 0, sizeof(bi));

   bi.ulFlags   = BIF_USENEWUI;
   bi.hwndOwner = hOwner;
   bi.lpszTitle = szCaption;

   // must call this if using BIF_USENEWUI
   ::OleInitialize(NULL);

   // Show the dialog and get the itemIDList for the 
   // selected folder.
   LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

   if(pIDL != NULL)
   {
      // Create a buffer to store the path, then 
      // get the path.
      char buffer[_MAX_PATH] = {'\0'};
      if(::SHGetPathFromIDList(pIDL, buffer) != 0)
      {
         // Set the string value.
         folderpath = buffer;
         retVal = true;
      }

      // free the item id list
      CoTaskMemFree(pIDL);
   }

   ::OleUninitialize();

   return retVal;
}

void CPersonDetectionDlg::OnBnClickedBrowseTrain()
{
	std::string szPath("");

	if (GetFolder(szPath, "Select a folder.") == true) 
	{
		UpdateData(TRUE);
		m_strTrainPath = szPath.c_str();
		UpdateData(FALSE);
		this->Invalidate(FALSE);
	}
	else
	{
		printf("No folder selected!\n");
	}
}

void CPersonDetectionDlg::OnBnClickedBrowseTest()
{
	std::string szPath("");

	if (GetFolder(szPath, "Select a folder.") == true) 
	{
		UpdateData(TRUE);
		m_strTestPath = szPath.c_str();
		UpdateData(FALSE);
		this->Invalidate(FALSE);
	}
	else
	{
		printf("No folder selected!\n");
	}
}

void CPersonDetectionDlg::OnBnClickedBrowseSvmTrain()
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_EXTENSIONDIFFERENT,_T("XML (*.xml)|*.xml|All Files (*.*)|*.*|"),0);

	if ( dlg.DoModal() == IDOK )
	{
		m_strSVMTrainPath = dlg.GetPathName();
		UpdateData(FALSE);
		//this->Invalidate(FALSE);		
		
		strcpy(m_strSVMTrain,  (char*) (LPCTSTR) m_strSVMTrainPath);
		MessageBox(_T("The SVM Train Data is now loading. \nPlease wait for a few second! OK?"),_T("Notification"), MB_OK | MB_ICONINFORMATION);
		detector.setSVMTrainPath(m_strSVMTrain);
		MessageBox(_T("SVM Train Data Loading Complete"),_T("Notification"), MB_OK | MB_ICONINFORMATION);
	}
}

void CPersonDetectionDlg::OnBnClickedBrowseRes()
{	
	std::string szPath("");

	if (GetFolder(szPath, "Select a folder.") == true) 
	{
		UpdateData(TRUE);
		m_strResPath = szPath.c_str();
		UpdateData(FALSE);
		this->Invalidate(FALSE);
	}
	else
	{
		printf("No folder selected!\n");
	}
}

void CPersonDetectionDlg::OnBnClickedStartTraining()
{
	// Dataset/small train/
	/*char* trainPath = m_strTrainPath*/
	//char posDat[200];
	//memcpy(posDat, m_strTrainPath, 200);
	//memcpy(&posDat[200], "pos/pos_train(", 200);
	UpdateData(FALSE);
	string trainPath = convertCStringToString(m_strTrainPath);
	string testPath = convertCStringToString(m_strTestPath);
	trainPath.append("/pos/pos_train(");
	testPath.append("/neg/neg_train(");
	char* format = ").png";
	CvSize cell = cvSize(8,8);
	CvSize window = cvSize(64, 128);
	int nSamples = 60;
	int sInd = 1;
	int eInd = 60;
	string posTrain = convertCStringToString(m_strTrainPath);
	string negTrain = convertCStringToString(m_strTestPath);
	string saveTrainPath =posTrain;
	posTrain.append("/train64x128_Pos.xml");
	negTrain.append("/train64x128_Neg.xml");
	
	hogClassifier.trainDataset_64x128((char*) trainPath.c_str(), format, cell, window, nSamples, sInd, eInd, (char*)posTrain.c_str());
	//hogClassifier.trainDataset_64x128(negDat, format, cell, window, nSamples, sInd, eInd, negTrain);
	hogClassifier.trainLargeDataset((char*) testPath.c_str(), format, cell, window, nSamples, 5, 2, sInd, eInd, (char*)negTrain.c_str());
	hogClassifier.trainSVM(NULL, NULL, (char*) saveTrainPath.append("/TrainSVM.xml").c_str(), (char*) posTrain.c_str(), (char*)negTrain.c_str());
	MessageBox(_T("Hoan tat qua trinh Train!"),_T("Congratulation!!!"),MB_OK | MB_ICONINFORMATION);
}

//int CPersonDetectionDlg::readFilesInFolder(string dir, string foldername)
//{
//	string filepath;
//	int num;
//	DIR *dp;
//	struct dirent *dirp;
//	struct stat filestat;
//
//	dp = opendir( dir.c_str() );
//	if (dp == NULL)
//	{
//		MessageBox("Cannot found the specified folder",_T("Error!"),MB_OK | MB_ICONINFORMATION);
//		return errno;
//	}
//
//	string testResultPath = convertCStringToString(m_strTestPath);
//	testResultPath.append("/Result/").append(foldername);
//
//	while ((dirp = readdir( dp )))
//	{
//		filepath = dir + "/" + dirp->d_name;
//
//		// If the file is a directory (or is in some way invalid) we'll skip it 
//		if (stat( filepath.c_str(), &filestat )) continue;
//		if (S_ISDIR( filestat.st_mode ))         continue;
//
//		if (m_resImage != NULL)
//			cvReleaseImage(&m_resImage);
//		
//		m_resImage = detector.DetectMultiScale((char*)filepath.c_str());
//		
//		cvSaveImage((char*)testResultPath.c_str(), m_resImage);
//	}
//
//	closedir( dp );
//	return 0;
//}

void CPersonDetectionDlg::OnBnClickedStartDetecting()
{
	char path[400];	
	strcpy(path,  (char*) (LPCTSTR) m_strPath);
	if (strlen(m_strSVMTrain) == 0)
	{
		char* temp = (char*) (LPCTSTR) m_strSVMTrainPath;
		strcpy(m_strSVMTrain,  temp);
		detector.setSVMTrainPath(m_strSVMTrain);
	}

	string testsPath = convertCStringToString(m_strTestPath);
	string posTestsPath = testsPath.append("/pos/");
	string negTestsPath = testsPath.append("/neg/");

	char* testDef = "Dataset/Test/testDef.txt";

	char testFile[1000];
	string fileName;
	fstream file;
	file.open(testDef, ios::in);

	while (!file.eof())
	{
		file.getline(testFile, 1000);				
		detector.DetectMultiScale(testFile);
	}
	file.close();	

	MessageBox(_T("Hoan tat qua trinh Detect!!!"),_T("Thong bao"),MB_OK | MB_ICONINFORMATION);		
}

void CPersonDetectionDlg::OnBnClickedDodetect()
{
	if (m_strPath == "")
	{
		MessageBox(_T("Please choose an image first!!!"),_T("ERROR!!!"),MB_OK | MB_ICONWARNING);
		return;
	}
	char path[400];	
	strcpy(path,  (char*) (LPCTSTR) m_strPath);	
	if (strlen(m_strSVMTrain) == 0)
	{		
		char* temp = (char*) (LPCTSTR) m_strSVMTrainPath;
		ifstream ifile(temp);
		if (!ifile)
		{
			MessageBox(_T("SVM Train file is not exist!!!"),_T("ERROR!!!"),MB_OK | MB_ICONWARNING);
			return;
		}
		strcpy(m_strSVMTrain,  temp);
		MessageBox(_T("The SVM Train Data is now loading. \nPlease wait for a few second! OK?"),_T("Notification"), MB_OK | MB_ICONINFORMATION);
		detector.setSVMTrainPath(m_strSVMTrain);
		MessageBox(_T("SVM Train Data Loading Complete"),_T("Notification"), MB_OK | MB_ICONINFORMATION);
	}

	clock_t startTime, endTime;	
	startTime = clock();
	//startTime = time(NULL);	
	IplImage* resultImage = detector.DetectMultiScale(path);
	//endTime = time(NULL);
	endTime = clock();
	//m_pImage = cvLoadImage(str);
	
	char totalTime[20];
	itoa((int)(endTime - startTime) / CLOCKS_PER_SEC, totalTime, 10);

	char strResult[100];
	UpdateData(FALSE);
	if (resultImage != NULL)
	{
		strcpy(strResult,"Da detect duoc Nguoi!!!\n\tThoi gian Detect: ");
		if (m_resImage != NULL)
			cvReleaseImage(&m_resImage);
		cvShowImage("Detect Result: ", resultImage);
		m_resImage = resultImage;
		m_resLoad = true;
	}
	else
	{
		m_resLoad = false;
		strcpy(strResult, "Khong co doi tuong nguoi trong anh.\n\tThoi gian Detect: ");		
	}
	this->Invalidate(FALSE);

	int len1 = strlen(strResult);
	int len2 = strlen(totalTime);
	for (int i = 0; i < len2; i++)
	{
		strResult[i + len1] = totalTime[i];
	}
	strResult[len1 + len2] = '\0';
	strcat(strResult, " (s)");
		
	MessageBox(strResult,_T("Ket qua Detect!!!"),MB_OK | MB_ICONINFORMATION);
}
