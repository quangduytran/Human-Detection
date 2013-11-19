#include "StdAfx.h"
#include "PersonDetector.h"
#include <math.h>

PersonDetector::PersonDetector()
{
	m_cell = cvSize(8, 8);
	m_block = cvSize(2, 2);
	m_winSize = cvSize(64, 128);
	m_stepOverlap = 1;
	classifier.SetParams(m_cell, m_block , m_stepOverlap);

	startScale = 1;
	scaleStep = 1.5;
	m_widthStep = 8;
	m_heigthStep = 8;

	svmTrainPath = "Dataset/small train/TrainSVM.xml";	
	
}

void PersonDetector::setParams(CvSize cell, CvSize block, float stepOverLap)
{
	m_cell = cell;
	m_block = block;
	m_stepOverlap = stepOverLap;
	classifier.SetParams(cell, block , stepOverLap);
}

void PersonDetector::setSVMTrainPath(char* path)
{
	svmTrainPath = path;
	classifier.loadTrainSVM(path);
}

int PersonDetector::DetectPosWindows(IplImage* img)
{
	int nWins = 0;

	return nWins;
}

/// Ham detect people multiscale
// Params: srcImgPath: Duong dan toi file anh test
// Return value: so luong nguoi detect duoc
IplImage* PersonDetector::DetectMultiScale(char* srcImgPath)
{
	int count = 0;
	IplImage* srcImg = cvLoadImage(srcImgPath, 1);
	CvSize size = cvGetSize(srcImg);

	// 
	vector<IntegralScale*> integralsScale;
	//
	if (size.width > 400)
	{
		//IplImage* dst = cvCreateImage(cvSize((int)(srcImg->width / 2), (int)(srcImg->height / 2)),
		//								srcImg->depth, srcImg->nChannels);
		//cvResize(srcImg, dst);
		//cvReleaseImage(&srcImg);
		//srcImg = cvCloneImage(dst);
		startScale = 2.5;		
	}
	else if (size.width > 300)
	{
		startScale = 2;
	}
	// Tinh toan so buoc scale anh	
	endScale = min((float)size.width / m_winSize.width, (float)size.height / m_winSize.height);
	nScaleStep = floor(log(endScale / startScale) / log(scaleStep) + 1); 
	
	// Khoi tao kich thuoc scale ban dau
	float scale = startScale;	

	// Duyet qua tat ca cac muc scale
	for (int k = 0; k <= nScaleStep; k++)
	{
		IplImage* input = NULL;
		if (k == nScaleStep)
		{
			// Kiem tra neu buoc scale truoc do da cho ti le anh input bang ti le cua so thi
			if ((int)(srcImg->width / scale) == m_winSize.width &&
				(int)(srcImg->height / scale) == m_winSize.height)
				break;
			else // Nguoc lai thuc hien them 1 lan test voi kich thuoc anh scale bang dung kich thuoc cua so
				input = cvCreateImage(cvSize((int)(m_winSize.width), (int)(m_winSize.height)),
													srcImg->depth, srcImg->nChannels);
		}
		else
		{			
			input = cvCreateImage(cvSize((int)(srcImg->width / scale), (int)(srcImg->height / scale)),
													srcImg->depth, srcImg->nChannels);
			
		}
		cvResize(srcImg, input);

		int x = 0;
		int y = 0;
		int nBins = classifier.hog.NumOfBins;
		IplImage** integrals = classifier.calcIntegralHOG(input);

		// add integral image + scale vao list
		IntegralScale *integralScaleImage = new IntegralScale(integrals, scale);
		
		integralsScale.push_back(integralScaleImage);
		//

		IplImage** integralsInput = (IplImage**)malloc(nBins * sizeof(IplImage*));		
		while(y + m_winSize.height <= integrals[0]->height)
		{
			x = 0;
			while (x + m_winSize.width <= integrals[0]->width)
			{	
				// set region of interest				

				for (int i = 0; i < nBins; i++)
				{
					cvSetImageROI(integrals[i], cvRect(x, y, m_winSize.width + 1, m_winSize.height + 1));
					integralsInput[i] = cvCreateImage(cvGetSize(integrals[i]),integrals[i]->depth, integrals[i]->nChannels);
					cvCopy(integrals[i], integralsInput[i], NULL);
				}
				//IplImage *img = cvCreateImage(cvGetSize(input),input->depth, input->nChannels);
				// copy subimage
				//cvCopy(input, img, NULL);
				float weight = classifier.testSVM(svmTrainPath, integralsInput, NULL);
				if (weight != 2) // -2.4115036f)
				{					
					ScanWindow *window = new ScanWindow(x, y, scale, 64, 128);		// changed 3rd parameter from 1 to scale
					window->setWeight(weight);
					m_detectedWindows.push_back(window);
				}
				for (int i = 0; i < nBins; i++)
					cvResetImageROI(integrals[i]);
				

				x += m_widthStep;
				//cvReleaseImage(&img);
				for (int i = 0; i < nBins; i++)
					cvReleaseImage(&integralsInput[i]);
			}
			y += m_heigthStep;
		}

		for (int i = 0; i < nBins; i++)
			cvReleaseImage(&integrals[i]);

		free(integrals);
		free(integralsInput);
		cvReleaseImage(&input);
		scale *= scaleStep;
	}


	// PHAN LOCALIZATION 
	// Input: tap cac cua so o cac muc scale da detect duoc tren anh test input
	// Output: tra ra tap cac cua so da HOI TU

	
	int numberWindows = -1;

	OverlappingDetection *overlapDetector;
	vector<CvMat*> boundaryList;

	while (true)
	{
		overlapDetector = new OverlappingDetection(&m_detectedWindows);
		// cluster nhung cum cua so overlap len nhau
		overlapDetector->clusterWindows();

		for (int i = 0; i < overlapDetector->_clusteredWindows.size(); i++)
		{
			// doi voi moi cum, tao ra mot doi tuong localization de localize
			ScanWindow** listOverlapped = new ScanWindow*[overlapDetector->_clusteredWindows[i].size()];
			for (int j = 0; j < overlapDetector->_clusteredWindows[i].size(); j++)
			{
				listOverlapped[j] = m_detectedWindows[overlapDetector->_clusteredWindows[i][j]];
			}

			//Localisation *localize = new Localisation(overlapDetector->_clusteredWindows[i], m_widthStep, m_heigthStep, log(1.03), scaleStep, &classifier, svmTrainPath, srcImg, integralsScale);
			Localisation *localize = new Localisation(listOverlapped, m_widthStep, m_heigthStep, log(1.03), scaleStep, &classifier, svmTrainPath, srcImg, integralsScale);
			//localize->_detectedWindows = overlapDetector->_clusteredWindows[i];

			// tinh toan cua so cuoi cung de tim boundary 
			localize->localize2(overlapDetector->_clusteredWindows[i].size());

			// lay ra boundary
			boundaryList.push_back(localize->_finalLocalisationWindow);
			for (int j = 0; j < overlapDetector->_clusteredWindows[i].size(); j++)
			{
				delete listOverlapped[j]; 
			}
			delete listOverlapped;
			delete localize;
		}
		
		if (boundaryList.size() == numberWindows)
		{
			break;
		}
		else
		{
			
			numberWindows = boundaryList.size();
			// huy vung nho
			m_detectedWindows.clear();

			for (int i = 0; i < boundaryList.size(); i++)
			{
				float scaleWindow = cvGetReal2D(boundaryList[i], 2, 0);
				int x = (int) cvGetReal2D(boundaryList[i], 0, 0);
				int y = (int) cvGetReal2D(boundaryList[i], 1, 0);
				x = (int) x / scaleWindow;
				y = (int) y/ scaleWindow;
				ScanWindow* window = new ScanWindow(x, y, scaleWindow, 64, 128);
				m_detectedWindows.push_back(window);
			}
			//

			for (int i = 0; i < boundaryList.size(); i++)
			{
				cvReleaseMat(&boundaryList[i]);
			}

			free(overlapDetector);
			boundaryList.clear();

		}
	}
	

	// ket thuc phan localization

	// Ve Bounding box o day, su dung danh sach boundaryList 
	// Trong do, cua so thu i bao gom
	// x = cvGetReal2D(boundaryList[i], 0, 0)
	// y = cvGetReal2D(boundaryList[i], 1, 0)
	// scale = cvGetReal2D(boundaryList[i], 2, 0)
	IplImage* resultImage = cvCloneImage(srcImg);

	float scaleWindow;
	int x, y;
	float width, height;
	for (int i = 0; i < boundaryList.size(); i++)
	{
		//scaleWindow = pow(10, (float) cvGetReal2D(boundaryList[i], 2, 0));
		//scaleWindow = exp((float) cvGetReal2D(boundaryList[i], 2, 0));
		scaleWindow = cvGetReal2D(boundaryList[i], 2, 0);
		x = (int) cvGetReal2D(boundaryList[i], 0, 0);
		y = (int) cvGetReal2D(boundaryList[i], 1, 0);
		//				
		//

		width = 64 * scaleWindow;
		height = 128 * scaleWindow;

		cvRectangle(resultImage, cvPoint(x, y), cvPoint(x + (int) width, y + (int) height), cvScalar(0, 255, 255, 0), 2, 8, 0);
		
	}

	
	//float scaleWindow;
	//int x, y;
	//int width, height;
	//for (int i = 0; i < m_detectedWindows.size(); i++)
	//{
	//	//scaleWindow = pow(10, (float) cvGetReal2D(boundaryList[i], 2, 0));
	//	scaleWindow = m_detectedWindows[i]->getScale();
	//	x = (int) m_detectedWindows[i]->getXCoordinate();
	//	y = (int) m_detectedWindows[i]->getYCoordinate();
	//	width = 64 * scaleWindow;
	//	height = 128 * scaleWindow;

	//	cvRectangle(resultImage, cvPoint(x, y), cvPoint(x + (int) width, y + (int) height), cvScalar(0, 255, 255, 0), 2, 8, 0);
	//		
	//}
	
	
	/*string sFile(srcImgPath);
	int pos = sFile.find_last_of('/');
	string sFileOut = sFile.substr(pos + 1, sFile.length() - pos - 1);
	sFileOut = "Result/neg/" + sFileOut;

	cvSaveImage(sFileOut.c_str(), resultImage, 0);
	cvReleaseImage(&resultImage);*/

	// ket thuc ve bounding box
	
	//count = m_detectedWindows.size();
	count = boundaryList.size();
	
	// huy vung nho	
	for (int i = 0; i < boundaryList.size(); i++)
	{
		cvReleaseMat(&boundaryList[i]);
	}

	if (srcImg != NULL)
		cvReleaseImage(&srcImg);

	// free vung nho
	free(overlapDetector);
	boundaryList.clear();
	integralsScale.clear();
	m_detectedWindows.clear();

	return resultImage;
}