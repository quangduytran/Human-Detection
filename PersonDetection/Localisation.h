#pragma once
#include "cxcore.h"
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include <stdio.h>
#include "iostream"
#include "fstream"
#include "HOGProcessor.h"
#include "ScanWindow.h"
#include "OverlappingDetection.h"
#include "HOGClassifier.h"
#include "IntegralScale.h"
#include <cmath>
using namespace std;

class Localisation
{

public:
	int _numberOfDetectedWindows;
	HOGProcessor hogProcessor;
	HOGClassifier *_hogClassifier;
	IplImage* _testImage;
	char* _trainPath;	

	vector<ScanWindow*> _detectedWindows;	// SCALE luu trong nay la nguyen ban, chua dua qua ham Log
	ScanWindow** _detectedWindows2;
	vector<CvMat*> _positionScaleSpace;	
	vector<float> _logScale;
	vector<float> _weightOfWindows;

	vector<CvMat*> _CovarianceMatrixH;

	vector<IntegralScale*> _integralScale;
	CvMat* _previousMeanShiftWindow;
	CvMat* _finalLocalisationWindow;

	float _sigmaX;	// gia tri paper 8, nhung 4 lai cho best performance
	float _sigmaY;	// gia tri paper 16, nhung 8 lai cho best performance
	float _sigmaS;	// gia tri paper log(1.3)
	float _scaleRatio; // gia tri paper 1.05

public:
	
	Localisation(void);	
	~Localisation(void);
	Localisation(vector<ScanWindow*> detectedWindows, float sigmaX, float sigmaY, float sigmaS, float scaleRatio, HOGClassifier *classifier, char* trainPath, IplImage* testImage, vector<IntegralScale*> integralScale);
	Localisation(ScanWindow** detectedWindows2, float sigmaX, float sigmaY, float sigmaS, float scaleRatio, HOGClassifier *classifier, char* trainPath, IplImage* testImage, vector<IntegralScale*> integralScale);

	void constructPositionScaleSpace();	// tao khong gian 3 chieu x, y, scale (log)
	void calculateCovarianceMatrixH();
	float calculateMahalanobisDistance(CvMat *y, CvMat* yi, CvMat* Hi);
	float clipWiFunction(float wi, float threshold_c, float a, string clippingType);
	float calculatePowerOfDeterminant(CvMat *matrix, float power);
	float calculateVariantPi(int index_yi, int index_y);	// Wi(y)
	CvMat* calculateDataWeightedHarmonicMean(int index_y); // Hh^-1(y)
	CvMat* computeMeanShiftWindow(int index_y); // ym
	void increasePositionScaleSpace(int increaseSize);
	void addASampleToSpace(CvMat* newSample);
	void updateCovarianceMatrix();
	void localize();
	void localize2(int size);
};

