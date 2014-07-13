#include <fstream>
#include <windows.h>

#include "CSVData.h"
#include "csv.h"

void item_callback(void* s,size_t i,void* data) {
	CSVData* csv_data = static_cast<CSVData*>(data);
	
	#ifdef _WIN32
		int size_needed = MultiByteToWideChar(CP_UTF8,0,static_cast<char*>(s),static_cast<int>(i),NULL,0);
		std::wstring wcontent = L"";
		if(size_needed) {
			wcontent = std::wstring(size_needed,0);
			MultiByteToWideChar(CP_UTF8,0,static_cast<char*>(s),static_cast<int>(i),&wcontent[0],size_needed);
		}
	#else
		std::wstring wcontent = L"implement me";
	#endif
	
	csv_data->addItem(wcontent);
}

void row_callback(int c,void* data) {
	CSVData* csv_data = static_cast<CSVData*>(data);
	csv_data->addRow();
}

int CSVData::load(const std::string& name) {
	std::ifstream file(name);
	std::string content = std::string(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());
	
	csv_parser parser;
	csv_init(&parser,CSV_APPEND_NULL);
	if(csv_parse(&parser,content.c_str(),content.size(),item_callback,row_callback,this) != content.size()) {
		return 0;
	}
	
	if(csv_fini(&parser,item_callback,row_callback,this) == -1) {
		return 0;
	}
	
	csv_free(&parser);
	
	return 1;
}
