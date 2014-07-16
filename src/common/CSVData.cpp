#include <windows.h>

#include <fstream>

#include "CSVData.h"
#include "csv.h"

void item_callback(void* s,size_t i,void* data) {
	CSVData* csv_data = static_cast<CSVData*>(data);
	
	std::string content(static_cast<const char*>(s));
	std::string search("\\n");
	
	for(size_t place = content.find(search); place != std::string::npos; place = content.find(search)) {
		content.replace(place,search.length(),"\n\r");
	}
	
	#ifdef _WIN32
		int size_needed = MultiByteToWideChar(CP_UTF8,0,content.c_str(),static_cast<int>(content.size()),NULL,0);
		std::wstring wcontent = L"";
		if(size_needed) {
			wcontent = std::wstring(size_needed,0);
			MultiByteToWideChar(CP_UTF8,0,content.c_str(),static_cast<int>(content.size()),&wcontent[0],size_needed);
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
