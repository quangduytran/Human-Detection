#include "StdAfx.h"
#include "IntegralScale.h"


IntegralScale::IntegralScale(void)
{
	_scale = 1;
	_integral = NULL;
}


IntegralScale::~IntegralScale(void)
{
	if (_integral != NULL)
	{
	for (int i = 0; i < 9; i++)
	{
	if (_integral[i] != NULL)
	{
	cvReleaseImage(&_integral[i]);
	}
	}

	free(_integral);
	}

}

IntegralScale::IntegralScale(IplImage** integral, float scale)
{
	//_integral = integral;
	_integral = (IplImage**)malloc(9 * sizeof(IplImage*));		
	for (int i = 0; i < 9; i++)
	{
		_integral[i] = cvCloneImage(integral[i]);
	}

	_scale = scale;
}
