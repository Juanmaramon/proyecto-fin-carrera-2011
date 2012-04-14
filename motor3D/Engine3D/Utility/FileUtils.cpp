#include "FileUtils.h"

std::string cFileUtils::GetDirectory(const std::string lacFile)
{
	for (int liCharIndex=lacFile.length()-1;liCharIndex >= 0;--liCharIndex)
	{
		if (lacFile[liCharIndex] == '\\' || lacFile[liCharIndex] == '/')
		{
			return lacFile.substr(0, liCharIndex);
		}
	}
	return "";
}

std::string cFileUtils::GetFilename(const std::string lacFile)
{
	for (int liCharIndex=lacFile.length()-1;liCharIndex>=0;--liCharIndex)
	{
		if (lacFile[liCharIndex] == '\\' || lacFile[liCharIndex] == '/')
		{
			return lacFile.substr(liCharIndex + 1);
		}
	}
	return lacFile;
}


bool cFileUtils::CheckFile(const std::string lacFile) {
	FILE* fp = fopen(lacFile.c_str(), "r");
	if (fp) {
		// file exists
		fclose(fp);
		return true;
	} else {
		// file doesn't exist
		return false;
	}
}