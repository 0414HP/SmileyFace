#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include "detect_recog.h"
#include <opencv2\contrib\contrib.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <io.h>  
#include <direct.h> 
#include <sys/types.h>
#include <conio.h>
//#include "SmileyFaceDlg.h"
using namespace std;
using namespace cv;
/////////////////////////////////////////////
vector<Mat> images;
vector<int> labels;
const char * strConfidence = "70";
////////////////////////////////////////////



/*
void detect_and_draw( IplImage* img ) // ֻ�Ǽ�⣬��Ȧ������
{
    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
    IplImage *gray, *small_img;
    int i, j;
    gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)), 8, 1 );
    cvCvtColor( img, gray, CV_BGR2GRAY ); // ��ɫRGBͼ��תΪ�Ҷ�ͼ�� 
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img ); // ֱ��ͼ���⻯ 
    cvClearMemStorage( storage );
    if( cascade )
    {
        double t = (double)cvGetTickCount(); 
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, 0
                                            //|CV_HAAR_FIND_BIGGEST_OBJECT
                                            |CV_HAAR_DO_ROUGH_SEARCH
                                            //|CV_HAAR_DO_CANNY_PRUNING
                                            //|CV_HAAR_SCALE_IMAGE
                                            ,
                                            cvSize(30, 30) );
        t = (double)cvGetTickCount() - t; // ͳ�Ƽ��ʹ��ʱ�� 
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i ); // ��faces���ݴ�CvSeqתΪCvRect 
            CvMat small_img_roi;
            CvSeq* nested_objects;
            CvPoint center,recPt1,recPt2;
            CvScalar color = colors[i%8]; // ʹ�ò�ͬ��ɫ���Ƹ���face��������ɫ 
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale); // �ҳ�faces���� 
            center.y = cvRound((r->y + r->height*0.5)*scale);
			recPt1.x = cvRound((r->x)*scale);
			recPt1.y = cvRound((r->y)*scale);
			recPt2.x = cvRound((r->x + r->width)*scale);
			recPt2.y = cvRound((r->y + r->height)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale); 
				
			cvGetSubRect( small_img, &small_img_roi, *r );
			
			IplImage *result;
			CvRect roi;
			roi = *r;
			result = cvCreateImage( cvSize(r->width, r->height), img->depth, img->nChannels );
			cvSetImageROI(img,roi);
			// ������ͼ��
			cvCopy(img,result);
			cvResetImageROI(img);
			
			//IplImage *resizeRes;
			CvSize dst_cvsize;
			dst_cvsize.width=(int)(100);
			dst_cvsize.height=(int)(100);
			resizeRes=cvCreateImage(dst_cvsize,result->depth,result->nChannels);
			cvResize(result,resizeRes,CV_INTER_NN);
			faceGray = cvCreateImage(cvGetSize(resizeRes), IPL_DEPTH_8U, 1);//����Ŀ��ͼ��	
			cvCvtColor(resizeRes,faceGray,CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
            cvShowImage( "��ʾ����", resizeRes );
			cvRectangle(img,recPt1,recPt2,color,1, 8,0);
			//rectangle(img,recPt1,recPt2,color,1,8,0);
			//cvCircle( img, center, radius, color, 3, 8, 0 ); // ������λ�û�Բ��Ȧ����������
     
        }
    }
    //cvShowImage( "result", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}

*/
/*

//��Ⲣʶ������������ÿ֡ͼƬ��д����
void recog_and_draw2( IplImage* img )
{
    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
    IplImage *gray, *small_img;
    int i, j;
    gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)), 8, 1 );
    cvCvtColor( img, gray, CV_BGR2GRAY ); // ��ɫRGBͼ��תΪ�Ҷ�ͼ�� 
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img ); // ֱ��ͼ���⻯ 
    cvClearMemStorage( storage );
    if( cascade )
    {
        double t = (double)cvGetTickCount(); 
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, 0
                                            //|CV_HAAR_FIND_BIGGEST_OBJECT
                                            //|CV_HAAR_DO_ROUGH_SEARCH
                                            |CV_HAAR_DO_CANNY_PRUNING
                                            //|CV_HAAR_SCALE_IMAGE
                                            ,
                                            cvSize(30, 30) );
        t = (double)cvGetTickCount() - t; // ͳ�Ƽ��ʹ��ʱ�� 
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i ); // ��faces���ݴ�CvSeqתΪCvRect 
            CvMat small_img_roi;
            CvSeq* nested_objects;
            CvPoint center,recPt1,recPt2; //�����е㡢�Խǵ�����
            CvScalar color = colors[i%8]; // ʹ�ò�ͬ��ɫ���Ƹ���face��������ɫ 
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale); // �ҳ�faces���� 
            center.y = cvRound((r->y + r->height*0.5)*scale);
			recPt1.x = cvRound((r->x)*scale);
			recPt1.y = cvRound((r->y)*scale);
			recPt2.x = cvRound((r->x + r->width)*scale);
			recPt2.y = cvRound((r->y + r->height)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale); 
				
			cvGetSubRect( small_img, &small_img_roi, *r );
			
			IplImage *result;
			CvRect roi;
			roi = *r;
			result = cvCreateImage( cvSize(r->width, r->height), img->depth, img->nChannels );
			cvSetImageROI(img,roi);
			// ������ͼ��
			cvCopy(img,result);
			cvResetImageROI(img);
			
			//IplImage *resizeRes;
			CvSize dst_cvsize;
			dst_cvsize.width=(int)(100);
			dst_cvsize.height=(int)(100);
			resizeRes=cvCreateImage(dst_cvsize,result->depth,result->nChannels);
			cvResize(result,resizeRes,CV_INTER_NN);

			faceGray = cvCreateImage(cvGetSize(resizeRes), IPL_DEPTH_8U, 1);//����Ŀ��ͼ��	
			cvCvtColor(resizeRes,faceGray,CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
            cvShowImage( "��⵽������ͼ��", resizeRes );
			cvRectangle(img,recPt1,recPt2,color,3, 8,0);
			//cvCircle( img, center, radius, color, 3, 8, 0 ); // ������λ�û�Բ��Ȧ����������
			
			Mat test = faceGray;
			//images[images.size() - 1] = test;
			model->train(images, labels);
			//predictedLabel = model->predict(test);
			double predicted_confidence = 0.0;
			model->predict(test,predictedLabel,predicted_confidence);  //ʶ��������ѵ�����е����Ƴ̶ȡ�
			stringstream strStream;
			strStream<<predicted_confidence;
			string ss = strStream.str();   //��Ԥ��������Ƴ̶�ֵ��ת�����ַ���ss.
			cvText(img, ss.c_str(), r->x+r->width*0.5, r->y); //��ͼƬ��д���֡�
			if(predicted_confidence <= dConfidence)
				cvText(img, "Result:YES", 0, 30); 
			else
				cvText(img, "Result:NO", 0, 30);  
			//cout << "predict:"<<model->predict(test) << endl;
			//cout << "predict:"<< predictedLabel << "\nconfidence:" << predicted_confidence << endl;

			/*
            if( !nested_cascade )
                continue;
            
            nested_objects = cvHaarDetectObjects( &small_img_roi, nested_cascade, storage,
                                        1.1, 2, 0
                                        //|CV_HAAR_FIND_BIGGEST_OBJECT
                                        //|CV_HAAR_DO_ROUGH_SEARCH
                                        //|CV_HAAR_DO_CANNY_PRUNING
                                        //|CV_HAAR_SCALE_IMAGE
                                        ,
                                        cvSize(0, 0) );
            for( j = 0; j < (nested_objects ? nested_objects->total : 0); j++ )
            {
                CvRect* nr = (CvRect*)cvGetSeqElem( nested_objects, j );
                center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
                center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
                radius = cvRound((nr->width + nr->height)*0.25*scale);
                cvCircle( img, center, radius, color, 3, 8, 0 );
            }
			*/
/*
        }
    }
    //cvShowImage( "result", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}

8*/


bool recog_and_draw_new ( IplImage* img2,CascadeClassifier& face_cascade ,int& takeSample,int& sampleNum)
	                               //takeSample = 0ʱѵ������������1ʱ��ѵ������������2ʱѵ����������
{
	//if(takeSample){
		//����  �ɹ���������������������������������������
	//	AfxMessageBox(L"OK");
	//}
	images.clear();
	labels.clear();   //vector �Ǹ������������ڼ��ϡ�
	
	
//	model->set("threshold", dConfidence);
	//string output_folder;
	//output_folder = string("../einfacedata");

	//��ȡ���CSV�ļ�·��
	string fn_csv = string("../einfacedata/at.txt");

	//�������������ͼ�����ݺͶ�Ӧ�ı�ǩ

	try
	{
		read_csv(fn_csv, images, labels);	
	}
	catch(cv::Exception &e)
	{
		cerr<<"Error opening file "<<fn_csv<<". Reason: "<<e.msg<<endl;
		exit(1);
	}
	

/*	
	if(!read_img(images, labels))
	{
		AfxMessageBox(L"Error in reading images!");
		//MessageBox("Error in reading images!");
		images.clear();
		labels.clear();
		return false;
	}
*/
	//���û�ж����㹻��ͼƬ�����˳�
	if(images.size() < 1)
	{
	//	MessageBox("This demo needs at least 1 images to work!");
		AfxMessageBox(L"This demo needs at least 1 images to work!");
		return false;
	}
	//training
//	model->train(images, labels);

	if(takeSample==1){
	string faceImgName = "..//einfacedata//save.xml";
	model->load(faceImgName);
	}
	else if(takeSample==0)
	{
	model->train(images, labels);
	string faceImgName = "..//einfacedata//save.xml";
	model->save(faceImgName);
	takeSample = 1;
    }else if(takeSample==2){
	model->train(images, labels);
	string faceImgName = "..//einfacedata//save.xml";
	model->save(faceImgName);
	}


    Mat cam_frame = img2; // ��iplimageת��Ϊmat��
	//imshow( "cam_frame", cam_frame );
	std::vector<Rect> faces;
	Mat frame_gray,frame_face_gray;

	cvtColor( cam_frame, frame_gray, CV_BGR2GRAY ); //�Ҷȴ���
	equalizeHist( frame_gray, frame_gray );  // ֱ��ͼ���⻯ 
	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

	for( size_t i = 0; i < faces.size(); i++ )
	{
		Point center( int(faces[i].x + faces[i].width*0.5), int(faces[i].y + faces[i].height*0.5) );
     
			Point point1( int(faces[i].x), int(faces[i].y) );  //�ҵ������е�
			Point point2( int(faces[i].x + faces[i].width), int(faces[i].y + faces[i].height) );  //�ҵ������е�
		
			rectangle(cam_frame,point1,point2,Scalar( 255, 0, 255 ),3); //����
	
		//Mat test = faceGray;
			//images[images.size() - 1] = test;
	//		model->train(images, labels);
			//predictedLabel = model->predict(test);
			double predicted_confidence = 0.0;
			//ͼ���и�,���������
			IplImage *result;
			CvRect roi;
			roi=faces[i];
			result = cvCreateImage( cvSize(faces[i].width, faces[i].height), img2->depth, img2->nChannels );//����һ��������һ����С��ͼƬ
			cvSetImageROI(img2,roi);
			// ������ͼ��
			cvCopy(img2,result);
			cvResetImageROI(img2);//�ͷ�ROI����
			Mat mat_result = result;
			cvtColor( mat_result, frame_face_gray, CV_BGR2GRAY ); //�Ҷȴ���
////////////////////////////////////////////////////////////////////////////////////////
			if(takeSample==2){  //���ȡ�����򱣴�ͼƬ
	
	//Mat img(frame_face_gray,0);
	stringstream ss;
	ss << (read_img_number()+1);
	sampleNum = read_img_number()+1;
	string faceImgName = "..//einfacedata//trainingdata//"+ss.str()+".jpg";
	imwrite(faceImgName,frame_face_gray);

	//pTip->GetWindowText(strTip);
// 	CString tipPhoto = strTip + "\r\n���ճɹ����Ѵ�Ϊ" + ("/einfacedata/trainingdata/"+ss.str()+".jpg").c_str();
	//pTip->SetWindowText( tipPhoto );
	//MessageBox("OK");
			}
/////////////////////////////////////////////////////////////////////////
			model->predict(frame_face_gray,predictedLabel,predicted_confidence);  //ʶ��������ѵ�����е����Ƴ̶ȡ�
	
			if(predictedLabel==1){  //��ʶ��ʱ����ʾ
			stringstream strStream;
			strStream<<predicted_confidence;
			string ss ="Similarity:";
				ss+=strStream.str();   //��Ԥ��������Ƴ̶�ֵ��ת�����ַ���ss.
			cvText(img2, ss.c_str(),int(faces[i].x), int(faces[i].y)); //��ͼƬ��д���֡�
		//	if(predicted_confidence <= dConfidence)
				cvText(img2, "GUO", 0, 30); 
			AfxMessageBox(L"��ҫ��");
				//MessageBox(L"GUO");
		//	else
		//		cvText(img2, "Result:NO", 0, 30);  
			
			}else if(predictedLabel==2){  //��ʶ��ʱ����ʾ
			stringstream strStream;
			strStream<<predicted_confidence;
			string ss ="Similarity:";
				ss+=strStream.str();   //��Ԥ��������Ƴ̶�ֵ��ת�����ַ���ss.
			cvText(img2, ss.c_str(),int(faces[i].x), int(faces[i].y)); //��ͼƬ��д���֡�
		//	if(predicted_confidence <= dConfidence)
				cvText(img2, "HE", 0, 30); 
				AfxMessageBox(L"������2����");
		//	else
		//		cvText(img2, "Result:NO", 0, 30);  
			
			}else if(predictedLabel==3){  //��ʶ��ʱ����ʾ
			stringstream strStream;
			strStream<<predicted_confidence;
			string ss ="Similarity:";
				ss+=strStream.str();   //��Ԥ��������Ƴ̶�ֵ��ת�����ַ���ss.
			cvText(img2, ss.c_str(),int(faces[i].x), int(faces[i].y)); //��ͼƬ��д���֡�
		//	if(predicted_confidence <= dConfidence)
				cvText(img2, "SB", 0, 30); 
			AfxMessageBox(L"������3����");
		//	else
		//		cvText(img2, "Result:NO", 0, 30);  
			
			}
	} 
	
	////////////////////////////////////////////////////////////
/*    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
    IplImage *gray, *small_img;
    int i, j;
    gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)), 8, 1 );
    cvCvtColor( img, gray, CV_BGR2GRAY ); // ��ɫRGBͼ��תΪ�Ҷ�ͼ�� 
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img ); // ֱ��ͼ���⻯ 
    cvClearMemStorage( storage );
    if( cascade )
    {
        double t = (double)cvGetTickCount(); 
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, 0
                                            //|CV_HAAR_FIND_BIGGEST_OBJECT
                                            //|CV_HAAR_DO_ROUGH_SEARCH
                                            |CV_HAAR_DO_CANNY_PRUNING
                                            //|CV_HAAR_SCALE_IMAGE
                                            ,
                                            cvSize(30, 30) );
        t = (double)cvGetTickCount() - t; // ͳ�Ƽ��ʹ��ʱ�� 
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i ); // ��faces���ݴ�CvSeqתΪCvRect 
            CvMat small_img_roi;
            CvSeq* nested_objects;
            CvPoint center,recPt1,recPt2; //�����е㡢�Խǵ�����
            CvScalar color = colors[i%8]; // ʹ�ò�ͬ��ɫ���Ƹ���face��������ɫ 
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale); // �ҳ�faces���� 
            center.y = cvRound((r->y + r->height*0.5)*scale);
			recPt1.x = cvRound((r->x)*scale);
			recPt1.y = cvRound((r->y)*scale);
			recPt2.x = cvRound((r->x + r->width)*scale);
			recPt2.y = cvRound((r->y + r->height)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale); 
				
			cvGetSubRect( small_img, &small_img_roi, *r );
			
			IplImage *result;
			CvRect roi;
			roi = *r;
			result = cvCreateImage( cvSize(r->width, r->height), img->depth, img->nChannels );
			cvSetImageROI(img,roi);
			// ������ͼ��
			cvCopy(img,result);
			cvResetImageROI(img);
			
			//IplImage *resizeRes;
			CvSize dst_cvsize;
			dst_cvsize.width=(int)(100);
			dst_cvsize.height=(int)(100);
			resizeRes=cvCreateImage(dst_cvsize,result->depth,result->nChannels);
			cvResize(result,resizeRes,CV_INTER_NN);

			faceGray = cvCreateImage(cvGetSize(resizeRes), IPL_DEPTH_8U, 1);//����Ŀ��ͼ��	
			cvCvtColor(resizeRes,faceGray,CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
            cvShowImage( "��⵽������ͼ��", resizeRes );
			cvRectangle(img,recPt1,recPt2,color,3, 8,0);
			//cvCircle( img, center, radius, color, 3, 8, 0 ); // ������λ�û�Բ��Ȧ����������
			
			Mat test = faceGray;
			//images[images.size() - 1] = test;
			model->train(images, labels);
			//predictedLabel = model->predict(test);
			double predicted_confidence = 0.0;
			model->predict(test,predictedLabel,predicted_confidence);  //ʶ��������ѵ�����е����Ƴ̶ȡ�
			stringstream strStream;
			strStream<<predicted_confidence;
			string ss = strStream.str();   //��Ԥ��������Ƴ̶�ֵ��ת�����ַ���ss.
			cvText(img, ss.c_str(), r->x+r->width*0.5, r->y); //��ͼƬ��д���֡�
			if(predicted_confidence <= dConfidence)
				cvText(img, "Result:YES", 0, 30); 
			else
				cvText(img, "Result:NO", 0, 30);  
			//cout << "predict:"<<model->predict(test) << endl;
			//cout << "predict:"<< predictedLabel << "\nconfidence:" << predicted_confidence << endl;

			/*
            if( !nested_cascade )
                continue;
            
            nested_objects = cvHaarDetectObjects( &small_img_roi, nested_cascade, storage,
                                        1.1, 2, 0
                                        //|CV_HAAR_FIND_BIGGEST_OBJECT
                                        //|CV_HAAR_DO_ROUGH_SEARCH
                                        //|CV_HAAR_DO_CANNY_PRUNING
                                        //|CV_HAAR_SCALE_IMAGE
                                        ,
                                        cvSize(0, 0) );
            for( j = 0; j < (nested_objects ? nested_objects->total : 0); j++ )
            {
                CvRect* nr = (CvRect*)cvGetSeqElem( nested_objects, j );
                center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
                center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
                radius = cvRound((nr->width + nr->height)*0.25*scale);
                cvCircle( img, center, radius, color, 3, 8, 0 );
            }
			*/
/*
        }
    }
    //cvShowImage( "result", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
	
*/
return true;
}




/**
* This function is used to write words to the image.
*
*/
void cvText(IplImage* img, const char* text, int x, int y)  
{  
    CvFont font;  
    double hscale = 1.0;  
    double vscale = 1.0;  
    int linewidth = 2;  
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC,hscale,vscale,0,linewidth);  
    CvScalar textColor =cvScalar(0,255,255);  
    CvPoint textPos =cvPoint(x, y);  
    cvPutText(img, text, textPos, &font,textColor);  
}

Mat norm_0_255(cv::InputArray _src)
{
	Mat src = _src.getMat();
	Mat dst;

	switch(src.channels())
	{
	case 1:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}

	return dst;
}

void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator)
{
	std::ifstream file(filename.c_str(), ifstream::in);
	if(!file)
	{
		string error_message = "No valid input file was given.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line, path, classlabel;
	while(getline(file, line))
	{
		stringstream liness(line);
		getline(liness, path, separator);  //�����ֺžͽ���
		getline(liness, classlabel);     //�����ӷֺź��濪ʼ���������н���
		if(!path.empty() && !classlabel.empty())
		{
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}
//ʵ���˴�trainningdata Ŀ¼��ֱ�Ӷ�ȡjpg�ļ���Ϊѵ����
bool read_img(vector<Mat> &images, vector<int> &labels)
{
	long file;  
    struct _finddata_t find; 
	//AllocConsole();
	string path = "..//einfacedata//trainingdata/";
	char filepath[60];
    //_chdir("..//einfacedata//trainingdata");  
    if((file=_findfirst("..//einfacedata//trainingdata/*.jpg", &find))==-1L) 
	{  
		AfxMessageBox(L"Cannot find the dir");
        return false;  
    }  
	int i = 0;
    images.push_back(imread(path+find.name, 0));
	labels.push_back(0); 
    while(_findnext(file, &find)==0)  
    {  
		//_cprintf("%s\n", path+find.name);
		//_cprintf("%d\n", i++);
		images.push_back(imread(path+find.name, 0));
	
			labels.push_back(0); 
    }  
    _findclose(file);
	return true;
}
//ʵ���˴�trainningdata Ŀ¼�¶�ȡjpg�ļ���Ŀ
int read_img_number()
{
	long file;  
	int i = 0;
    struct _finddata_t find; 
	//AllocConsole();
	string path = "..//einfacedata//trainingdata/";
	char filepath[60];
    if((file=_findfirst("..//einfacedata//trainingdata/*.jpg", &find))==-1L) 
        return i;
	i++;
    while(_findnext(file, &find)==0)  
    {  
		i++;
    }  
    _findclose(file);
	return i;
}
bool delete_img()
{
	system( "del ..\\einfacedata\\trainingdata\\" ); 
	return true;
}
