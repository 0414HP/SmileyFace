
// SmileyFaceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SmileyFace.h"
#include "SmileyFaceDlg.h"
#include "afxdialogex.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
//#include <stdio.h>
#include<windows.h>
//#include "CimageAndMat.h"  
using namespace std;
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



 bool isFirst;
/////////////////////////////
//����ʶ�𲿷�
CvVideoWriter* writer = 0;
bool CloseThread = true;
bool doRecognize = false;//Ĭ��Ϊ��ʶ�𣬽����������
int takeSample = 0;//�����ж��Ƿ�ɼ������� 
                        //takeSample = 0ʱѵ������������1ʱ��ѵ������������2ʱѵ����������
IplImage* pFrame = NULL;  
Mat cam_frame;
CvCapture * pCapture;
//static CvCapture* pCapture;
CEdit* pEdtConfidence;
bool trainErro = false;
int sampleNum = -1;

//static bool takeSample = false;
//CvCapture* capture;
//CRect rect;
//CDC *pDC;
//HDC hDC;
//CWnd *pwnd;
//CvVideoWriter* writer = 0;
//IplImage *resizeRes;//��ż�⵽������
//IplImage* faceGray;//��ż�⵽������ �Ҷ�ͼ��
//bool bool_cameOpen = false;//ȫ�ֱ��� ��־����ͷ�Ƿ��
bool bool_picNum = false;//ȫ�ֱ��� ��־ѵ��ͼƬ�Ƿ�Ϊ��
bool bool_detec_reco = false;//ȫ�ֱ��� 
double dConfidence = 0;//���Ŷ�
int predictedLabel = 100000;

//CvMemStorage* storage = 0;
//CvHaarClassifierCascade* cascade = 0;
//CvHaarClassifierCascade* nested_cascade = 0;
//int use_nested_cascade = 0;
//const char* cascade_name =
 //   "../data/haarcascades/haarcascade_frontalface_alt.xml";
//const char* nested_cascade_name =
 //   "../data/haarcascade_eye_tree_eyeglasses.xml";
double scale = 1;
int num_components = 9;
double facethreshold = 9.0;
//cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();
cv::Ptr<cv::FaceRecognizer> model = cv::createLBPHFaceRecognizer();//LBP����������ڵ���������֤����Ч�����
//cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
//vector<Mat> images;
//vector<int> labels;

//IplImage *frame, *frame_copy = 0;
//IplImage *image = 0;
const char* scale_opt = "--scale="; // ������ѡ��ָʾ���� 
int scale_opt_len = (int)strlen(scale_opt);
const char* cascade_opt = "--cascade=";
int cascade_opt_len = (int)strlen(cascade_opt);
const char* nested_cascade_opt = "--nested-cascade";
int nested_cascade_opt_len = (int)strlen(nested_cascade_opt);
int i;
const char* input_name = 0;

// CAboutDlg dialog used for App About
CString strConfidence = L"70";
//CEdit* pEdtConfidence;
CString strTip = L"";
//CEdit* pTip;





//////////////////////////////



//��������
bool doShowFace = false;
CImage static image;  //����ȫ�ֱ�����������ͼƬ��
CImage static imageShow;  //����ȫ�ֱ�����������ʶ����ͼƬ��
CString path;//ͼƬ��ַȫ�ֱ���
CRect rect_face;//faceͼƬ��С
CRect rect_image;//imageͼƬ��С
CRect rect;//ͼƬ��ʾ�����С
CRect Facerect;//Face�����С
CDC *pDc;//pictrue�ؼ���Dc
CDC *pDc_face;//face�ؼ���Dc
CStatic* pStatic;//��������õ�
/** Function Headers */
void detectAndDisplay( Mat& frame,CDC *pDc);
void detectAndDisplay2( Mat& frame ,int type);
void ShowFace(CDC *pDc,Mat faceROI);//��ʾ��������
void ShowPath(CString path);
//void detect_and_draw(IplImage* image);
String face_cascade_name = "haarcascade_frontalface_alt.xml";
//String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
String eyes_cascade_name = "haarcascade_eye.xml";
String mouth_cascade_name = "haarcascade_mcs_mouth.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
CascadeClassifier mouth_cascade;
	/*MatToCImage
	*��飺
	*	OpenCV��MatתATL/MFC��CImage����֧�ֵ�ͨ���ҶȻ���ͨ����ɫ
	*������
	*	mat��OpenCV��Mat
	*	cimage��ATL/MFC��CImage
	*/
	void MatToCImage( Mat& mat, CImage& cimage);


	/*CImageToMat
	*��飺
	*	ATL/MFC��CImageתOpenCV��Mat����֧�ֵ�ͨ���ҶȻ���ͨ����ɫ
	*������
	*	cimage��ATL/MFC��CImage
	*	mat��OpenCV��Mat
	*/
	void CImageToMat(CImage& cimage, Mat& mat);





// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
/*
CSmileyFaceDlg::~CSmileyFaceDlg()

{

   AfxMessageBox(L"��������");  //�����Ի���

}
*/

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSmileyFaceDlg �Ի���




CSmileyFaceDlg::CSmileyFaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSmileyFaceDlg::IDD, pParent)
	, m_Result_Text(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmileyFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_IMAGEMAP, m_Picture);
	DDX_Control(pDX, IDC_FACE_IMAGE, m_Face_Image);
	//	DDX_Control(pDX, IDC_RESULT_TEXT, m_Result_Text);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox);
	DDX_Control(pDX, IDC_EDIT1, pEdtConfidence);
}

BEGIN_MESSAGE_MAP(CSmileyFaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_PIC_BUTTON, &CSmileyFaceDlg::OnBnClickedOpenPicButton)
	ON_BN_CLICKED(IDC_START_BUTTON, &CSmileyFaceDlg::OnBnClickedStartButton)
	//ON_BN_CLICKED(IDC_EXIT_BUTTON, &CSmileyFaceDlg::OnBnClickedExitButton)
//	ON_BN_CLICKED(IDC_BUTTON1, &CSmileyFaceDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CAM_BUTTON, &CSmileyFaceDlg::OnBnClickedCamButton)
	ON_BN_CLICKED(IDC_CHECK1, &CSmileyFaceDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_RECOGNIZE_BUTTON, &CSmileyFaceDlg::OnBnClickedRecognizeButton)
	ON_BN_CLICKED(IDC_SAMPLE_BUTTON, &CSmileyFaceDlg::OnBnClickedSampleButton)
	ON_BN_CLICKED(IDC_CLEAR_BUTTON, &CSmileyFaceDlg::OnBnClickedClearButton)
END_MESSAGE_MAP()


// CSmileyFaceDlg ��Ϣ�������

BOOL CSmileyFaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
//	pEdtConfidence = (CEdit*) GetDlgItem(IDC_EdtConfidence);
	pEdtConfidence.SetWindowText(L"70");
	pEdtConfidence.GetWindowText(strConfidence);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSmileyFaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSmileyFaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSmileyFaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*************��ͼƬ����**********************/


void CSmileyFaceDlg::OnBnClickedOpenPicButton()
{
	///////////////////dakaiwenjian
	CFileDialog dlg(TRUE,L"*.bmp", L"",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,L"image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",NULL);	
	dlg.m_ofn.lpstrTitle = L"��ѡ��������Ƭ";
	if (dlg.DoModal() == IDOK) 
	{
		path= dlg.GetPathName();
	    ShowPath(path);
		if (!image.IsNull()) {
		image.Destroy();
	};
	image.Load(path);
	int nWidth = image.GetWidth();
	int nHeight = image.GetHeight();
//	CRect rect;//���������
//	CRect rect1;
	m_Picture.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
    pDc = m_Picture.GetDC();//���pictrue�ؼ���Dc
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	if (nWidth <= rect.Width() && nHeight <= rect.Width()) //СͼƬ��������
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_image = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//���������ԭ��ͼƬ
		CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP);
        pStatic->SetBitmap(NULL);
	
		image.StretchBlt(pDc->m_hDC, rect_image, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������

	}
	else
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_image = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//���������ԭ��ͼƬ
		pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP);
        pStatic->SetBitmap(NULL);
	
		image.StretchBlt(pDc->m_hDC, rect_image, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������
	}
//	ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
	}
}
/******************�����������**************************************/

void CSmileyFaceDlg::OnBnClickedStartButton()
{
	if(!path.IsEmpty()){
	pDc_face = m_Face_Image.GetDC();//���face_pictrue�ؼ���Dc           //�˴���pdc�޸�ΪpDc_face��Ӧ�ж����Ǹ�ͼƬ��ʾ����
	m_Face_Image.GetClientRect(&Facerect); //���pictrue�ؼ����ڵľ�������
	//Mat frame;
    Mat frame;  //�趨Ҫʶ��ͼƬ��ַ���˴�ǰӦ���л�ȡͼƬ�ķ�����
//	const std::string &fileName ="F:\\6.jpg" ;
	//Mat frame = imread(fileName);  //�趨Ҫʶ��ͼƬ��ַ���˴�ǰӦ���л�ȡͼƬ�ķ�����
	CImageToMat(image, frame);//ͨ��Cimage������Mat
	//-- 1. Load the cascades
	bool isLoad1 = face_cascade.load( face_cascade_name );//��������������
	bool isLoad2 =eyes_cascade.load( eyes_cascade_name );//�����۾�������
	bool isLoad3= mouth_cascade.load( mouth_cascade_name );//�����첿������
//	if(isLoad1)
	if( !frame.empty() )
	{ detectAndDisplay(frame,pDc_face); }
	MatToCImage(frame, imageShow);
	imageShow.StretchBlt(pDc->m_hDC, rect_image, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ������� uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
	ReleaseDC(pDc_face);//�ͷ�picture�ؼ���Dc
	}else
	{
		AfxMessageBox(L"���ȴ�Ҫ����ͼƬ��");  //�����Ի���
	}
}

void detectAndDisplay( Mat& frame , CDC *pDc)
{
	int num_Mouths=0;
	int	num_Faces=0;
	int	num_Eyes=0;  //���ڴ������⵽����Ŀ
	std::vector<Rect> faces;
	std::vector<Rect> eyes;
	std::vector<Rect> mouth;
	Mat faceROI;
	Mat frame_gray;
	//�趨8����ɫ,zijia
	static CvScalar colors[8] = {
		{{0,0,255}},
		{{0,128,255}},
		{{0,255,255}},
		{{0,255,0}},
		{{255,128,0}},
		{{255,255,0}},
		{{255,0,0}},
		{{255,0,255}}
	};

	cvtColor( frame, frame_gray, CV_BGR2GRAY );//�Ҷȴ���
	imshow("ԭͼ", frame ); 
	imshow("�ҶȻ�����", frame_gray ); 
	equalizeHist( frame_gray, frame_gray );//ֱ��ͼ���⻯
	imshow("ֱ��ͼ", frame_gray ); 
	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
//	cout<<faces.size()<<endl;
	//�õ�����ͼ�񣬽��л�Ȧ
	for( size_t i = 0; i < (faces.size()?faces.size() : 0); i++ )
	{

		faceROI = frame_gray( faces[i] );//�õ�����ͼƬ
		//����������۾������û���۾�����������
	//	cout<<faceROI.size()<<endl;
	
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) ); //����۾�
		if(eyes.size()==0){                                                   //����۾�û��⵽��������
			mouth_cascade.detectMultiScale( faceROI, mouth, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) ); //���mouth
	//		cout<<"�����"<<mouth.size()<<endl;
		for( int j = 0; j < (mouth.size()?mouth.size() : 0); j++ )
			{
				int m_int_x = int(faces[i].x + mouth[j].x + mouth[j].width*0.5);
				int m_int_y = int(faces[i].y + mouth[j].y + mouth[j].height*0.5);
				Point center = Point(m_int_x , m_int_y); 								
				int radius = cvRound( mouth[j].width*0.25 );		
	//			cout<<"mouth��������="<<mouth[j].y<<"  ���ĸ߶�"<<faces[i].height*0.5<<endl;
				if(mouth[j].y>faces[i].height*0.5){
				circle( frame, center, radius, colors[(i+1) % 8], 1, 3, 0 );  //����
				num_Mouths++;//������+1
				}
				
		
		    }
		}else{

		try
		{
		//	cout<<"�۾�����"<<eyes.size()<<endl;
			for( int j = 0; j < (eyes.size()?eyes.size() : 0); j++ )
			{
				int int_x = int(faces[i].x + eyes[j].x + eyes[j].width*0.5);
				int int_y = int(faces[i].y + eyes[j].y + eyes[j].height*0.5);
				Point center = Point(int_x , int_y); 
				int radius = cvRound( eyes[j].width*0.25 );
			//	cout<<"�۾���y="<<eyes[j].y<<"�۾���x="<<eyes[j].x<<"  ���ĸ߶�"<<faces[i].height*0.5<<endl;
				if(eyes[j].y<faces[i].height*0.5&&eyes[j].y>faces[i].height*0.2){
				circle( frame, center, radius, colors[(i+3) % 8], 3, 2, 0 );  //���۾�
				num_Eyes++;//����+1
				}
			//	Sleep(500);
			}
		}
		catch( cv::Exception& e )
		{
			const char* err_msg = e.what();
		//	printf ("%s\n", err_msg);
			
		}
		}
	        if(eyes.size()!=0||mouth.size()!=0||faces[i].height<80){
			Point center2( int(faces[i].x + faces[i].width*0.5), int(faces[i].y + faces[i].height*0.5) );  //�ҵ������е�
			Point point1( int(faces[i].x), int(faces[i].y) );  //�ҵ������е�
			Point point2( int(faces[i].x + faces[i].width), int(faces[i].y + faces[i].height) );  //�ҵ������е�
		//	ellipse( frame, center2, Size( int(faces[i].width*0.5), int(faces[i].height*0.5)), 0, 0, 360,colors[i % 8], 2, 8, 0 );  //����Ȧ
			rectangle(frame,point1,point2,colors[i % 8],3); //����
			    num_Faces++;//������+1
				CString str1,str2,str3,str4,str5;
                str1.Format(_T("%d"), num_Faces);
				str2.Format(_T("%d"), num_Eyes);
				str3.Format(_T("%d"), num_Mouths);
				str4="����⵽��\n";
				str4+=str1;str4+=" ����\n";
				str4+=str2;str4+=" ֻ��\n";
				str4+=str3;str4+=" ����";
				SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_RESULT_TEXT,str4);	
				ShowFace(pDc,faceROI);  //��ʾ��aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
				if(doShowFace){
				str5="face";str5+=str1;
		        CStringW strw =str5;
                CStringA stra(strw.GetBuffer(0));
                strw.ReleaseBuffer();
                std::string facenum=stra.GetBuffer(0);
                stra.ReleaseBuffer();
		
			    imshow(facenum, faceROI );  
				/////////////////////////////////////////////////////////////////////����
			//	std::string tmp = (LPSTR)(LPCTSTR)str5;
            //    const std::string &sFileName = tmp;
			//	imshow(sFileName, faceROI ); //����
				/////////////////////////////////////////////////////////////////
				}
 	}
		///////////////////////////////////////////////////////////////////////////////////////
		//����۾�
		/*       std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

		for( size_t j = 0; j < eyes.size(); j++ )
		{
		Point center( int(faces[i].x + eyes[j].x + eyes[j].width*0.5), int(faces[i].y + eyes[j].y + eyes[j].height*0.5) ); 
		int radius = cvRound( (eyes[j].width + eyes[i].height)*0.25 );
		circle( frame, center, radius, Scalar( 255, 0, 0 ), 3, 8, 0 );
		}
		*/
	} 
/*	//-- Show what you got
	//imshow( "22222", frame );
	//��ʾ��picture control��
	///////////////////dakaiwenjian
    MatToCImage(frame, imageShow);
	int nWidth = imageShow.GetWidth();
	int nHeight = imageShow.GetHeight();
//	CRect rect;//���������
//	CRect rect1;
//	m_Picture.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
//	CDC *pDc = m_Picture.GetDC();//���pictrue�ؼ���Dc
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	if (nWidth <= rect.Width() && nHeight <= rect.Width()) //СͼƬ��������
	{
		rect1 = CRect(rect.TopLeft(), CSize(nWidth, nHeight));
		imageShow.StretchBlt(pDc->m_hDC, rect1, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������

	}
	else
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect1 = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//���������ԭ��ͼƬ
	//	CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP);
    //    pStatic->SetBitmap(NULL);
	
		imageShow.StretchBlt(pDc->m_hDC, rect1, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������
	}
*/
	
	CString str1,str2,str3,str4,str5;
                str1.Format(_T("%d"), num_Faces);
				str2.Format(_T("%d"), num_Eyes);
				str3.Format(_T("%d"), num_Mouths);
				str4="����⵽��\n";
				str4+=str1;str4+=" ����\n";
				str4+=str2;str4+=" ֻ��\n";
				str4+=str3;str4+=" ����";
				SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_RESULT_TEXT,str4);	
//	imshow( "���������", frame );                //      hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
	
	}




void ShowFace(CDC *pDc,Mat faceROI)
{   CImage faceImage;
	MatToCImage(faceROI, faceImage);
	int nWidth = faceImage.GetWidth();
	int nHeight = faceImage.GetHeight();
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	if (nWidth <= Facerect.Width() && nHeight <= Facerect.Width()) //СͼƬ��������
	{
		float xScale = (float)Facerect.Width() / (float)nWidth;
		float yScale = (float)Facerect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((Facerect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((Facerect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_face = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//���������ԭ��ͼƬ
	//	CStatic* pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,IDC_FACE_IMAGE);
    //    pStatic->SetBitmap(NULL);
	
		faceImage.StretchBlt(pDc->m_hDC, Facerect, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������

	}
	else
	{
		float xScale = (float)Facerect.Width() / (float)nWidth;
		float yScale = (float)Facerect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((Facerect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((Facerect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_face = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//���������ԭ��ͼƬ
	//	CStatic* pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,IDC_FACE_IMAGE);
  //      pStatic->SetBitmap(NULL);
	
//		faceImage.StretchBlt(pDc->m_hDC, Facerect, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������
		faceImage.StretchBlt(pDc->m_hDC, rect_face, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������
	}
//	ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
}








void MatToCImage(Mat& mat, CImage& cimage)
{
	if (0 == mat.total())
	{
		return;
	}


	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth    = mat.cols;
	int nHeight   = mat.rows;


	//�ؽ�cimage
	cimage.Destroy();
	cimage.Create(nWidth, nHeight, 8 * nChannels);


	//��������


	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)cimage.GetBits();		//ָ����������ָ��
	int nStep = cimage.GetPitch();					//ÿ�е��ֽ���,ע���������ֵ�����и�


	if (1 == nChannels)								//���ڵ�ͨ����ͼ����Ҫ��ʼ����ɫ��
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cimage.GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cimage.SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete []rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0 ; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}			
			}
		}	
	}
}

void CImageToMat(CImage& cimage, Mat& mat)
{
	if (true == cimage.IsNull())
	{
		return;
	}


	int nChannels = cimage.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth    = cimage.GetWidth();
	int nHeight   = cimage.GetHeight();


	//�ؽ�mat
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if(3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}


	//��������


	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)cimage.GetBits();		//ָ����������ָ��
	int nStep = cimage.GetPitch();					//ÿ�е��ֽ���,ע���������ֵ�����и�


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0 ; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}			
			}
		}	
	}
}

//����


void CSmileyFaceDlg::OnBnClickedExitButton()
{
	
}


void CSmileyFaceDlg::OnBnClickedButton1()
{ 
	Mat frame;

	//-- 1. Load the cascades
	if(face_cascade.empty())
	face_cascade.load( face_cascade_name );

	///////////////////dakaiwenjian
	CFileDialog dlg(TRUE,L"*.avi", L"",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,L"AVI files (*.avi) |*.avi;*.mp4;*.3gp;*.rmvb|All Files (*.*)|*.*||",NULL);	
	dlg.m_ofn.lpstrTitle = L"��ѡ��������Ƶ�ļ�";
	if (dlg.DoModal() == IDOK) 
	{
		path= dlg.GetPathName();
		ShowPath(path);
	/*	  CStringW strw =path;
    CStringA stra(strw.GetBuffer(0));
    strw.ReleaseBuffer();
    std::string imgpath=stra.GetBuffer(0);
    stra.ReleaseBuffer();

	*/
		////////////////////////////////////////////////////////////////
		//����
		std::string tmp = (LPSTR)(LPCTSTR)path;
        const std::string &sFileName = tmp;




		///////////////////////////////////////////////////
//	VideoCapture capture("Simple.avi");
    VideoCapture capture(sFileName);///////////////////////////ceshi
//	VideoCapture capture(path);
	if( capture.isOpened() )	// ����ͷ��ȡ�ļ�����
	{
		while( true )
		{
 		//	frame = cvQueryFrame( capture );	// ����ͷ��ȡ�ļ�����
			capture >> frame;

			//-- 3. Apply the classifier to the frame
			if( !frame.empty() )
			{ detectAndDisplay2( frame,1 ); }
			else
			{ printf(" --(!) No captured frame -- Break!"); break; }

			int c = waitKey(10);
			if( (char)c == 'c' ) {  break; } 

		}
	}
	}
	cvDestroyWindow("��Ƶ�ļ��������");
}

/***********��������ͷ******************/

//�˴��������޸�
void CSmileyFaceDlg::OnBnClickedCamButton()
{
///**********************************************************************************************8

	takeSample = false;
	if(CloseThread){
	m_Picture.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
    pDc = m_Picture.GetDC();//���pictrue�ؼ���Dc
	pDc_face = m_Face_Image.GetDC();//���pictrue�ؼ���Dc
	m_Face_Image.GetClientRect(&Facerect); //���pictrue�ؼ����ڵľ�������
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	SetStretchBltMode(pDc_face->m_hDC, STRETCH_HALFTONE);
	//�˴�Ӧ���ж�pdc�Ƿ�Ϊ�գ�����������Ӧ�����߳̿��ƻ��ƣ�boole �������߳̽������ı䰴ť���֡�
	CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP);	
    pStatic->SetBitmap(NULL);
	pStatic = (CStatic*)GetDlgItem(IDC_FACE_IMAGE);	
    pStatic->SetBitmap(NULL);
	CloseThread = false; //���ڸı��߳�״̬��trueʱ�̹߳رա�
    CString str1;
    str1 = "�ر�����ͷ";
	SetDlgItemText(IDC_CAM_BUTTON,str1);
	//�˴�Ӧ�ý�ʶ�𡢲ɼ��زġ�������ť��Ϊ�ɿ�
    ::AfxBeginThread(ThreadFun,NULL);  //�����߳�
	GetDlgItem(IDC_RECOGNIZE_BUTTON)->EnableWindow(true);
	GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(true);
    str1="����ͷ�ѿ���";
	SetDlgItemText(IDC_STATE_TEXT,str1);
	}else
	{
	GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(false);
	GetDlgItem(IDC_RECOGNIZE_BUTTON)->EnableWindow(false);
	CString str1;
    str1 = "��������ͷ";
	SetDlgItemText(IDC_CAM_BUTTON,str1);
	CloseThread = true; //���ڸı��߳�״̬��trueʱ�̹߳رա�
	//�˴�Ӧ�ý�ʶ�𡢲ɼ��زġ�������ť��Ϊ���ɿ�
	cvReleaseCapture(&pCapture);
	str1="����ͷ�ѹر�";
	SetDlgItemText(IDC_STATE_TEXT,str1);
//	isFirst= true;
	cam_frame.release();
	}





/////////////////////////////////////////****************************************************************
	/*
	if(face_cascade.empty())
	face_cascade.load( face_cascade_name );

		//����IplImageָ��  
 // IplImage* pFrame = NULL;  
  Mat cam_frame;
 //��ȡ����ͷ  
  CvCapture* pCapture = cvCreateCameraCapture(0);  
   
  //��������  
 // cvNamedWindow("video", 1);  
   
  //��ʾ����  
  while(1)  
  {  cam_frame=cvQueryFrame( pCapture );  
    //  pFrame=cvQueryFrame( pCapture );
  Sleep(3000);
      if(cam_frame.empty())break;  
     // cvShowImage("video",pFrame);
	  detectAndDisplay2( cam_frame,2 );
      
	  char c=cvWaitKey(33);  
      if(c==27)break;  
  }  
  cvReleaseCapture(&pCapture);  
  cvDestroyWindow("����ͷʵʱ���");
  */
	/*****�Ľ��㷨*******/
///////////////////////////////////////////////////////////////////
	//���ԣ����´�����ע�͵�
	////////////////////////////////////////////////////////
	/*
if(face_cascade.empty())
	face_cascade.load( face_cascade_name );

		//����IplImageָ��  
 // IplImage* pFrame = NULL;  
  Mat cam_frame;
   CvCapture* pCapture;
   bool isFirst = true;
  while(1)  
  { 
	
while(cam_frame.empty()&&isFirst){
 //��ȡ����ͷ  
 pCapture = cvCreateCameraCapture(0);  
  //��ʾ����  
   cam_frame=cvQueryFrame( pCapture );
   isFirst= false;
}
         cam_frame=cvQueryFrame( pCapture );
    //  pFrame=cvQueryFrame( pCapture );
    //  Sleep(3000);
  //    if(cam_frame.empty()) 
     // cvShowImage("video",pFrame);
	  detectAndDisplay2( cam_frame,2 );  //cam_frame Ϊ�����á�
      
	  char c=cvWaitKey(33);  
      if(c==27)break;  
  }  
  cvReleaseCapture(&pCapture);
  */
  ///////////////////////////////////////////////////////////////////
	//���ԣ����´�����ע�͵�
	////////////////////////////////////////////////////////

//  cvDestroyWindow("����ͷʵʱ���");

/*****�Ľ��㷨*******/
	/*	Mat cam_frame;
	//-- 1. Load the cascades
//	face_cascade.load( face_cascade_name );
	//eyes_cascade.load( eyes_cascade_name ); 
	//-- 2. Read the video stream
 	CvCapture* capture = cvCaptureFromCAM( 0 );	// ����ͷ��ȡ�ļ�����
//	VideoCapture capture("Sample.avi");
//	if( capture.isOpened() )	// ����ͷ��ȡ�ļ�����
//	{
		while( true )
		{
 			cam_frame = cvQueryFrame( capture );	// ����ͷ��ȡ�ļ�����
		//	capture >> frame;

			//-- 3. Apply the classifier to the frame
			if( !cam_frame.empty() )
			{// detectAndDisplay2( cam_frame ); 
	Sleep(2000);
				imshow( "1111", cam_frame );
			}

		}
		8*/
}


/********************ʵʱ������⺯��********************************/
//�����󷵻�Mat����
void detectAndDisplay2( Mat &cam_frame ,int type )
{
	std::vector<Rect> faces;
	Mat frame_gray;
	Mat faceROI;//������ͼ
	cvtColor( cam_frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );
	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

	for( size_t i = 0; i < faces.size(); i++ )
	{
		Point center( int(faces[i].x + faces[i].width*0.5), int(faces[i].y + faces[i].height*0.5) );
	//ellipse( cam_frame, center, Size( int(faces[i].width*0.5), int(faces[i].height*0.5)), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
     
			Point point1( int(faces[i].x), int(faces[i].y) );  //�ҵ������е�
			Point point2( int(faces[i].x + faces[i].width), int(faces[i].y + faces[i].height) );  //�ҵ������е�
		
			rectangle(cam_frame,point1,point2,Scalar( 255, 0, 255 ),3); //����
	        faceROI = frame_gray( faces[i] );

			ShowFace(pDc_face,faceROI);
			if(takeSample==2){  //���ȡ�����򱣴�ͼƬ
	
	//Mat img(frame_face_gray,0);
	stringstream ss;
	ss << (read_img_number()+1);
	sampleNum = read_img_number()+1;
	string faceImgName = "..//einfacedata//trainingdata//"+ss.str()+".jpg";
	imwrite(faceImgName,faceROI);
	takeSample = 0;
	//pTip->GetWindowText(strTip);
// 	CString tipPhoto = strTip + "\r\n���ճɹ����Ѵ�Ϊ" + ("/einfacedata/trainingdata/"+ss.str()+".jpg").c_str();
	//pTip->SetWindowText( tipPhoto );
	//MessageBox("OK");
			}

			
			/*	Mat faceROI = frame_gray( faces[i] );
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

		for( size_t j = 0; j < eyes.size(); j++ )
		{
			Point center( int(faces[i].x + eyes[j].x + eyes[j].width*0.5), int(faces[i].y + eyes[j].y + eyes[j].height*0.5) ); 
			int radius = cvRound( (eyes[j].width + eyes[i].height)*0.25 );
			circle( frame, center, radius, Scalar( 255, 0, 0 ), 3, 8, 0 );
		}
		*/
	} 


	//-- Show what you got
//	if(2==type)
//	imshow( "����ͷʵʱ���", cam_frame );
//	else
//	imshow( "��Ƶ�ļ��������", cam_frame );

}

void CSmileyFaceDlg::OnBnClickedCheck1()
{
	if(BST_UNCHECKED==m_CheckBox.GetCheck())
		doShowFace = false;
	else
		doShowFace = true;
}

void ShowPath(CString path){
//CString str1,str2,str3,str4,str5;
 //               str1.Format(_T("%d"), num_Faces);
	//			str2.Format(_T("%d"), num_Eyes);
	//			str3.Format(_T("%d"), num_Mouths);
	//			str4="����⵽��\n\n";
	//			str4+=str1;str4+=" ����\n\n";
		//		str4+=str2;str4+=" ֻ��\n\n";
		//		str4+=str3;str4+=" ����";
	CString str1;
str1 = "��ǰ�ļ�·����";
	str1+=path;
				SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_STATE_TEXT,str1);	

}


///////////////////////////////////////////////////////////////////////////











//Ӧ�ò��ö��̡߳�

//����ʶ����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void CSmileyFaceDlg::OnBnClickedRecognizeButton()
{                                            //do false   train false    ������ʶ��״̬
	                                         //do true    train false    ����ʶ��״̬
	                                         //do false   train ture     ѵ��ʧ��״̬
	if(trainErro){
    trainErro=false;
	doRecognize = false;
    CString str1;
    str1 = "��ʼʶ��";
	SetDlgItemText(IDC_RECOGNIZE_BUTTON,str1);
	GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(true);
	}else
	if(!doRecognize){
		GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(false);
	pEdtConfidence.GetWindowText(strConfidence);
	try
	{
//		CString str=_T("�����ҵĲ��Գ���");
// �ȵõ�Ҫת��Ϊ�ַ��ĳ���
    const size_t strsize=(strConfidence.GetLength()+1)*2; // ���ַ��ĳ���;
    char * pstr= new char[strsize]; //����ռ�;
    size_t sz=0;
    wcstombs_s(&sz,pstr,strsize,strConfidence,_TRUNCATE);

	dConfidence = atoi((const char*)pstr);	
		
	}
	catch(cv::Exception &e)
	{
		MessageBox(L"���Ŷ�������������");
		return;
	}
	
	model->set("threshold", dConfidence);
	doRecognize = true;
	CString str1;
    str1 = "ֹͣʶ��";
	SetDlgItemText(IDC_RECOGNIZE_BUTTON,str1);
	GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(false);
	}else{
		GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(true);
	doRecognize = false;
    CString str1;
    str1 = "��ʼʶ��";
	SetDlgItemText(IDC_RECOGNIZE_BUTTON,str1);
	}
	/*
	if(CloseThread){
	m_Picture.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
    pDc = m_Picture.GetDC();//���pictrue�ؼ���Dc
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	//�˴�Ӧ���ж�pdc�Ƿ�Ϊ�գ�����������Ӧ�����߳̿��ƻ��ƣ�boole �������߳̽������ı䰴ť���֡�
	CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP);	
    pStatic->SetBitmap(NULL);
	CloseThread = false; //���ڸı��߳�״̬��trueʱ�̹߳رա�
    CString str1;
    str1 = "ֹͣ���";
	SetDlgItemText(IDC_RECOGNIZE_BUTTON,str1);
    ::AfxBeginThread(ThreadFun,NULL);  
	}else
	{
	CString str1;
    str1 = "�沿ʶ��";
	SetDlgItemText(IDC_RECOGNIZE_BUTTON,str1);
	CloseThread = true; //���ڸı��߳�״̬��trueʱ�̹߳رա�
	}
	*/


}


UINT WINAPI CSmileyFaceDlg::ThreadFun(LPVOID lpParam){


	if(face_cascade.empty())
	face_cascade.load( face_cascade_name );
    //����IplImageָ��  
   pFrame = NULL;  
 //  Mat cam_frame;
 //  CvCapture* pCapture;
   isFirst = true;
   while(1)  
 { 	
  while(cam_frame.empty()&&isFirst){
 //��ȡ����ͷ  
   pCapture = cvCreateCameraCapture(0);  
  //��ʾ����  
   cam_frame=cvQueryFrame( pCapture );
   isFirst= false;
  }
  if(doRecognize){   //�ж��Ƿ�ʶ������
      pFrame=cvQueryFrame( pCapture );
      
      if(!recog_and_draw_new(pFrame,face_cascade,takeSample,sampleNum)){//����ʶ������	
	  //���ʶ������ļ���ȡ������doRecognize����Ϊfalse
	  doRecognize = false;
	  trainErro = true;
    //  CString str1;
   //   str1 = "��ʼʶ��";
	//  SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_RECOGNIZE_BUTTON,str1);
	//  SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_RECOGNIZE_BUTTON,str1);
	  }  
	  
	  cam_frame = pFrame;
	  //�Ľ�������������ͷ����ͼ����ʾ��picture�ؼ���	
	  MatToCImage(cam_frame, imageShow);//��ʶ��õ�matת��ΪCImage
	  int nWidth = imageShow.GetWidth();
	  int nHeight = imageShow.GetHeight();
   	CRect rect_CamRecognize;//��������� ���ڱ�������ͷͼƬ��С

	if (nWidth <= rect.Width() && nHeight <= rect.Width()) //СͼƬ��������
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_CamRecognize = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//���������ԭ��ͼƬ
	//	CStatic* pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,ID_IMAGEMAP);
	//	CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP,lpParam);
   //     pStatic->SetBitmap(NULL);
	
		imageShow.StretchBlt(pDc->m_hDC, rect_CamRecognize, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������

	}
	else
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_CamRecognize = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//���������ԭ��ͼƬ
	//	pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,ID_IMAGEMAP);
     //   pStatic->SetBitmap(NULL);
	
		imageShow.StretchBlt(pDc->m_hDC, rect_CamRecognize, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������
	}
	  }
	  else{    //�����������
	 // Mat findFaceResult = pFrame;
		  cam_frame=cvQueryFrame( pCapture );
      detectAndDisplay2( cam_frame ,1);//�ڶ�������û��  
	   //�Ľ�������������ͷ����ͼ����ʾ��picture�ؼ���	
	  MatToCImage(cam_frame, imageShow);//��ʶ��õ�matת��ΪCImage
	  int nWidth = imageShow.GetWidth();
	  int nHeight = imageShow.GetHeight();
   	CRect rect_CamRecognize;//��������� ���ڱ�������ͷͼƬ��С

	if (nWidth <= rect.Width() && nHeight <= rect.Width()) //СͼƬ��������
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_CamRecognize = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//���������ԭ��ͼƬ
	//	CStatic* pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,ID_IMAGEMAP);
	//	CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP,lpParam);
   //     pStatic->SetBitmap(NULL);
	
		imageShow.StretchBlt(pDc->m_hDC, rect_CamRecognize, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������

	}
	else
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_CamRecognize = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//���������ԭ��ͼƬ
	//	pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,ID_IMAGEMAP);
     //   pStatic->SetBitmap(NULL);
	
		imageShow.StretchBlt(pDc->m_hDC, rect_CamRecognize, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������



	  }
}
	if(CloseThread)
		break;
  

  

}

 return 0;

}
/**
*��������
*/
void CSmileyFaceDlg::OnBnClickedSampleButton()
{	/*
	if(takeSample==1||takeSample==0){
	takeSample = 2;
	CString str1;
    str1 = "ֹͣ����";
	SetDlgItemText(IDC_SAMPLE_BUTTON,str1);
	}else
	{
	takeSample = 0;
	CString str1;
    str1 = "�ɼ�����";
	SetDlgItemText(IDC_SAMPLE_BUTTON,str1);	
	if(sampleNum!=-1){
	    CString str1,str2;
	    str1="ѵ����������������";
		str2.Format(_T("%d"), sampleNum);
	    str1 += str2;
	    SetDlgItemText(IDC_STATE_TEXT,str1);
	  }
	}
	*/
	takeSample = 2;
	if(sampleNum!=-1){
	    CString str1,str2;
	    str1="ѵ����������������";
		str2.Format(_T("%d"), sampleNum);
	    str1 += str2;
	    SetDlgItemText(IDC_STATE_TEXT,str1);
	  }
}


void CSmileyFaceDlg::OnBnClickedClearButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(delete_img())
	{
		AfxMessageBox(L"ɾ���ɹ���");
		sampleNum = 0;
		 CString str1,str2;
	    str1="ѵ����������������";
		str2.Format(_T("%d"), sampleNum);
	    str1 += str2;
	    SetDlgItemText(IDC_STATE_TEXT,str1);
	}
	else
	{
		AfxMessageBox(L"ɾ��ʧ�ܣ�");
	}
}



