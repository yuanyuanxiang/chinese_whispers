#include "faces_classify.h"

/**
* @brief MAIN
* @details �÷�: chinese_whispers ����1 ����2
*		��һ������ָ���������ݿ��ļ�
*		�ڶ�������ָ��ԭʼ����ͼ����Ŀ¼
*/
int main(int argc, char *argv[])
{
	const char *db = argc > 1 ? argv[1] : "face.db";
	const char *src = argc > 2 ? argv[2] : "face";
	faces_classify fc;
	fc.load(db);
	fc.CW(src);
	fc.clear();
	system("PAUSE");

	return 0;
}
