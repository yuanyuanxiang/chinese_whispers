#pragma once
#include <map>
#include "config.h"
#include "chinese_whispers.h"
using namespace dlib;

template<typename T, int dim>
class Vec
{
public:
	Vec(){ }
	~Vec(){ }
	friend T distance(const Vec &v1, const Vec &v2) {
		T s = 0;
		for (int i=0; i<dim; ++i)
		{
			T t = v1.data[i] - v2.data[i];
			s += t*t;
		}
		return sqrt(s);
	}

	T data[dim];
};

// ������������
typedef Vec<double, 512> facevec;


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
	void CW(const char *src, double threshold = 0.6);
};
