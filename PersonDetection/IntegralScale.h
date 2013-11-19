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
#include <cmath>
using namespace std;

class IntegralScale
{
public: 
	IplImage **_integral;
	float _scale;

public:
	IntegralScale(void);
	~IntegralScale(void);
	IntegralScale(IplImage** integral, float scale);
};

