#include "chinese_whispers.h"
#include "sample_pair.h"
#include <iostream>
using namespace dlib;

// 随机生成一些浮点数据，对CW算法进行测试
int main(int argc, char *argv[])
{
	const int size = 1 == argc ? 512 : std::max(atoi(argv[1]), 10);
	double *data = new double[size];
	srand(time(NULL));
	for (int i = 0; i < size; ++i)
		data[i] = 10.0 * ::rand() / RAND_MAX;
	clock_t t = clock();
	std::vector<sample_pair> edges;
	for (int i = 0; i < size; ++i)
	{
		for (int j = i; j < size; ++j)
		{
			if (std::abs(data[j] - data[i]) < 0.6)
				edges.push_back(sample_pair(i, j));
		}
	}
	std::vector<unsigned long> labels;
	const int num_clusters = chinese_whispers(edges, labels);
	t = clock() - t;
	std::cout << "element num: " << size << ", using time: " << t << "ms\n";
	std::cout << "number of clusters: "<< num_clusters << "\n";
	if (size <= 2048)
	for (int cluster_id = 0; cluster_id < num_clusters; ++cluster_id)
	{
		std::cout << "cluster " << cluster_id << ": ";
		for (int j = 0; j < labels.size(); ++j)
		{
			if (cluster_id == labels[j])
				std::cout << j << ", ";
		}
		std::cout << "\n";
	}

	system("PAUSE");
	delete [] data;
	return 0;
}
