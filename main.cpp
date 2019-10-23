#include "faces_classify.h"
#include <io.h>
#include <windows.h>

// �ݹ�ɾ��Ŀ¼
bool DeleteFolder(const std::string &folder_path)
{
	bool ret = true;
	//�ļ����
	intptr_t hFile = 0;
	//�ļ���Ϣ  
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
				if (FILE_ATTRIBUTE_DIRECTORY & fileinfo.attrib) //��Ŀ¼
				{
					if (DeleteFolder(child)) {
						BOOL b = RemoveDirectory(child.c_str());
						if (FALSE == b) ret = false;
						printf("ɾ��\"%s\"%s\n", child.c_str(), b ? "�ɹ�" : "ʧ��");
					}
					else ret = false;
				}
				else // �ǿ�Ŀ¼
				{
					if (!DeleteFileA(child.c_str())) {
						ret = false;
						printf("ɾ��\"%s\"ʧ��\n", child.c_str());
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
* @details �÷�: chinese_whispers ����1 ����2
*		��һ������ָ���������ݿ��ļ�
*		�ڶ�������ָ��ԭʼ����ͼ����Ŀ¼
* ����"face.db"���ݿ��1��Ϊid����2��ΪͼƬ��
* ��3��Ϊclass��𣬵�4��Ϊembedding���
* ��5-516��Ϊp1~p512������Ƭ������������
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
