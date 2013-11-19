#pragma once
#include "cxcore.h"
#include "cv.h"
#include "highgui.h"
#include "iostream"
#include "fstream"
using namespace std;
#define PI 3.14
class HOGProcessor
{	
public:
	static const int NumOfBins = 9;
	CvSize m_cell;	// kich thuo mac dinh cua moi cell la 8 x 8 (pixels)
	CvSize m_block;	// kich thuo mac dinh cua moi block la 2 x 2 (cells)
	float m_fStepOverlap; // duoc tinh theo don vi ti le cell

	HOGProcessor(void);
	void SetParams(CvSize cell, CvSize block, float stepOverLap);
	int getWindowFeatureVectorSize(CvSize window);
	int getWindowFeatureVectorSize(CvSize window, CvSize block, CvSize cell, float overlap);
	IplImage* doSobel(IplImage* in,int xorder, int yorder, int apertureSize);
	IplImage** calculateIntegralHOG(IplImage* in);
	void calcHOGForCell(CvRect cell, CvMat* hogCell, IplImage** integrals,int normalization);
	void calcHOGForBlock(CvRect block, CvMat* hogBlock, IplImage** integrals,CvSize cell, int normalization);
	CvMat* calcHOGWindow(IplImage** integrals, CvRect window, int normalization) ;
	CvMat* calcHOGWindow(IplImage *img, IplImage** integrals, CvRect window, int normalization) ;
	CvMat* calcHOGFromImage(char* filename, CvSize window, int normalization) ;
	void writeFeatureVector(CvMat* mat,char* className,ofstream &fout);

	~HOGProcessor(void);
};

