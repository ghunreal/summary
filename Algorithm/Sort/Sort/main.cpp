#include <iostream>

#include "../code/Algorithm_Sort.h"

using namespace std;

void print(vector<int>& data)
{
	for (size_t i = 0; i < data.size(); ++i)
	{
		cout << data[i] << "  ";
	}
	cout << endl;
}
int main()
{
	vector<int> testData = {49, 38, 65, 97, 76, 13, 27, 49};
	cout << "=============== original data =================\n";
	print(testData);
	cout << "+++++++++  sorted data +++++++++++ \n";
	int loopTimes = Sort::InsertionSortAscending(testData);
	print(testData);
	cout << "===  total loop times = " << loopTimes << endl;

	vector<int> testData1 = { 49, 38, 65, 97, 76, 13, 27, 49 };
	cout << "=============== Descending original data =================\n";
	print(testData1);
	cout << "+++++++++  sorted data +++++++++++ \n";
	loopTimes = Sort::InsertionSortDescending(testData1);
	print(testData1);
	cout << "===  total loop times = " << loopTimes << endl;


	vector<int> testData2 = { 49, 38, 65, 97, 76, 13, 27, 49 };
	cout << "=============== Shell sorting =================\n";
	cout << "+++++++++  sorted data +++++++++++ \n";
	loopTimes = Sort::InsertionShellAscending(testData2);
	print(testData2);
	cout << "===  total loop times = " << loopTimes << endl;


	cout << "=============== SimpleSelection sorting =================\n";
	vector<int> testData3 = { 49, 38, 65, 97, 76, 13, 27, 49 };
	loopTimes = Sort::SimpleSelectionAscending(testData3);
	cout << "===  total loop times = " << loopTimes << endl;
	print(testData3);

	cout << "=============== SimpleSelectionEx sorting =================\n";
	vector<int> testData4 = { 49, 38, 65, 97, 76, 13, 27, 49 };
	loopTimes = Sort::SimpleSelectionAscendingEx(testData4);
	cout << "===  total loop times = " << loopTimes << endl;
	print(testData4);

	cout << "=============== HeapAscending sorting =================\n";
	vector<int> testData5 = { 49, 38, 65, 97, 76, 13, 27, 49 };
	loopTimes = Sort::HeapAscending(testData5);
	return 0;
}