#pragma once
#include <string>  
 
//п╢нд╪Ч
class CWriteLog {
public:
	bool open(const std::string& path);
	void close();
	void addIndent();
	void removeIndent();
	void writeLine(const char* format, ...);
	void write(const char* format, ...);
private:
	FILE* m_file = NULL;
	int m_indent = 0;
	const int TABSIZE = 3;
};