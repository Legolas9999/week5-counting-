#include<iostream>
#include<opencv2/opencv.hpp>
using namespace cv;


int main()
{
	
	//-------------��ϰ1��ͼ��Ķ�ֵ������ʴ�����͡������㡢������--------------
	Mat yuan = imread("cv.jpg",0);							//ԭͼ�Ҷ�ͼ
	Mat otsu;												//otsu�����ֵ��ͼ
	Mat Materode;											//��ʴ��ͼ��
	Mat Matdilate;											//���ͺ�ͼ��
	Mat open;												//������
	Mat close;												//������

	threshold(yuan, otsu, 100, 255, THRESH_OTSU);			//����
	imshow("otsu(��ֵ��)", otsu);
	Mat element =getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));		//������̬ѧ������3*3����,ê��
    erode(otsu, Materode, element,Point(-1, -1), 1);							//��ʴ������1��
	imshow("��ʴ", Materode);
	dilate(otsu, Matdilate, element, Point(-1, -1), 1);							//���ͣ�����1��
	imshow("����", Matdilate);
	morphologyEx(otsu, open, 3, element, Point(-1, -1), 1);						//��̬ѧ�ۺϺ����������㣬����1��
	imshow("������", open);
	morphologyEx(otsu, close, 4, element, Point(-1, -1), 1);					//��̬ѧ�ۺϺ����������㣬����1��
	imshow("������", close);
	
	
	//--------------��ϰ2����ͨ���ǣ�����������ı���---------------------------------
	int shu;				//��ͨ�� + ���� ����
	Mat labels;				//��¼ÿ��������ͨ���ǩ��
	Mat stats;				//״̬����
	Mat cen;				//��¼��ͨ�����ĵľ���
	Mat dilate2;			//�������͵�����ͼ��
	morphologyEx(Matdilate, dilate2, 1, element, Point(-1, -1), 2);			//��̬ѧ�ۺϺ������������㣬����1��
	shu = connectedComponentsWithStats(dilate2,labels,stats,cen,4,CV_32S);	//��ͨ���Ǻ���
	imshow("��������", dilate2);
	std::cout << "Ӳ�Ҹ���Ϊ" << std::endl;					//��ӡӲ�Ҹ���
	std::cout << shu-1 <<std::endl;
	std::cout << "Ӳ�ң�״̬����" << std::endl;				//��ӡӲ�� ״̬����
	std::cout << stats << std::endl;
	
	for (int i = 1; i < 11; i++)				//����������ͨ�򣬲��滭����
	{
		Rect rect;								//���ñ���rect�Ĳ���
		rect.x = stats.at<int>(i,0);
		rect.y = stats.at<int>(i,1);
		rect.width = stats.at<int>(i, 2);
		rect.height = stats.at<int>(i, 3);
		rectangle(dilate2,rect , CV_RGB(255, 255, 255), 2, 8, 0);		//�����εĺ���
	}
	imshow("����", dilate2);

	
	//--------------------��ϰ3������pcb���ԭ�����--------------------------------
	Mat pcb = imread("pcb.jpg", 0);
	Mat otsu_pcb;										//���Ķ�ֵ��
	Mat erode_pcb;										//��ʴͼ��
	Mat dilate_pcb;										//����ͼ��
	//imshow("pcb",pcb);
	threshold(pcb, otsu_pcb, 100, 255, THRESH_OTSU);		//����
	imshow("��ֵ��", otsu_pcb);
	Mat pcb_element = getStructuringElement(MORPH_RECT, Size(7, 7), Point(-1, -1));		//������̬ѧ������7*7���Σ�ê��λ������
	morphologyEx(otsu_pcb, erode_pcb,1, pcb_element, Point(-1, -1), 4);				//��ʴ����������4��
	morphologyEx(erode_pcb, dilate_pcb, 0, pcb_element, Point(-1, -1), 4);			//���Ͳ���������4��
	imshow("��ʴ",erode_pcb);
	imshow("����", dilate_pcb);
	Mat count=255 - dilate_pcb;				//ǰ���ͺ�ת��
	int circle;					//��¼ ��ͨ��+���� 
	Mat pcb_labels;				//��¼ÿ��������ͨ����
	Mat pcb_stats;				//״̬����
	Mat pcb_cen;				//��¼��ͨ�����ĵľ���
	circle=connectedComponentsWithStats(count, pcb_labels, pcb_stats, pcb_cen, 4, CV_32S);		//��ͨ���Ǻ���
	for (int i = 1; i < 31; i++)		//����ÿ����ͨ�򣬲�������
	{
		Rect rect;
		rect.x = pcb_stats.at<int>(i, 0);
		rect.y = pcb_stats.at<int>(i, 1);
		rect.width = pcb_stats.at<int>(i, 2);
		rect.height = pcb_stats.at<int>(i, 3);
		rectangle(count,rect,CV_RGB(255,255,255),1,8,0);
	}
	imshow("�ڰ׷�ת", count);
	std::cout << "ԭ�����" << std::endl;
	std::cout << circle-1<< std::endl;
	std::cout << "pcb״̬����" << std::endl;
	std::cout << pcb_stats << std::endl;



	//-----------------------��ϰ4���������������-------------------------------
	Mat pin = imread("pin.jpg", 0);
	Mat pin_otsu;			//��ֵ��
	Mat pin_erode;			//��ʴ
	Mat pin_suo;			//ͼ��ü�
	Mat pin_labels;			//��¼ÿ��������ͨ����
	Mat pin_stats;			//״̬����
	Mat pin_cen;			//��¼��ͨ�����ĵľ���
	Mat pin_revers;			//ǰ���ͱ�������
	int pin_counter;		//��ͨ��+����
	threshold(pin, pin_otsu, 100, 255, THRESH_OTSU);		//��ֵ��
	imshow("otsu", pin_otsu);
	Mat pin_element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));		//������̬ѧ������3*3���Σ�ê��λ������
	morphologyEx(pin_otsu, pin_erode, 1, pin_element, Point(-1, -1), 1);		//��ʴ1��
	imshow("erode",pin_erode);
	Rect pin_rect(10, 10, 831, 823);		//����rect ��ͼ����вü�
	pin_suo = pin_erode(pin_rect);			//ͼ��ü�
	//imshow("�ü�ͼ��",pin_suo);
	pin_revers = 255 - pin_suo;				//ǰ���ͱ�������
	pin_counter= connectedComponentsWithStats(pin_revers, pin_labels, pin_stats, pin_cen, 4, CV_32S);
	for (int i = 1; i < 14; i++)			//����ÿ����ͨ�򣬲�������
		{
			Rect rect;
			rect.x = pin_stats.at<int>(i, 0);
			rect.y = pin_stats.at<int>(i, 1);
			rect.width = pin_stats.at<int>(i, 2);
			rect.height = pin_stats.at<int>(i, 3);
			rectangle(pin_revers,rect,CV_RGB(255,255,255),1,8,0);
		}
	imshow("����", pin_revers);
	std::cout << "���������" << std::endl;
	std::cout << pin_counter-1<< std::endl;
	std::cout << "������״̬����" << std::endl;
	std::cout << pin_stats << std::endl;

	waitKey(0);
}

