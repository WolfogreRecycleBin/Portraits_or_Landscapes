#include <io.h>
#include <iomanip>
#include <string>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

/*函数声明*/
void GetFiles(string &path, vector<string> & files);
bool IsImage(string name);
bool Detect(CascadeClassifier &cascade, string file_path_name);
bool LoadCascadeFile(CascadeClassifier &cascade, string &cascade_file_name);
void CopyFile(string path, string name, bool is_face);

/*主函数*/
int main(void)
{
	/*准备级联检测器*/
	CascadeClassifier face_cascade;									//定义级联检测器
	string cascade_file_name = "haarcascade_frontalface_alt.xml";	//定义级联文件名
	if (!LoadCascadeFile(face_cascade, cascade_file_name))			//自定义函数加载级联文件
	{
		system("pause");											//若加载级联文件失败返回系统
		return -1;
	}

	/*获得待处理文件*/
	string filePath;												//定义路径
	vector<string> files;											//定义文件名容器
	GetFiles(filePath, files);										//调用函数，询问路径，搜索图片文件

	/*创建用于拷贝的目标文件夹*/
	string command = "md " + filePath + "人物照\\";					//生成新建文件夹命令
	system(command.c_str());										//执行
	command = "md " + filePath + "风景照\\";						//生成新建文件夹命令
	system(command.c_str());										//执行

	/*逐个处理文件*/
	for (unsigned i = 0; i < files.size(); i++)							//逐个访问
	{
		string file_path_name = filePath + files[i];				//生成完整文件路径+文件名
		if (Detect(face_cascade, file_path_name))					//调用函数检测是否有脸
			CopyFile(filePath, files[i], 1);						//有脸拷入人物照
		else
			CopyFile(filePath, files[i], 0);						//无脸拷入风景照
	}
	destroyAllWindows();											//关闭所有窗口
	system("pause");
	return 0;														//返回系统
}

/*函数：搜索图片文件*/
void GetFiles(string &path, vector<string>& files)
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
		GetFiles(path, files);										//递归调用
	}
}

/*函数：判断是否为图片文件*/
bool IsImage(string name)
{
	char p[4];														//C字符串临时变量
	name._Copy_s(p, 5, 3, name.length() - 3);						//拷贝扩展名
	p[3] = '\0';
	string ext_name(p);												//定义类字符串存储扩展名
	return (ext_name == "jpg" || ext_name == "JPG" || ext_name == "png" || ext_name == "PNG");//与支持的图片文件扩展名比较，返货比较结果;
}

/*函数：加载级联文件并返回结果*/
bool LoadCascadeFile(CascadeClassifier &cascade, string &cascade_file_name)
{
	cascade.load(cascade_file_name);								//加载级联文件
	if (cascade.empty())											//判断是否成功加载
	{ 
		cout << "没有正确加载文件" << cascade_file_name << endl;
		return false;
	}
	cout << "已正确加载文件" << cascade_file_name << endl;
	return true;
}

/*函数：检测图片并返回结果*/
bool Detect(CascadeClassifier &cascade, string file_path_name)
{
	Mat image = imread(file_path_name.c_str());						//加载图片文件
	vector<Rect> faces;												//定义用于存放结果的容器

	/*规整图片大小，便于检测，不影响文件*/
	const double new_row = 600;
	double new_col =  new_row / image.rows * image.cols;
	Size new_size(new_col, new_row);
	resize(image, image, new_size);

	/*执行检测函数*/
	cascade.detectMultiScale(image, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (unsigned i = 0; i < faces.size(); i++)							//在图片中检测到目标的地方画圆，不影响文件
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		circle(image, center, (faces[i].width + faces[i].height)*0.25, Scalar(0, 0, 255), 3);
	}
	imshow("人脸检测结果", image);									//显示检测和和画圆结果
	waitKey(1);
	return faces.size() != 0;										//返回知否检测到
}

/*函数：拷贝文件到相应目录*/
void CopyFile(string path, string name, bool is_face)
{
	string command;													//定义命令
	if (is_face)													//如果有脸
	{
		cout << setw(30) << name<< " 是人物照;	";								//输出提示
		command = "copy " + path + name + " " + path + "人物照\\";	//生成拷贝到人物照的命令
		system(command.c_str());									//执行
		return;
	}
	cout << setw(30) << name << " 是风景照;	";									//输出提示
	command = "copy " + path + name + " " + path + "风景照\\";		//生成拷贝到风景照的命令
	system(command.c_str());										//执行
	return;
}