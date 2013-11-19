#include "StdAfx.h"
#include "ScanWindow.h"


ScanWindow::ScanWindow(void)
{
	_xCoordinate = _yCoordinate = _scale = 0;
}


ScanWindow::~ScanWindow(void)
{

}

ScanWindow::ScanWindow(float x, float y, float scale, float width, float height)
{
	_xCoordinate = x;
	_yCoordinate = y;
	_scale = scale;
	_width = width;
	_height = height;
}

ScanWindow::ScanWindow(float x, float y, float scale)
{
	_xCoordinate = x;
	_yCoordinate = y;
	_scale = scale;
}

void ScanWindow::setXCoordinate(float xValue)
{
	_xCoordinate = xValue;
}

void ScanWindow::setYCoordinate(float yValue)
{
	_yCoordinate = yValue;
}

void ScanWindow::setWidth(float w)
{
	_width = w;
}

void ScanWindow::setHeight(float h)
{
	_height = h;
}

void ScanWindow::setScale(float sValue)
{
	_scale = sValue;
}

void ScanWindow::setPosition(float xValue, float yValue)
{
	_xCoordinate = xValue;
	_yCoordinate = yValue;
}

void ScanWindow::setWeight(float wValue)
{
	_weight = wValue;
}

float ScanWindow::getXCoordinate()
{
	return _xCoordinate;
}

float ScanWindow::getYCoordinate()
{
	return _yCoordinate;
}

float ScanWindow::getScale()
{
	return _scale;
}

vector<float> ScanWindow::getPosition()
{
	vector<float> position;
	position.push_back(_xCoordinate);
	position.push_back(_yCoordinate);
	return position;
}

float ScanWindow::getWeight()
{
	return _weight;
}

float ScanWindow::getWidth()
{
	return _width;
}

float ScanWindow::getHeight()
{
	return _height;
}
