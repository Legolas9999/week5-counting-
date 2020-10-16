#include<iostream>
#include<opencv2/opencv.hpp>
using namespace cv;


int main()
{
	
	//-------------练习1：图像的二值化、腐蚀、膨胀、开运算、闭运算--------------
	Mat yuan = imread("cv.jpg",0);							//原图灰度图
	Mat otsu;												//otsu法后二值化图
	Mat Materode;											//腐蚀后图像
	Mat Matdilate;											//膨胀后图像
	Mat open;												//开运算
	Mat close;												//闭运算

	threshold(yuan, otsu, 100, 255, THRESH_OTSU);			//おつ
	imshow("otsu(二值化)", otsu);
	Mat element =getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));		//基本形态学函数，3*3矩阵,锚点
    erode(otsu, Materode, element,Point(-1, -1), 1);							//腐蚀，迭代1次
	imshow("腐蚀", Materode);
	dilate(otsu, Matdilate, element, Point(-1, -1), 1);							//膨胀，迭代1次
	imshow("膨胀", Matdilate);
	morphologyEx(otsu, open, 3, element, Point(-1, -1), 1);						//形态学综合函数，开运算，迭代1次
	imshow("开运算", open);
	morphologyEx(otsu, close, 4, element, Point(-1, -1), 1);					//形态学综合函数，闭运算，迭代1次
	imshow("闭运算", close);
	
	
	//--------------练习2：连通域标记，并绘制外接四边形---------------------------------
	int shu;				//连通域 + 背景 个数
	Mat labels;				//记录每个像素连通域标签号
	Mat stats;				//状态矩阵
	Mat cen;				//记录连通域中心的矩阵
	Mat dilate2;			//两次膨胀迭代后图像
	morphologyEx(Matdilate, dilate2, 1, element, Point(-1, -1), 2);			//形态学综合函数，膨胀运算，迭代1次
	shu = connectedComponentsWithStats(dilate2,labels,stats,cen,4,CV_32S);	//连通域标记函数
	imshow("两次膨胀", dilate2);
	std::cout << "硬币个数为" << std::endl;					//打印硬币个数
	std::cout << shu-1 <<std::endl;
	std::cout << "硬币：状态矩阵" << std::endl;				//打印硬币 状态矩阵
	std::cout << stats << std::endl;
	
	for (int i = 1; i < 11; i++)				//遍历所有连通域，并绘画矩形
	{
		Rect rect;								//设置变量rect的参数
		rect.x = stats.at<int>(i,0);
		rect.y = stats.at<int>(i,1);
		rect.width = stats.at<int>(i, 2);
		rect.height = stats.at<int>(i, 3);
		rectangle(dilate2,rect , CV_RGB(255, 255, 255), 2, 8, 0);		//画矩形的函数
	}
	imshow("计数", dilate2);

	
	//--------------------练习3：计数pcb板的原点个数--------------------------------
	Mat pcb = imread("pcb.jpg", 0);
	Mat otsu_pcb;										//おつ二值化
	Mat erode_pcb;										//腐蚀图像
	Mat dilate_pcb;										//膨胀图像
	//imshow("pcb",pcb);
	threshold(pcb, otsu_pcb, 100, 255, THRESH_OTSU);		//おつ
	imshow("二值化", otsu_pcb);
	Mat pcb_element = getStructuringElement(MORPH_RECT, Size(7, 7), Point(-1, -1));		//基本形态学函数，7*7矩形，锚点位于中心
	morphologyEx(otsu_pcb, erode_pcb,1, pcb_element, Point(-1, -1), 4);				//腐蚀操作，迭代4次
	morphologyEx(erode_pcb, dilate_pcb, 0, pcb_element, Point(-1, -1), 4);			//膨胀操作，迭代4次
	imshow("腐蚀",erode_pcb);
	imshow("膨胀", dilate_pcb);
	Mat count=255 - dilate_pcb;				//前景和后景转换
	int circle;					//记录 连通域+背景 
	Mat pcb_labels;				//记录每个像素连通域标号
	Mat pcb_stats;				//状态矩阵
	Mat pcb_cen;				//记录连通域中心的矩阵
	circle=connectedComponentsWithStats(count, pcb_labels, pcb_stats, pcb_cen, 4, CV_32S);		//连通域标记函数
	for (int i = 1; i < 31; i++)		//遍历每个连通域，并画矩形
	{
		Rect rect;
		rect.x = pcb_stats.at<int>(i, 0);
		rect.y = pcb_stats.at<int>(i, 1);
		rect.width = pcb_stats.at<int>(i, 2);
		rect.height = pcb_stats.at<int>(i, 3);
		rectangle(count,rect,CV_RGB(255,255,255),1,8,0);
	}
	imshow("黑白反转", count);
	std::cout << "原点个数" << std::endl;
	std::cout << circle-1<< std::endl;
	std::cout << "pcb状态矩阵" << std::endl;
	std::cout << pcb_stats << std::endl;



	//-----------------------练习4：计数回形针个数-------------------------------
	Mat pin = imread("pin.jpg", 0);
	Mat pin_otsu;			//二值化
	Mat pin_erode;			//腐蚀
	Mat pin_suo;			//图像裁剪
	Mat pin_labels;			//记录每个像素连通域标号
	Mat pin_stats;			//状态矩阵
	Mat pin_cen;			//记录连通域中心的矩阵
	Mat pin_revers;			//前景和背景互换
	int pin_counter;		//连通域+背景
	threshold(pin, pin_otsu, 100, 255, THRESH_OTSU);		//二值化
	imshow("otsu", pin_otsu);
	Mat pin_element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));		//基本形态学函数，3*3矩形，锚点位于中心
	morphologyEx(pin_otsu, pin_erode, 1, pin_element, Point(-1, -1), 1);		//腐蚀1次
	imshow("erode",pin_erode);
	Rect pin_rect(10, 10, 831, 823);		//设置rect 对图像进行裁剪
	pin_suo = pin_erode(pin_rect);			//图像裁剪
	//imshow("裁剪图像",pin_suo);
	pin_revers = 255 - pin_suo;				//前景和背景互换
	pin_counter= connectedComponentsWithStats(pin_revers, pin_labels, pin_stats, pin_cen, 4, CV_32S);
	for (int i = 1; i < 14; i++)			//遍历每个连通域，并画矩形
		{
			Rect rect;
			rect.x = pin_stats.at<int>(i, 0);
			rect.y = pin_stats.at<int>(i, 1);
			rect.width = pin_stats.at<int>(i, 2);
			rect.height = pin_stats.at<int>(i, 3);
			rectangle(pin_revers,rect,CV_RGB(255,255,255),1,8,0);
		}
	imshow("计数", pin_revers);
	std::cout << "回形针个数" << std::endl;
	std::cout << pin_counter-1<< std::endl;
	std::cout << "回形针状态矩阵" << std::endl;
	std::cout << pin_stats << std::endl;

	waitKey(0);
}

