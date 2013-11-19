#include "StdAfx.h"
#include "LineSegment.h"


LineSegment::LineSegment(void)
{
	_a = _b = 0;
}


LineSegment::~LineSegment(void)
{
	
}

LineSegment::LineSegment(CvPoint p1, CvPoint p2)
{
	_begin = p1;
	_end = p2;

	CvMat *A = cvCreateMat(2, 2, CV_32FC1);
	CvMat *X = cvCreateMat(2, 1, CV_32FC1);
	CvMat *B = cvCreateMat(2, 1, CV_32FC1);

	cvSetReal2D(A, 0, 0, (int) p1.x);
	cvSetReal2D(A, 0, 1, (int) 1);
	cvSetReal2D(A, 1, 0, (int) p2.x);
	cvSetReal2D(A, 1, 1, (int) 1);

	cvSetReal2D(B, 0, 0, (int) p1.y);
	cvSetReal2D(B, 1, 0, (int) p2.y);

	// giai he phuong trinh de tim he so a, b cho phuong trinh tham so cua duong thang
	cvSolve(A, B, X, 0);

	_a = (int) cvGetReal2D(X, 0, 0);
	_b = (int) cvGetReal2D(X, 1, 0);

	if (_a < 0)
	{
		_a = -1;
		_b = -1;
	}

	/*cvReleaseData(&A);
	cvReleaseData(&X);
	cvReleaseData(&B);*/
	cvReleaseMat(&A);
	cvReleaseMat(&B);
	cvReleaseMat(&X);

}

void LineSegment::calculateCoEfficient()
{
	CvMat *A = cvCreateMat(2, 2, CV_32FC1);
	CvMat *X = cvCreateMat(2, 1, CV_32FC1);
	CvMat *B = cvCreateMat(2, 1, CV_32FC1);

	cvSetReal2D(A, 0, 0, (int) _begin.x);
	cvSetReal2D(A, 0, 1, (int) 1);
	cvSetReal2D(A, 1, 0, (int) _end.x);
	cvSetReal2D(A, 1, 1, (int) 1);

	cvSetReal2D(B, 0, 0, (int) _begin.y);
	cvSetReal2D(B, 1, 0, (int) _end.y);

	// giai he phuong trinh de tim he so a, b cho phuong trinh tham so cua duong thang
	cvSolve(A, B, X, 0);

	_a = (int) cvGetReal2D(X, 0, 0);
	_b = (int) cvGetReal2D(X, 1, 0);

	if (_a < 0)
	{
		_a = -1;
		_b = -1;
	}
	cvReleaseMat(&A);
	cvReleaseMat(&B);
	cvReleaseMat(&X);
}
CvPoint LineSegment::getIntersection(LineSegment *l1, LineSegment *l2)
{
	CvPoint intersection = cvPoint(-1, -1);

	// neu 2 duong thang song song -> return ko cat nhau
	if ((l1->_a * l2->_b) == (l2->_a * l1->_b))
	{
		return intersection;
	}

	if (l1->_a == -1 || l2->_a == -1)
	{
		if (l1->_a == -1)
		{
			intersection.x = l1->_begin.x;
			intersection.y = l2->_a * intersection.x + l2->_b;
		}
		if (l2->_a == -1)
		{
			intersection.x = l2->_begin.x;
			intersection.y = l1->_a * intersection.x + l1->_b;
		}
	}
	else
	{
		intersection.x = (int) ((l2->_b - l1->_b) / (l1->_a - l2->_a));
		intersection.y = (int) (l1->_a * intersection.x + l1->_b);
	}
	

	// kiem tra xem giao diem co nam trong 2 doan thang hay ko?
	if ((intersection.x >= l1->_begin.x && intersection.x <= l1->_end.x && intersection.y >= l1->_begin.y && intersection.y <= l1->_end.y)
		&& (intersection.x >= l2->_begin.x && intersection.x <= l2->_end.x && intersection.y >= l2->_begin.y && intersection.y <= l2->_end.y))
	{
		return intersection;
	}

	return cvPoint(-1, -1);
}