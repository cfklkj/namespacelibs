
#include "Fly_CwriteLog.h"
#include <Windows.h>
#include <stdio.h>
#pragma warning(disable:4996)
 
bool CWriteLog::open(const std::string& path) {
	close();
	m_file = fopen(path.c_str(), "w");
	return m_file ? true : false;
}
void CWriteLog::close()
{
	if (m_file)
		fclose(m_file);
	m_file = NULL;
}
void  CWriteLog::addIndent() {
	if (m_file)
		m_indent++;
}
void  CWriteLog::removeIndent() {
	if (m_file && m_indent > 0) {
		m_indent--;
	}
}
void  CWriteLog::writeLine(const char* format, ...) {
	if (!m_file)
		return;
	for (int i = 0; i < m_indent * TABSIZE; i++) {
		fprintf(m_file, " ");
	}
	va_list ap;
	va_start(ap, format);
	vfprintf(m_file, format, ap);
	va_end(ap);
	fprintf(m_file, "\n");
}
void  CWriteLog::write(const char* format, ...) {
	if (!m_file)
		return;
	for (int i = 0; i < m_indent * TABSIZE; i++) {
		fprintf(m_file, " ");
	}
	va_list ap;
	va_start(ap, format);
	vfprintf(m_file, format, ap);
	va_end(ap);
} 