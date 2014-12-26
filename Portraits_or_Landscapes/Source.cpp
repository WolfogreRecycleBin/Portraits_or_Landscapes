#include <io.h>
#include <iomanip>
#include <string>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

/*��������*/
void GetFiles(string &path, vector<string> & files);
bool IsImage(string name);
bool Detect(CascadeClassifier &cascade, string file_path_name);
bool LoadCascadeFile(CascadeClassifier &cascade, string &cascade_file_name);
void CopyFile(string path, string name, bool is_face);

/*������*/
int main(void)
{
	/*׼�����������*/
	CascadeClassifier face_cascade;									//���弶�������
	string cascade_file_name = "haarcascade_frontalface_alt.xml";	//���弶���ļ���
	if (!LoadCascadeFile(face_cascade, cascade_file_name))			//�Զ��庯�����ؼ����ļ�
	{
		system("pause");											//�����ؼ����ļ�ʧ�ܷ���ϵͳ
		return -1;
	}

	/*��ô������ļ�*/
	string filePath;												//����·��
	vector<string> files;											//�����ļ�������
	GetFiles(filePath, files);										//���ú�����ѯ��·��������ͼƬ�ļ�

	/*�������ڿ�����Ŀ���ļ���*/
	string command = "md " + filePath + "������\\";					//�����½��ļ�������
	system(command.c_str());										//ִ��
	command = "md " + filePath + "�羰��\\";						//�����½��ļ�������
	system(command.c_str());										//ִ��

	/*��������ļ�*/
	for (unsigned i = 0; i < files.size(); i++)							//�������
	{
		string file_path_name = filePath + files[i];				//���������ļ�·��+�ļ���
		if (Detect(face_cascade, file_path_name))					//���ú�������Ƿ�����
			CopyFile(filePath, files[i], 1);						//��������������
		else
			CopyFile(filePath, files[i], 0);						//��������羰��
	}
	destroyAllWindows();											//�ر����д���
	system("pause");
	return 0;														//����ϵͳ
}

/*����������ͼƬ�ļ�*/
void GetFiles(string &path, vector<string>& files)
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
		GetFiles(path, files);										//�ݹ����
	}
}

/*�������ж��Ƿ�ΪͼƬ�ļ�*/
bool IsImage(string name)
{
	char p[4];														//C�ַ�����ʱ����
	name._Copy_s(p, 5, 3, name.length() - 3);						//������չ��
	p[3] = '\0';
	string ext_name(p);												//�������ַ����洢��չ��
	return (ext_name == "jpg" || ext_name == "JPG" || ext_name == "png" || ext_name == "PNG");//��֧�ֵ�ͼƬ�ļ���չ���Ƚϣ������ȽϽ��;
}

/*���������ؼ����ļ������ؽ��*/
bool LoadCascadeFile(CascadeClassifier &cascade, string &cascade_file_name)
{
	cascade.load(cascade_file_name);								//���ؼ����ļ�
	if (cascade.empty())											//�ж��Ƿ�ɹ�����
	{ 
		cout << "û����ȷ�����ļ�" << cascade_file_name << endl;
		return false;
	}
	cout << "����ȷ�����ļ�" << cascade_file_name << endl;
	return true;
}

/*���������ͼƬ�����ؽ��*/
bool Detect(CascadeClassifier &cascade, string file_path_name)
{
	Mat image = imread(file_path_name.c_str());						//����ͼƬ�ļ�
	vector<Rect> faces;												//�������ڴ�Ž��������

	/*����ͼƬ��С�����ڼ�⣬��Ӱ���ļ�*/
	const double new_row = 600;
	double new_col =  new_row / image.rows * image.cols;
	Size new_size(new_col, new_row);
	resize(image, image, new_size);

	/*ִ�м�⺯��*/
	cascade.detectMultiScale(image, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (unsigned i = 0; i < faces.size(); i++)							//��ͼƬ�м�⵽Ŀ��ĵط���Բ����Ӱ���ļ�
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		circle(image, center, (faces[i].width + faces[i].height)*0.25, Scalar(0, 0, 255), 3);
	}
	imshow("���������", image);									//��ʾ���ͺͻ�Բ���
	waitKey(1);
	return faces.size() != 0;										//����֪���⵽
}

/*�����������ļ�����ӦĿ¼*/
void CopyFile(string path, string name, bool is_face)
{
	string command;													//��������
	if (is_face)													//�������
	{
		cout << setw(30) << name<< " ��������;	";								//�����ʾ
		command = "copy " + path + name + " " + path + "������\\";	//���ɿ����������յ�����
		system(command.c_str());									//ִ��
		return;
	}
	cout << setw(30) << name << " �Ƿ羰��;	";									//�����ʾ
	command = "copy " + path + name + " " + path + "�羰��\\";		//���ɿ������羰�յ�����
	system(command.c_str());										//ִ��
	return;
}