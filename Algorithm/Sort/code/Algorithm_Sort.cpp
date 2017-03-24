//#include <algorithm>
#include "Algorithm_Sort.h"

/************************************************************************/
/*			��������
��һ����¼���뵽������õ�������У��Ӷ��õ�һ���£���¼����1�������
�����Ƚ����еĵ�1����¼������һ������������У�Ȼ��ӵ�2����¼������в��룬
ֱ��������������Ϊֹ��

Ҫ�㣺�����ڱ�����Ϊ��ʱ�洢���ж�����߽�֮�á�

�������һ���Ͳ���Ԫ����ȵģ���ô����Ԫ�ذ�������Ԫ�ط������Ԫ�صĺ��档
���ԣ����Ԫ�ص�ǰ��˳��û�иı䣬��ԭ�������г�ȥ��˳������ź�����˳��
���Բ����������ȶ��ġ�

Ч�ʣ�	O(n^2)
*/
/************************************************************************/
// ����
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

// ��������
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
/*   ���� ϣ������   (��С��������)                                                             
	����˼��:
	�Ƚ�����������ļ�¼���зָ��Ϊ���������зֱ����ֱ�Ӳ������򣬴����������еļ�¼
	����������ʱ���ٶ�ȫ���¼��������ֱ�Ӳ�������

	����������
		ѡ��һ����������t1��t2������tk������ti>tj��tk=1��
		���������и���k�������н���k ������
		ÿ�����򣬸��ݶ�Ӧ������ti�����������зָ�����ɳ���Ϊm �������У��ֱ�Ը��ӱ����
		ֱ�Ӳ������򡣽���������Ϊ1 ʱ������������Ϊһ�������������ȼ�Ϊ�������еĳ��ȡ�

	�㷨ʵ�֣�
		���Ǽ򵥴����������У���������d = {n/2 ,n/4, n/8 .....1} nΪҪ�������ĸ���
		�����Ƚ�Ҫ�����һ���¼��ĳ������d��n/2,nΪҪ�������ĸ������ֳ������������У�ÿ����
		��¼���±����d.��ÿ����ȫ��Ԫ�ؽ���ֱ�Ӳ�������Ȼ������һ����С��������d/2������
		���з��飬��ÿ�����ٽ���ֱ�Ӳ������򡣼���������С����ֱ��Ϊ1�����ʹ��ֱ�Ӳ��������������


		ϣ������ʱЧ�������ѣ��ؼ���ıȽϴ������¼�ƶ�����������������������d��ѡȡ��
		�ض�����¿���׼ȷ������ؼ���ıȽϴ����ͼ�¼���ƶ�������Ŀǰ��û���˸���ѡȡ
		��õ������������еķ����������������п����и���ȡ������ȡ�����ģ�Ҳ��ȡ�����ģ�
		����Ҫע�⣺���������г�1 ��û�й����ӣ������һ���������ӱ���Ϊ1��ϣ�����򷽷�
		��һ�����ȶ������򷽷���

		�ѵ㣺
			�������е�ѡ��(ע���������һ��Ҫ��֤Ϊ1�������������)
*/
/************************************************************************/
//#include <iostream>
int Sort::InsertionShellAscending(std::vector<int>& data)
{
	int counter = 0;
	
	int step = 4;
	int len = (int)data.size();
	std::vector<int> deltas;
	// ȡ����������
	for (int dk = len / step; dk >= 1; dk = dk / step)
	{
		deltas.push_back(dk);
	}

	// ȷ�������������һ��Ϊ1
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
��ѡ������
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

// ��Ԫ��ѡ������
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
	������
	
*/
/************************************************************************/
#include <algorithm>
int Sort::HeapAscending(std::vector<int>& data)
{
	int counter = 0;

	std::make_heap(data.begin(), data.end());

	return counter++;
}