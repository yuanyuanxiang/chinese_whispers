#include "faces_classify.h"

/**
* @brief MAIN
* @details 用法: chinese_whispers 参数1 参数2
*		第一个参数指定人脸数据库文件
*		第二个参数指定原始人脸图像存放目录
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
