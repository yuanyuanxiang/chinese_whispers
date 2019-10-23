#include "faces_classify.h"
#include <io.h>
#include <windows.h>

// 递归删除目录
bool DeleteFolder(const std::string &folder_path)
{
	bool ret = true;
	//文件句柄
	intptr_t hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	std::string p;
	try
	{
		if ((hFile = _findfirst(p.assign(folder_path).append("\\*.*").c_str(), &fileinfo)) != -1)
		{
			do {
				if (0 == strcmp(fileinfo.name, ".") || 0 == strcmp(fileinfo.name, ".."))
					continue;
				std::string child = folder_path + "\\" + fileinfo.name;
				if (FILE_ATTRIBUTE_DIRECTORY & fileinfo.attrib) //子目录
				{
					if (DeleteFolder(child)) {
						BOOL b = RemoveDirectory(child.c_str());
						if (FALSE == b) ret = false;
						printf("删除\"%s\"%s\n", child.c_str(), b ? "成功" : "失败");
					}
					else ret = false;
				}
				else // 非空目录
				{
					if (!DeleteFileA(child.c_str())) {
						ret = false;
						printf("删除\"%s\"失败\n", child.c_str());
					}
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}
	catch (std::exception e) { if (hFile) _findclose(hFile); }

	return ret;
}

/**
* @brief MAIN
* @details 用法: chinese_whispers 参数1 参数2
*		第一个参数指定人脸数据库文件
*		第二个参数指定原始人脸图像存放目录
* 其中"face.db"数据库第1列为id，第2列为图片名
* 第3列为class类别，第4列为embedding标记
* 第5-516列为p1~p512，是特片的特征向量。
*/
int main(int argc, char *argv[])
{
	const char *db = argc > 1 ? argv[1] : "face.db";
	const char *src = argc > 2 ? argv[2] : "face";
	DeleteFolder("./classify");
	faces_classify fc;
	fc.load(db);
	fc.CW(src, 0.8);
	fc.clear();
	system("PAUSE");

	return 0;
}
