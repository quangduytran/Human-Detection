#pragma once
#include "HOGClassifier.h"
#include "ScanWindow.h"
#include "OverlappingDetection.h"
#include "Localisation.h"
#include "IntegralScale.h"

class PersonDetector
{
private:
	int DetectPosWindows(IplImage* img);
	int m_widthStep;	// tinh theo pixel
	int m_heigthStep; 
	CvSize m_cell;
	CvSize m_block;
	CvSize m_winSize;
	int m_stepOverlap;

	// Tham so dung cho viec duyet cua so
	float startScale;
	float endScale;
	float scaleStep;
	int nScaleStep;

	char* svmTrainPath;

	void calcScaleStep();
public:
	PersonDetector();

	HOGClassifier classifier;
	vector<ScanWindow*> m_detectedWindows;

	void setParams(CvSize cell, CvSize block, float stepOverLap);
	void setSVMTrainPath(char* path);
	void drawRect2Img(CvMat& img, string rectFile,float minValue, CvRect realRect,bool isMeanshift);
	IplImage* DetectMultiScale(char* srcImgPath);	
	
	CvRect getRect(int x,int y, float scale);
};




