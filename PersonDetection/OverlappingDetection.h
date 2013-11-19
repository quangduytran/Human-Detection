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
#include "LineSegment.h"
using namespace std;

class OverlappingDetection
{
public:
	int _numberOfWindows;
	vector<ScanWindow*> *_windows;
	vector<vector<int>> _clusteredWindows;
	//int* _group;
	int _nGroup;

public:
	OverlappingDetection(void);
	OverlappingDetection(vector<ScanWindow*> *detectedWindows);
	OverlappingDetection(int numberOfDetectedWindows, vector<ScanWindow*> *detectedWindows);
	~OverlappingDetection(void);
	vector<float> getOverlappingPercent(ScanWindow *window1, ScanWindow *window2);
	void clusterWindows();
};

