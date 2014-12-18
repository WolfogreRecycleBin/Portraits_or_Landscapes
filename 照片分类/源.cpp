#include <io.h>
#include <string>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

//��������
void getFiles(string &path, vector<string> & files);
bool IsImage(string name);
bool Detect(CascadeClassifier &cascade, string file_path_name);
bool LoadCascadeFile(CascadeClassifier &cascade, string &cascade_name);
void CopyFile(string path, string name, bool if_face);

//������
int main(void)
{
	CascadeClassifier face_cascade;
	string cascade_name = "haarcascade_frontalface_alt.xml";
	LoadCascadeFile(face_cascade, cascade_name);
	string filePath;												//����·��
	vector<string> files;											//�����ļ�������
	getFiles(filePath, files);										//���ú�����ѯ��·��������ͼƬ�ļ�
	string command = "md " + filePath + "������\\";
	system(command.c_str());
	command = "md " + filePath + "�羰��\\";
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

//����ͼƬ�ļ�����
void getFiles(string &path, vector<string>& files)
{
	cout << "�������ļ�·��:" << endl;
	getline(cin, path);												//����Ŀ¼
	path.append("\\");												//��Ŀ¼����һ��б�ܣ������������
	long   hFile = 0;
	struct _finddata_t fileinfo;									//�����ļ���Ϣ����
	string p;														//����������������ʱĿ¼
	if ((hFile = _findfirst(p.assign(path).append("*").c_str(), &fileinfo)) != -1)//Ŀ¼��Ч
	{
		do															//����ѭ�������������ļ�
		{
			if (!(fileinfo.attrib &  _A_SUBDIR) && IsImage(fileinfo.name))//����������Ĳ���Ŀ¼������ͼƬ�ļ�
				files.push_back(fileinfo.name);						//�ļ���ѹջ����
		} while (_findnext(hFile, &fileinfo) == 0);					//Ѱ����һ���ļ�
		_findclose(hFile);
	}
	int size = files.size();
	cout << "��Ŀ¼��" << path << "���·���" << size << "��ͼƬ�ļ�:" << endl;
	for (int i = 0; i < size; i++)									//�����������ͼƬ�ļ��ļ���
		cout << files[i].c_str() << endl;
	if (size == 0)													//������������ļ���0�������ٴ�����Ŀ¼��������
	{
		cout << "����·���Ƿ���ȷ,����������·��!" << endl << endl;
		getFiles(path, files);										//�ݹ����
	}
}

//�ж��Ƿ�ΪͼƬ�ļ�����
bool IsImage(string name)
{
	char p[4];														//C�ַ�����ʱ����
	name._Copy_s(p, 5, 3, name.length() - 3);						//������չ��
	p[3] = '\0';
	string ext_name(p);												//�������ַ����洢��չ��
	return (ext_name == "jpg" || ext_name == "JPG" || ext_name == "png" || ext_name == "PNG");//��֧�ֵ�ͼƬ�ļ���չ���Ƚϣ������ȽϽ��;
}

bool LoadCascadeFile(CascadeClassifier &cascade, string &cascade_name)
{
	cascade.load(cascade_name);
	if (cascade.empty())
	{ 
		cout << "û����ȷ�����ļ�" << cascade_name << endl;
		return false;
	}
	cout << "����ȷ�����ļ�" << cascade_name << endl;
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
	imshow("���������", showimage);
	waitKey(1);
	return faces.size() != 0;
}

void CopyFile(string path, string name, bool if_face)
{
	string command;
	if (if_face)
	{
		command = "copy " + path + name + " " + path + "������\\";
		system(command.c_str());
		return;
	}
	command = "copy " + path + name + " " + path + "�羰��\\";
	system(command.c_str());
	return;
}