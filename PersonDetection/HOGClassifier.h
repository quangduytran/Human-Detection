#pragma once
#include "cxcore.h"
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include <stdio.h>
#include "iostream"
#include "fstream"
#include "HOGProcessor.h"
using namespace std;

class HOGClassifier
{
private:
	
public:
	//CvSize m_cell;
	//CvSize m_block;
	//float m_fStepOverlap;
	CvSVM svmClassifier;
	HOGProcessor hog;

	HOGClassifier(void);
	HOGClassifier(HOGProcessor hogProc);

	char *cvtInt( char *str, int num);
	void SetParams(CvSize cell, CvSize block, float stepOverLap);

	IplImage** calcIntegralHOG(IplImage* in);

	CvMat* trainDataset_64x128(char *prefix, char *suffix, CvSize cell,
					CvSize window, int nSamples, int startInd,
					int endInd, char *saveXML = NULL, int canny = 0,
					int block = 1, int normalization = 4);
	
	CvMat* trainLargeDataset(char *prefix, char *suffix,
					CvSize cell, CvSize window, int nImages,
					int horizontalScans, int verticalScans,
					int startInd, int endInd,
					char *saveXML = NULL, int normalization = 4);	

	void loadTrainSVM(char* trainXMLPath);

	void trainSVM(CvMat* posMat, CvMat* negMat, char *saveXML,
					char *posData = NULL, char *negData = NULL);

	float testSVM(char *trainXML, char *testData, char *saveData);
	float testSVM(char *trainXML, IplImage *testImg, char *saveData);
	// Ham TestSVM su dung integrals HOG (ung voi moi bins)
	float testSVM(char *trainXML, IplImage** integrals, char *saveData);
	float testSVM(char *trainXML, CvMat* win, int pos, int width, int height, char *saveData);
	void detectMultiScale();

	~HOGClassifier(void);
};

