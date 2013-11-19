#pragma once
#include "cxcore.h"
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include <stdio.h>
#include "iostream"
#include "fstream"
#include <vector>
#include "HOGProcessor.h"

class ScanWindow
{

private:
	float _xCoordinate;
	float _yCoordinate;
	float _width;
	float _height;
	float _scale;
	float _weight;

public:
	ScanWindow(void);
	~ScanWindow(void);
	ScanWindow(float x, float y, float scale);
	ScanWindow(float x, float y, float scale, float width, float height);
	void setXCoordinate(float xValue);
	void setYCoordinate(float yValue);

	void setWidth(float w);
	void setHeight(float h);

	void setPosition(float xValue, float yValue);
	void setScale(float sValue);
	void setWeight(float wValue);


	float getXCoordinate();
	float getYCoordinate();
	float getWidth();
	float getHeight();
	float getScale();
	float getWeight();
	vector<float> getPosition();
};

