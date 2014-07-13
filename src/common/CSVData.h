#ifndef __CSV_DATA_H__
#define __CSV_DATA_H__

#include <string>
#include <vector>

class CSVData {
	public:
		
		typedef std::vector<std::string> csv_row;
		
		int load(const std::string& name);
		
		inline void addItem(const char* item) { current_row.push_back(std::string(item)); }
		inline void addRow() { rows.push_back(current_row); current_row.clear(); }
		
		inline size_t getNumRows() const { return rows.size(); }
		inline const csv_row& getRow(size_t index) const { return rows[index]; }
		
	private:
		
		csv_row current_row;
		std::vector<csv_row> rows;
};


#endif // __CSV_DATA_H__