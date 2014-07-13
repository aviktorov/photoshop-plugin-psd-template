#include <cstdio>

#include "CSVData.h"
#include "csv.h"

void item_callback(void* s,size_t i,void* data) {
	CSVData* csv_data = static_cast<CSVData*>(data);
	csv_data->addItem(static_cast<const char*>(s));
}

void row_callback(int c,void* data) {
	CSVData* csv_data = static_cast<CSVData*>(data);
	csv_data->addRow();
}

int CSVData::load(const std::string& name) {
	FILE* file = fopen(name.c_str(),"rt");
	if(file == NULL) return 0;
	
	fseek(file,0,SEEK_END);
	size_t size = ftell(file);
	fseek(file,0,SEEK_SET);
	
	char* buffer = new char[size];
	fread(buffer,sizeof(char),size,file);
	fclose(file);
	
	csv_parser parser;
	csv_init(&parser,CSV_APPEND_NULL);
	if(csv_parse(&parser,buffer,size,item_callback,row_callback,this) != size) {
		delete [] buffer;
		return 0;
	}
	
	if(csv_fini(&parser,item_callback,row_callback,this) == -1) {
		delete [] buffer;
		return 0;
	}
	
	csv_free(&parser);
	delete [] buffer;
	
	return 1;
}
