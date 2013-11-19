#include "StdAfx.h"
#include "HOGProcessor.h"


HOGProcessor::HOGProcessor(void)
{
	m_cell = cvSize(4, 4);
	m_block = cvSize(2, 2);
	m_fStepOverlap = 1;
}


HOGProcessor::~HOGProcessor(void)
{
}

void HOGProcessor::SetParams(CvSize cell, CvSize block, float stepOverLap)
{
	m_cell = cell;
	m_block = block;
	m_fStepOverlap = stepOverLap;
}

// Ham tinh kich thuoc vector dac trung cho cua so
int HOGProcessor::getWindowFeatureVectorSize(CvSize window)
{	
	int blockFeatureSize = NumOfBins * m_block.height * m_block.width;	
	int blockWidth = m_cell.width * m_block.width;
	int blockHeight = m_cell.height * m_block.height;

	int num_overlap_block_of_width = ((window.width - blockWidth) / 
										(blockWidth - (m_cell.width * m_fStepOverlap))) + 1;
	int num_overlap_block_of_height = ((window.height - blockHeight) / 
										(blockHeight - (m_cell.height * m_fStepOverlap))) + 1;
	int totalBlockOfWindow = num_overlap_block_of_width * num_overlap_block_of_height;

	return totalBlockOfWindow * blockFeatureSize;
}

int HOGProcessor::getWindowFeatureVectorSize(CvSize window, CvSize block, CvSize cell, float overlap)
{
	int blockFeatureSize = NumOfBins * block.height * block.width;	
	int blockWidth = cell.width * block.width;
	int blockHeight = cell.height * block.height;

	int num_overlap_block_of_width = ((window.width - blockWidth) / 
										(blockWidth - (cell.width * overlap))) + 1;
	int num_overlap_block_of_height = ((window.height - blockHeight) / 
										(blockHeight - (cell.height * overlap))) + 1;

	int totalBlockOfWindow = num_overlap_block_of_width * num_overlap_block_of_height;

	return totalBlockOfWindow * blockFeatureSize;
}

//Ham tinh gradient
IplImage* HOGProcessor::doSobel(IplImage* in,int xorder, int yorder, int apertureSize)
{
	IplImage* derivative = cvCreateImage(cvGetSize(in), IPL_DEPTH_32F, 1);
	cvSobel(in, derivative, xorder, yorder, apertureSize);
	return derivative;
}

// Ham tinh integral image cho cac bin image
IplImage** HOGProcessor::calculateIntegralHOG(IplImage* in)
{
	CvSize size = cvGetSize(in);
	cvCreateImage(cvSize(4, 5), IPL_DEPTH_8U, 1);
	
	// Chuyen anh input sang grayscale
	IplImage* imgGray = cvCreateImage(size, IPL_DEPTH_8U, 1);
	cvCvtColor(in, imgGray, CV_BGR2GRAY);
	cvEqualizeHist(imgGray, imgGray);

	// Tinh gradient theo phuong x, y
	// 
	IplImage *xsobel, *ysobel;
	xsobel = doSobel(imgGray, 1, 0, 3);
	ysobel = doSobel(imgGray, 0, 1, 3);
	cvReleaseImage(&imgGray);

	// Tao (nbins) ma tran anh de tich luy cuong do gradient tai moi bin	
	IplImage** bins = (IplImage**)malloc(NumOfBins*sizeof(IplImage*));
	for(int i = 0; i < NumOfBins; i++)
	{
		bins[i] = cvCreateImage(size, IPL_DEPTH_32F, 1);
		cvSetZero(bins[i]);
	}

	// Tao (nbins) integral image
	IplImage** integrals = (IplImage**)malloc(NumOfBins * sizeof(IplImage*));
	for(int i = 0; i < NumOfBins; i++)
		integrals[i] = cvCreateImage(cvSize(in->width+1, in->height +1), IPL_DEPTH_64F,1);
	

	// Tinh cuong do va huong cua gradient tai moi pixel tren anh
	// Dong thoi chia bin cho cac pixel nay
	char buffer[50];
	int x, y;
	float xDevValue, yDevValue;
	float tempGradient, tempMagnitude;
	float** ptrs = (float**)malloc(NumOfBins * sizeof(float*)); // Cac bien con tro dung de tro toi cac bin images

	for(y = 0; y < in->height; y++)
	{		
		for(int i = 0; i < NumOfBins; i++){
			ptrs[i] = (float*)(bins[i]->imageData + y*(bins[i]->widthStep));
		}

		// Tinh cuong do va huong, dong thoi chia bin
		for( x = 0; x < in->width; x++)
		{
			xDevValue = ((float*)(xsobel->imageData + y*(xsobel->widthStep)))[x];			
			yDevValue = ((float*)(ysobel->imageData + y*(ysobel->widthStep)))[x];

			if(xDevValue == 0) // Luu y de tranh truong hop chia cho 0 khi tinh huong
			{					// ta cong them 1 dai luong rat nho vao mau
				tempGradient = ((atan(yDevValue/(xDevValue + 0.00001)))*(180/PI)) + 90;
			} 
			else 
			{
				tempGradient = ((atan(yDevValue/xDevValue))*(180/PI)) + 90;
			}

			tempMagnitude = sqrt((xDevValue*xDevValue)+ (yDevValue*yDevValue));			

			// Thuc hien chia bin cho moi pixel
			// @Note: code con cung cach chia bins
			if(tempGradient <= 20){
				ptrs[0][x] = tempMagnitude;
			} else if ( tempGradient <= 40){
				ptrs[1][x] = tempMagnitude;
			} else if ( tempGradient <= 60){
				ptrs[2][x] = tempMagnitude;
			} else if ( tempGradient <= 80) {
				ptrs[3][x] = tempMagnitude;
			} else if ( tempGradient <= 100) {
				ptrs[4][x] = tempMagnitude;
			} else if ( tempGradient <= 120) {
				ptrs[5][x] = tempMagnitude;
			} else if ( tempGradient <= 140) {
				ptrs[6][x] = tempMagnitude;
			} else if ( tempGradient <= 160) {
				ptrs[7][x] = tempMagnitude;
			} else if (tempGradient <= 180) {
				ptrs[8][x] = tempMagnitude;
			}
		}
	}

	cvReleaseImage(&xsobel);
	cvReleaseImage(&ysobel);

	// Tinh integral image tuong ung voi moi bin images
	for(int i = 0; i < NumOfBins; i++){
		cvIntegral(bins[i],integrals[i]);
	}

	for(int i = 0; i < NumOfBins; i++){
		cvReleaseImage(&bins[i]);
	}
	cvReleaseImage(bins);	

	free(ptrs);

	// Tra ve cac day anh integral
	return (integrals);
}

// Ham tinh dac trung HOG tai moi Cell
void HOGProcessor::calcHOGForCell(CvRect cell, CvMat* hogCell, IplImage** integrals, int normalization)
{
	// Lan luot tinh cuong do tich luy tai moi bin
	for(int i = 0; i < NumOfBins; i++){
		float a = ((double*)(integrals[i]->imageData + (cell.y)*(integrals[i]->widthStep)))[cell.x];		
					
		float b = ((double*)(integrals[i]->imageData + (cell.y + cell.height)*(integrals[i]->widthStep)))[cell.x + cell.width];
		
		float c = ((double*)(integrals[i]->imageData + (cell.y)*(integrals[i]->widthStep)))[cell.x + cell.width];
		
		float d = ((double*)(integrals[i]->imageData + (cell.y + cell.height)*(integrals[i]->widthStep)))[cell.x];
		
		((float*)hogCell->data.fl)[i] = (a+b)-(c+d);
	}

	// Chuan hoa vector dac trung
	if(normalization != -1){
		cvNormalize(hogCell, hogCell, 1, 0, normalization);
	}
}

// Ham tinh dac trung HOG tai moi block
void HOGProcessor::calcHOGForBlock(CvRect block, CvMat* hogBlock, IplImage** integrals,CvSize cell, int normalization)
{
	int cellStartX, cellStartY;
	CvMat vectorCell;
	int startcol = 0;
	for(cellStartY = block.y; cellStartY <= block.y + block.height - cell.height; cellStartY += cell.height)
	{
		for(cellStartX = block.x; cellStartX <=	block.x + block.width - cell.width;	cellStartX += cell.width)
		{
			cvGetCols(hogBlock, &vectorCell, startcol, startcol + NumOfBins);

			calcHOGForCell(cvRect(cellStartX, cellStartY, cell.width, cell.height), &vectorCell, integrals, -1);
			startcol += NumOfBins;
		}
	}

	if(normalization != -1){
		cvNormalize(hogBlock, hogBlock, 1, 0, normalization);
	}
}

// Ham tinh vector dac trung cho cua so HOG
CvMat* HOGProcessor::calcHOGWindow(IplImage** integrals, CvRect window, int normalization) 
{
	int blockStartX, blockStartY;	
	
	// Kich thuoc vector dac trung cho moi cell la 9
	// Kich thuoc vector dac trung cho moi block se duoc tinh bang:
	// 9 * (so cells trong moi block)
	// Vi du so cells/ 1 block = 4 => 9*4 = 36
	// => Kich thuoc vector dac trung cho moi cua so se la: 36 * (so block trong cua so)	
	int windowFeatureSize = getWindowFeatureVectorSize(cvSize(window.width, window.height));
	int blockFeatureSize = NumOfBins * m_block.height * m_block.width;

	CvMat* windowFeatureVector = cvCreateMat(1, windowFeatureSize, CV_32FC1);
	
	CvMat vectorBlock;
	int startcol = 0;
	
	// Khoi tao cac tham so phu cho viect tinh toan
	int widthBlockInPixels = m_cell.width * m_block.width;
	int heightBlockInPixels = m_cell.height * m_block.height;
	int heightStep = heightBlockInPixels - m_cell.height * m_fStepOverlap;
	int widthStep = widthBlockInPixels - m_cell.width * m_fStepOverlap;

	// Bat dau qua trinh tinh vector dac trung cho cua so
	for(blockStartY = window.y; 
		blockStartY <= window.y + window.height - heightBlockInPixels; 
		blockStartY += heightStep)
	{//overllap
			for(blockStartX = window.x; 
				blockStartX <= window.x + window.width - widthBlockInPixels;
				blockStartX += widthStep)
			{//overllap

			cvGetCols(windowFeatureVector, &vectorBlock, startcol, startcol + blockFeatureSize);

			calcHOGForBlock(
				cvRect(blockStartX, blockStartY, widthBlockInPixels , heightBlockInPixels)
				, &vectorBlock, integrals, m_cell
				, normalization);

			startcol += blockFeatureSize;
			}
	}

	return windowFeatureVector;
}

// @Note: Ham nay se duoc dieu chinh lai sau
// Under reconstruction....
CvMat* HOGProcessor::calcHOGWindow(IplImage *img, IplImage** integrals, CvRect window, int normalization)
{
	int blockStartX, blockStartY;
	int cellWidth = m_cell.width, cellHeight = m_cell.height;
	int blockWidth = m_block.width, blockHeight = m_block.height;

	// Kich thuoc vector dac trung cho moi cua so
	//			= blockFeatureSize * numOfBlocks
	int blockFeatureSize = NumOfBins * m_block.height * m_block.width;
	int num_overlap_block_of_width = ((window.width - cellWidth * blockWidth) / (cellWidth * m_fStepOverlap)) + 1;
	int num_overlap_block_of_height = ((window.height - cellHeight * blockHeight)/ (cellHeight * m_fStepOverlap)) + 1;
	int totalBlockOfWindow = num_overlap_block_of_width * num_overlap_block_of_height ;

	CvMat* windowFeatureVector = cvCreateMat(1, totalBlockOfWindow * blockFeatureSize, CV_32FC1);		  
	
	CvMat vector_block;
	int startcol = 0;
	
	cvNamedWindow("aaa");
	IplImage *imgHog = cvCreateImage(cvSize(9,9), img->depth, 1);
	IplImage *imgHogShow = cvCreateImage(cvSize(81,81), img->depth, 1);
	
	for(blockStartY = window.y; 
		blockStartY <= window.y + window.height - cellHeight * blockHeight; 
		blockStartY += cellHeight * m_fStepOverlap)
	{//overllap

			for(blockStartX = window.x; 
				blockStartX <= window.x + window.width - cellWidth * blockWidth;
				blockStartX += cellWidth * m_fStepOverlap)
			{//overllap
				
				cvGetCols(windowFeatureVector, &vector_block, startcol, startcol+36);

				calcHOGForBlock(cvRect(blockStartX,
					  blockStartY, cellWidth * blockWidth, cellHeight
					  * blockHeight), &vector_block, integrals, cvSize(
					  cellWidth, cellHeight), normalization);

				startcol += blockFeatureSize;

				int row=0;
				int col=0;
				for(row = 0; row < 9; row++)
				{
					for(col = 0; col < 9; col++)
					{
						float cell1 = CV_MAT_ELEM(vector_block, float, 0, (9 * row + col));
						CV_IMAGE_ELEM(imgHog, uchar, row, col) = cell1 * 255;					
					}
				}

				cvResize(imgHog, imgHogShow);
				cvShowImage("aaa", imgHogShow);

				cvWaitKey();			
			}
	}

	cvReleaseImage(&imgHog);
	cvReleaseImage(&imgHogShow);

	return windowFeatureVector;
}

void HOGProcessor::writeFeatureVector(CvMat* mat,char* className,ofstream &fout)
{
	CvSize posSize = cvGetSize(mat);
	int numberOfSamples = posSize.height;
	int featureVectorLength = posSize.width;

	char buffer[200];
	char buffer2[200];
	

	for(int  i = 0; i < numberOfSamples; i++){
		//fout<<className;
		for(int j = 0; j < featureVectorLength; j++)
		{			
			fout<<" ";
			float element = CV_MAT_ELEM( *mat, float, i, j);
			sprintf(buffer,"%d",j);
			fout<<buffer;
			fout<<":";

			sprintf(buffer2,"%f",element);
			fout<<buffer2;
		}

		fout<<"\n";
	}
}

CvMat* HOGProcessor::calcHOGFromImage(char* filename, CvSize window, int normalization)
{
	IplImage* im = cvLoadImage(filename);
	  
	//Convert Image To 64x128
	IplImage* dst = cvCreateImage(window,im->depth,im->nChannels);
	cvResize(im,dst);
	  
	// Tinh integral histogram
	IplImage** integrals;
	integrals = calculateIntegralHOG(dst);

	CvMat* imgFeatureVector;
	imgFeatureVector = calcHOGWindow(integrals, 
									cvRect(0, 0, window.width, window.height),
									normalization);

	for (int k = 0; k < NumOfBins; k++) 
		cvReleaseImage(&integrals[k]);

	return imgFeatureVector;
}