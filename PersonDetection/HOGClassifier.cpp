#include "StdAfx.h"
#include "HOGClassifier.h"


HOGClassifier::HOGClassifier(void)
{	
	CvSize cell = cvSize(8,8);
	CvSize block = cvSize(2, 2);
	hog.SetParams(cell, block, 1);
}

HOGClassifier::~HOGClassifier(void)
{
}

char* HOGClassifier::cvtInt( char *str, int num)
{
        sprintf( str, "%d", num );
		return str;
}

// Ham set cac tham so can thiet cho viec classify
void HOGClassifier::SetParams(CvSize cell, CvSize block, float stepOverLap)
{
	hog.SetParams(cell, block, stepOverLap);
}


// Ham tinh integral image
IplImage** HOGClassifier::calcIntegralHOG(IplImage* input)
{
	return hog.calculateIntegralHOG(input);
}

void HOGClassifier::loadTrainSVM(char* trainXMLPath)
{
	svmClassifier.load(trainXMLPath);
}

/// Ham Train cho tung cua so
/// Params:
// prefix: Duong dan toi thu muc train + "prefix cua file"
// suffix: suffix dinh dang cua file. Vi du: ".jpg"
// cell: kich thuoc cell: CvSize(8, 8)
// window: kich thuoc cua so: CvSize(64, 128)
// nSamples: so luong mau
// startInd: index cua file bat dau trong tap train
// endInd: index cua file ket thuc trong tap train
// saveXML: ten file xml dung de luu tru cac dac trung da duoc rut trich
// normalization: phuong phap chuan hoa dac trung, -1 => khong chuan hoa

CvMat* HOGClassifier::trainDataset_64x128(char *prefix, char *suffix, CvSize cell,
					CvSize window, int nSamples, int startInd,
					int endInd, char *saveXML, int canny,
					int block, int normalization) 
{
	char filename[50] = "\0", number[8];
	int lenPrefix;
	lenPrefix = strlen(prefix);
	int bins = 9;

	// Khoi tao ckich thuoc mac dinh cua cua so la 2 x 2
	int blockWidth = 2, blockHeight = 2;

	// Kich thuoc cua cua so la 64 x128
	int wFeatureVectorSize;
	//wFeatureVectorSize = (((window.width - cell.width * blockWidth)/ cell.width) + 1) * (((window.height - cell.height * blockHeight) / cell.height) + 1) * 36;
	wFeatureVectorSize = hog.getWindowFeatureVectorSize(window);

	// Ma tran dung de luu tru vector dac trung cua tat ca cac mau 
	CvMat* training = cvCreateMat(nSamples, wFeatureVectorSize, CV_32FC1);

	CvMat row;
	CvMat* imgFeatureVector;
	IplImage** integrals;
	int i = 0, j = 0;

	//printf("Bat dau rut trich dac trung HOG tu tap positive training\n");
	strcat(filename, prefix);

	// Bat dau tinh dac trung HOG cho tung anh
	for (i = startInd; i <= endInd; i++) 
	{	
		cvtInt(number, i);
		strcat(filename, number);
		strcat(filename, suffix);
		IplImage* img = cvLoadImage(filename);

		// Tinh integral histogram dung de tang toc do tinh toan dac trung HOG

		integrals = hog.calculateIntegralHOG(img);
		cvGetRow(training, &row, j);
		imgFeatureVector = hog.calcHOGWindow(integrals, cvRect(0, 0,
												window.width, window.height), normalization);
		cvCopy(imgFeatureVector, &row);
		//cvCopy(imgFeatureVector, &training[j]);
		
		j++;
		printf("%s\n", filename);
		filename[lenPrefix] = '\0';
		for (int k = 0; k < 9; k++) 
		{
			cvReleaseImage(&integrals[k]);
		}
	}
	if (saveXML != NULL) 
	{
		cvSave(saveXML, training);
	}

	return training;
}


// Ham train voi tap anh co kich thuoc lon hon 64x128
CvMat* HOGClassifier::trainLargeDataset(char *prefix, char *suffix,
					CvSize cell, CvSize window, int nImages,
					int horizontalScans, int verticalScans,
					int startInd, int endInd,
					char *saveXML, int normalization)
{
	char filename[50] = "\0", number[8];
	int lenPrefix;
	lenPrefix = strlen(prefix);
	int bins = 9;

	// Kich thuoc cua block la 2 x 2
	int blockWidth = 2, blockHeight = 2;

	// Tinh chieu dai vector dac trung cho cua so 64x128
	int wFeatureVectorSize;
	wFeatureVectorSize = (((window.width - cell.width * blockWidth) / cell.width) + 1) *
							(((window.height - cell.height * blockHeight) / cell.height) + 1) * 36;

	// Khoi tao ma tran dung de luu vector dac trung cho tat ca cac mau
	CvMat* training = cvCreateMat(nImages * horizontalScans * verticalScans,
								 wFeatureVectorSize, CV_32FC1);

	CvMat row;
	CvMat* imgFeatureVector;
	IplImage** integrals;
	int i = 0, j = 0;
	strcat(filename, prefix);

	printf("Beginning to extract HoG features from negative images\n");

	// Lap va tinh dac trung HOG cho tung anh

	for (i = startInd; i <= endInd; i++) 
	{
		cvtInt(number, i);
		strcat(filename, number);
		strcat(filename, suffix);
		IplImage* img = cvLoadImage(filename);
		integrals = hog.calculateIntegralHOG(img);
		for (int l = 0; l < verticalScans - 1; l++)
		{
			for (int k = 0; k < horizontalScans - 1; k++)
			{
				cvGetRow(training, &row, j);
				imgFeatureVector = hog.calcHOGWindow(
				integrals, cvRect(window.width * k,
				window.height * l, window.width,
				window.height), normalization);

				cvCopy(imgFeatureVector, &row);
				j++;
			}

			cvGetRow(training, &row, j);

			imgFeatureVector = hog.calcHOGWindow(integrals, cvRect(img->width - window.width,
														window.height * l, window.width, window.height), 
														normalization);

			cvCopy(imgFeatureVector, &row);
			j++;
		}

		for (int k = 0; k < horizontalScans - 1; k++)
		{
			cvGetRow(training, &row, j);

			imgFeatureVector = hog.calcHOGWindow(
			integrals, cvRect(window.width * k,
			img->height - window.height, window.width,
			window.height), normalization);

			cvCopy(imgFeatureVector, &row);
			j++;
		}
		cvGetRow(training, &row, j);

		imgFeatureVector = hog.calcHOGWindow(integrals,
												cvRect(img->width - window.width, 
												img->height - window.height,
												window.width, window.height),
												normalization);

		cvCopy(imgFeatureVector, &row);
		j++;

		printf("%s\n", filename);
		filename[lenPrefix] = '\0';
		for (int k = 0; k < 9; k++)
		{
			cvReleaseImage(&integrals[k]);
		}

		cvReleaseImage(&img);

	}

	printf("%d negative samples created \n", training->rows);

	if (saveXML != NULL)
	{
		cvSave(saveXML, training);
		printf("Negative samples saved as %s\n",
		saveXML);
	}

	return training;

}


/// Ham train su dung bo phan lop SVM tuyen tinh (linear SVM)
/// Params:
// posMat: 
//		Ma tran luu giu cac vector dac trung tuong ung voi tap Positive
// negMat:
//		Ma tran luu giu cac vector dac trung tuong ung voi tap Negative
// saveXML:
//		Ten ouput file cua qua trinh huan luyen svm
// posData: 
//		Ten file XML luu giu tap vector dac trung tuong ung voi tap du lieu Positive
// negData:
//		Ten file XML luu giu tap vector dac trung tuong ung voi tap du lieu Negative
void HOGClassifier::trainSVM(CvMat* posMat, CvMat* negMat, char *saveXML,
					char *posData, char *negData) 
{
	// Lay thong tin tap vector dac trung positive 
	if (posData != NULL) 
	{
		printf("Positive loading...\n");
		posMat = (CvMat*) cvLoad(posData);
		printf("Positive loaded\n");
	}

	// Lay thong tin tap vector dac trung negative
	if (negData != NULL)
	{
		negMat = (CvMat*) cvLoad(negData);
		printf("Negative loaded\n");
	}

	int nPos, nNeg;
	nPos = posMat->rows;
	nNeg = negMat->rows;
	int wFeatureVectorSize = posMat->cols;
	int nSamples;
	nSamples = nPos + nNeg;

	CvMat* trainData = cvCreateMat(nSamples, wFeatureVectorSize, CV_32FC1);

	CvMat* trainClasses = cvCreateMat(nSamples, 1, CV_32FC1 );

	CvMat trainData1, trainData2, trainClasses1, trainClasses2;

	printf("So luong mau Positive : %d\n", posMat->rows);

	// Copy tap vector Positive vao trainData
	cvGetRows(trainData, &trainData1, 0, nPos);
	cvCopy(posMat, &trainData1);
	cvReleaseMat(&posMat);

	// Copy tap vector Negative vao trainData
	cvGetRows(trainData, &trainData2, nPos, nSamples);

	cvCopy(negMat, &trainData2);
	cvReleaseMat(&negMat);

	printf("So luong mau Negative : %d\n", trainData2.rows);

	// Phan lop cho tap positive va negative
	// Positive : lop 1
	// Negative : lop 2
	cvGetRows(trainClasses, &trainClasses1, 0, nPos);
	cvSet(&trainClasses1, cvScalar(1));

	cvGetRows(trainClasses, &trainClasses2, nPos, nSamples);

	cvSet(&trainClasses2, cvScalar(2));


	// Bat dau qua trinh hoc SVM tu tap train
	CvSVM svm(trainData, trainClasses, 0, 0,
			CvSVMParams(CvSVM::C_SVC, CvSVM::LINEAR, 0, 0, 0, 2, 0, 0, 0, cvTermCriteria(CV_TERMCRIT_EPS,0, 0.01)));

	printf("Hoan tat qua trinh train SVM!!\n");

	// Luu lai mo hinh hoc
	if (saveXML != NULL) 
		svm.save(saveXML);

	//svmClassifier = svm;
	
	cvReleaseMat(&trainClasses);
	cvReleaseMat(&trainData);
}

float HOGClassifier::testSVM(char *trainXML, char *testData, char *saveData)
{
	//CvSVM svm = CvSVM ();
	//svm.load (trainXML);
	IplImage* img=cvLoadImage(testData);
	IplImage** integrals = hog.calculateIntegralHOG(img);
	CvMat* window_feature_vector = hog.calcHOGWindow(integrals, cvRect(0,0,64,128), CV_L2);
	float r=0; 
	r = svmClassifier.predict(window_feature_vector);	
	return r;
}

float HOGClassifier::testSVM(char *trainXML, IplImage *testImg, char *saveData)
{
	//CvSVM svm = CvSVM ();
	//svm.load (trainXML);
	IplImage** integrals = hog.calculateIntegralHOG(testImg);
	CvMat* window_feature_vector = hog.calcHOGWindow(integrals, cvRect(0,0,64,128), CV_L2); //Luu y: chua chinh tham so ve dang dynamic
	float r=0; 
	r = svmClassifier.predict(window_feature_vector);
	return r;
}

// Ham TestSVM su dung tap integrals HOG ung voi moi bins
float HOGClassifier::testSVM(char *trainXML, IplImage** integrals, char *saveData)
{
	//CvSVM svm = CvSVM ();
	//svmClassifier.load (trainXML);	
	CvMat* window_feature_vector = hog.calcHOGWindow(integrals, cvRect(0,0,64,128), CV_L2); //Luu y: chua chinh tham so ve dang dynamic
	float r=0; 
	r = svmClassifier.predict(window_feature_vector);
	return r;
}