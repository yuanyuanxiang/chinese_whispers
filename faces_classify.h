#pragma once
#include <map>
#include "config.h"
#include "chinese_whispers.h"
using namespace dlib;

// ������������
class facevec
{
public:
	cv::Vec<double, 512> data;
	friend inline double operator-(const facevec &v1, const facevec &v2)
	{
		double s = cv::norm(v1.data - v2.data, CV_L2);
		return s;
	}
};


/**
* @class faces_classify
* @brief ��CW����������ͼ����з���
* @details ����ͼ��洢��face.db��"faceInfo"����:
* id path class embedding p1 p2 ... p512
* id��ÿ�������Ĺؼ���,pathΪ����ͼ��λ��
* classΪ����������,embeddingΪ1,�����Ϊ��������
* "face"�ļ��м�"face.db"���ݿ�����ڵ�ǰĿ¼
* @date 2018-7-8
*/
class faces_classify
{
private:
	int num;
	facevec *data;
	std::map<int, std::string> faces;
	void save(int index, int class_id);
	std::string m_folder;

public:
	faces_classify(void);
	~faces_classify(void);
	// �������ݿ�
	void load(const char *db);
	// ��ս��
	void clear();
	// ����CW�����㷨
	void CW(const char *src);
};
