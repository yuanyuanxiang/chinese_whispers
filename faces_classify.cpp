#include "faces_classify.h"
#include "mString.h"
#include <iostream>
#include <direct.h>
#include <io.h>


faces_classify::faces_classify(void)
{
	num = 0;
	data = NULL;
}


faces_classify::~faces_classify(void)
{
	if (data)
		delete [] data;
}

void faces_classify::load(const char *db)
{
	if (-1 == _access(db, 0))
	{
		printf("数据库文件\"%s\"不存在!\n", db);
		return;
	}
	NAMES::db_handle* m_pDatabase = new NAMES::db_sqlite(db);
	if (m_pDatabase->open() && m_pDatabase->sql_select("select * from faceInfo;"))
	{
		const NAMES::db_rows* result = m_pDatabase->get_result();
		if (result)
		{
			const std::vector<NAMES::db_row*>& rows = result->get_rows();
			num = rows.size();
			if (NULL == data) data = new facevec[num];
			facevec *v = data;
			for (std::vector<NAMES::db_row*>::const_iterator p = rows.begin(); 
				p != rows.end(); ++p, ++v)
			{
				const NAMES::db_row* cur = *p;
				int id = atoi((*cur)[0]);
				const char *path = (*cur)[1];
				faces[id] = path;
#pragma omp parallel for
				for (int i = 0; i < 512; ++i)
					v->data[i] = atof((*cur)[4 + i]);
			}
		}
	}
	delete m_pDatabase;
}


void faces_classify::clear()
{
	num = 0;
	SAFEDELETE_ARRAY(data);
	faces.clear();
}


void faces_classify::CW(const char *src)
{
	if (0 == num || NULL == data)
		return;
	if (-1 == _access(src, 0))
	{
		printf("图片目录\"%s\"不存在!\n", src);
		return;
	}
	m_folder = src;

	ARRAY<sample_pair> edges(USING_STL ? 0 : num * num / 16);
	for (int i = 0; i < num; ++i)
	{
		for (int j = i; j < num; ++j)
		{
			double d = i == j ? 0 : data[j] - data[i];
			if (d < 0.6) edges.push_back(sample_pair(i, j));
		}
	}
	ARRAY<unsigned long> labels;
	const int num_clusters = chinese_whispers(edges, labels);
	int *count = new int[num_clusters]();
	std::cout << "element num: " << num << ", number of clusters: "<< num_clusters << "\n";
	for (int cluster_id = 0; cluster_id < num_clusters; ++cluster_id)
	{
		std::cout << "cluster " << cluster_id << ": ";
		for (int j = 0; j < labels.size(); ++j)
		{
			if (cluster_id == labels[j])
			{
				++count[cluster_id];
				save(j, cluster_id);
			}
		}
		std::cout << count[cluster_id] << "个。\n";
	}
	delete [] count;
}


void faces_classify::save(int index, int class_id)
{
	std::map<int, std::string>::iterator p = faces.begin();
	while(index--) ++p;
	std::string path = p->second;
	
	char dir[_MAX_PATH], src[_MAX_PATH], dst[_MAX_PATH];
	sprintf_s(dir, "classify/%d", class_id);
	_mkdir("classify");
	_mkdir(dir);
	sprintf_s(src, "%s/%s", m_folder.c_str(), path.c_str());
	sprintf_s(dst, "%s/%s", dir, path.c_str());
	if (FALSE == CopyFile(src, dst, TRUE))
		TRACE("CopyFile %s failed.\n", path.c_str());
}
