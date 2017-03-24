#pragma once
#include <vector>

#define STATISTICS_LOOP_TIMES 1

class Sort 
{
public:
	static int InsertionSortAscending(std::vector<int>& data);
	static int InsertionSortDescending(std::vector<int>& data);

	static int InsertionShellAscending(std::vector<int>& data);

	static int SimpleSelectionAscending(std::vector<int>& data);
	static int SimpleSelectionAscendingEx(std::vector<int>& data);

	static int HeapAscending(std::vector<int>& data);
};
