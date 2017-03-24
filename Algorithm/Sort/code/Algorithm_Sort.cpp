//#include <algorithm>
#include "Algorithm_Sort.h"

/************************************************************************/
/*			插入排序
将一个记录插入到已排序好的有序表中，从而得到一个新，记录数增1的有序表。
即：先将序列的第1个记录看成是一个有序的子序列，然后从第2个记录逐个进行插入，
直至整个序列有序为止。

要点：设立哨兵，作为临时存储和判断数组边界之用。

如果碰见一个和插入元素相等的，那么插入元素把想插入的元素放在相等元素的后面。
所以，相等元素的前后顺序没有改变，从原无序序列出去的顺序就是排好序后的顺序，
所以插入排序是稳定的。

效率：	O(n^2)
*/
/************************************************************************/
// 升序
int Sort::InsertionSortAscending(std::vector<int>& data)
{
	int counter = 0;

	int len = (int)data.size();
	for (int i = 1; i < len; ++i)
	{
		for (int j = i - 1; j >= 0 && data[j] > data[j + 1]; --j)
		{
			std::swap(data[j], data[j + 1]);
#if STATISTICS_LOOP_TIMES
			counter++;
#endif
		}
	}

	return counter;
}

// 降序排列
int Sort::InsertionSortDescending(std::vector<int>& data)
{
	int counter = 0;

	for (size_t i = 1; i < data.size(); ++i)
	{
		if (data[i] > data[i - 1])
		{
			int iGard = data[i];
			int j = i - 1;
			data[i] = data[i - 1];

			while (j > 0 && data[j - 1] < iGard)
			{
				data[j] = data[j - 1];
				--j;
#if STATISTICS_LOOP_TIMES
				counter++;
#endif
			}
			data[j] = iGard;
		}
#if STATISTICS_LOOP_TIMES
		else
		{
			counter++;
		}
#endif
	}

	return counter;
}

//====================================================
/************************************************************************/
/*   插入 希尔排序   (缩小增量排序)                                                             
	基本思想:
	先将整个待排序的记录序列分割成为若干子序列分别进行直接插入排序，待整个序列中的记录
	“基本有序”时，再对全体记录进行依次直接插入排序。

	操作方法：
		选择一个增量序列t1，t2，…，tk，其中ti>tj，tk=1；
		按增量序列个数k，对序列进行k 趟排序；
		每趟排序，根据对应的增量ti，将待排序列分割成若干长度为m 的子序列，分别对各子表进行
		直接插入排序。仅增量因子为1 时，整个序列作为一个表来处理，表长度即为整个序列的长度。

	算法实现：
		我们简单处理增量序列：增量序列d = {n/2 ,n/4, n/8 .....1} n为要排序数的个数
		即：先将要排序的一组记录按某个增量d（n/2,n为要排序数的个数）分成若干组子序列，每组中
		记录的下标相差d.对每组中全部元素进行直接插入排序，然后再用一个较小的增量（d/2）对它
		进行分组，在每组中再进行直接插入排序。继续不断缩小增量直至为1，最后使用直接插入排序完成排序。


		希尔排序时效分析很难，关键码的比较次数与记录移动次数依赖于增量因子序列d的选取，
		特定情况下可以准确估算出关键码的比较次数和记录的移动次数。目前还没有人给出选取
		最好的增量因子序列的方法。增量因子序列可以有各种取法，有取奇数的，也有取质数的，
		但需要注意：增量因子中除1 外没有公因子，且最后一个增量因子必须为1。希尔排序方法
		是一个不稳定的排序方法。

		难点：
			增量序列的选择。(注意序列最后一项要保证为1，否则排序错误)
*/
/************************************************************************/
//#include <iostream>
int Sort::InsertionShellAscending(std::vector<int>& data)
{
	int counter = 0;
	
	int step = 4;
	int len = (int)data.size();
	std::vector<int> deltas;
	// 取得增量序列
	for (int dk = len / step; dk >= 1; dk = dk / step)
	{
		deltas.push_back(dk);
	}

	// 确保增量序列最后一项为1
	if (deltas.back() != 1)
	{
		deltas.push_back(1);
	}

	for (size_t m = 0; m < deltas.size(); ++m)
	{
		int dk = deltas[m];
		for (int i = dk; i < len	; ++i)
		{
			for (int j = i - dk; j >=0 && data[j] > data[j + dk]; j -= dk)
			{
				std::swap(data[j], data[j + dk]);
#if STATISTICS_LOOP_TIMES
				counter++;
#endif
			}
		}
	}

	return counter++;
}

/************************************************************************/
/* 
简单选择排序
*/
/************************************************************************/
int Sort::SimpleSelectionAscending(std::vector<int>& data)
{
	if (data.size() < 2)
	{
		return 0;
	}
	int counter = 0;

	for (size_t i = 0; i < data.size(); ++i)
	{
		int idx = i ;
		for (int j = i + 1; j < data.size(); ++j)
		{
			if (data[j] < data[idx])
			{
				idx = j;
			}

#if STATISTICS_LOOP_TIMES
			counter++;
#endif
		}
		if (idx != i)
		{
			std::swap(data[idx], data[i]);
		}
	}

	return counter;
}

// 二元简单选择排序
int Sort::SimpleSelectionAscendingEx(std::vector<int>& data)
{
	if (data.size() < 2)
	{
		return 0;
	}
	int counter = 0;
	int min, max;

	for (int i = 0; i <= data.size() / 2; ++i)
	{
		min = max = i;
		for (int j = i + 1; j < data.size() - i; ++j)
		{
#if STATISTICS_LOOP_TIMES
			counter++;
#endif
			if (data[j] > data[max])
			{
				max = j;
				continue;
			}
			if (data[j] < data[min	])
			{
				min = j;
			}
		}

		if (min	!= i)
		{
			std::swap(data[i], data[min]);
		}
		if (max == i) // note 
		{
			max = min;
		}
		std::swap(data[data.size() - i - 1], data[max]);
	}

	return counter;
}

/************************************************************************/
/* 
	堆排序
	
*/
/************************************************************************/
#include <algorithm>
int Sort::HeapAscending(std::vector<int>& data)
{
	int counter = 0;

	std::make_heap(data.begin(), data.end());

	return counter++;
}