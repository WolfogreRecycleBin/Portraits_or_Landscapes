#include <io.h>
#include <string>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

//函数声明
void getFiles(string &path, vector<string> & files);
bool IsImage(string name);
bool Detect(CascadeClassifier &cascade, string file_path_name);
bool LoadCascadeFile(CascadeClassifier &cascade, string &cascade_name);
void CopyFile(string path, string name, bool if_face);

//主函数
int main(void)
{
	CascadeClassifier face_cascade;
	string cascade_name = "haarcascade_frontalface_alt.xml";
	LoadCascadeFile(face_cascade, cascade_name);
	string filePath;												//定义路径
	vector<string> files;											//定义文件名容器
	getFiles(filePath, files);										//调用函数，询问路径，搜索图片文件
	string command = "md " + filePath + "人物照\\";
	system(command.c_str());
	command = "md " + filePath + "风景照\\";
	system(command.c_str());
	for (int i = 0; i < files.size(); i++)
	{
		string file_path_name = filePath + files[i];
		if (Detect(face_cascade, file_path_name))
			CopyFile(filePath, files[i], 1);
		else
			CopyFile(filePath, files[i], 0);
	}
}

//搜索图片文件函数
void getFiles(string &path, vector<string>& files)
{
	cout << "请输入文件路径:" << endl;
	getline(cin, path);												//读入目录
	path.append("\\");												//将目录后添一反斜杠，方便后续操作
	long   hFile = 0;
	struct _finddata_t fileinfo;									//定义文件信息对象
	string p;														//定义用于搜索的临时目录
	if ((hFile = _findfirst(p.assign(path).append("*").c_str(), &fileinfo)) != -1)//目录有效
	{
		do															//进入循环，不断搜索文件
		{
			if (!(fileinfo.attrib &  _A_SUBDIR) && IsImage(fileinfo.name))//如果搜索到的不是目录，且是图片文件
				files.push_back(fileinfo.name);						//文件名压栈保存
		} while (_findnext(hFile, &fileinfo) == 0);					//寻找下一个文件
		_findclose(hFile);
	}
	int size = files.size();
	cout << "在目录“" << path << "”下发现" << size << "个图片文件:" << endl;
	for (int i = 0; i < size; i++)									//输出搜索到的图片文件文件名
		cout << files[i].c_str() << endl;
	if (size == 0)													//如果搜索到的文件数0，请求再次输入目录重新搜索
	{
		cout << "请检查路径是否正确,并重新输入路径!" << endl << endl;
		getFiles(path, files);										//递归调用
	}
}

//判断是否为图片文件函数
bool IsImage(string name)
{
	char p[4];														//C字符串临时变量
	name._Copy_s(p, 5, 3, name.length() - 3);						//拷贝扩展名
	p[3] = '\0';
	string ext_name(p);												//定义类字符串存储扩展名
	return (ext_name == "jpg" || ext_name == "JPG" || ext_name == "png" || ext_name == "PNG");//与支持的图片文件扩展名比较，返货比较结果;
}

bool LoadCascadeFile(CascadeClassifier &cascade, string &cascade_name)
{
	cascade.load(cascade_name);
	if (cascade.empty())
	{ 
		cout << "没有正确加载文件" << cascade_name << endl;
		return false;
	}
	cout << "已正确加载文件" << cascade_name << endl;
	return true;
}

bool Detect(CascadeClassifier &cascade, string file_path_name)
{
	Mat image = imread(file_path_name.c_str());
	const double new_row = 600;
	vector<Rect> faces; 
	double new_col =  new_row / image.rows * image.cols;
	Size new_size(new_col, new_row);
	resize(image, image, new_size);
	cascade.detectMultiScale(image, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	for (int i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		circle(image, center, (faces[i].width + faces[i].height)*0.25, Scalar(0, 0, 255), 3);
	}
	Mat showimage=image.clone();
	imshow("人脸检测结果", showimage);
	waitKey(1);
	return faces.size() != 0;
}

void CopyFile(string path, string name, bool if_face)
{
	string command;
	if (if_face)
	{
		command = "copy " + path + name + " " + path + "人物照\\";
		system(command.c_str());
		return;
	}
	command = "copy " + path + name + " " + path + "风景照\\";
	system(command.c_str());
	return;
}