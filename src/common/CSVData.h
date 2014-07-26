#ifndef __CSV_DATA_H__
#define __CSV_DATA_H__

#include <string>
#include <vector>

typedef std::vector<std::wstring> csv_row;

class CSVData {
	public:
		
		int load(const std::string& name);
		
		inline void addItem(const std::wstring& item) { current_row.push_back(item); }
		inline void addRow() { rows.push_back(current_row); current_row.clear(); }
		
		inline size_t getNumRows() const { return rows.size(); }
		inline const csv_row& getRow(size_t index) const { return rows[index]; }
		
	private:
		
		csv_row current_row;
		std::vector<csv_row> rows;
};


#endif // __CSV_DATA_H__