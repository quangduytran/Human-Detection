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
using namespace std;

class LineSegment
{
public:
	float _a;
	float _b;
	CvPoint _begin;
	CvPoint _end;
public:
	LineSegment(void);
	~LineSegment(void);
	LineSegment(CvPoint p1, CvPoint p2);
	void calculateCoEfficient();
	CvPoint getIntersection(LineSegment *l1, LineSegment *l2);
	
};

