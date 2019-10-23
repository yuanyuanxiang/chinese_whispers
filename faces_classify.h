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

// 人脸特征向量
typedef Vec<double, 512> facevec;


/**
* @class faces_classify
* @brief 用CW方法对人脸图像进行分类
* @details 人脸图像存储在face.db的"faceInfo"表中:
* id path class embedding p1 p2 ... p512
* id即每个人脸的关键字,path为人脸图像位置
* class为待分类类别号,embedding为1,后面的为特征向量
* "face"文件夹及"face.db"数据库放置于当前目录
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
	// 加载数据库
	void load(const char *db);
	// 清空结果
	void clear();
	// 运行CW分类算法
	void CW(const char *src, double threshold = 0.6);
};
