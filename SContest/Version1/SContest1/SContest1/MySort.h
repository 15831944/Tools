#pragma once

#include <stdlib.h>
#include <algorithm>
#include "Gbl.h"
#include "MyThreadPool.h"
#include <memory>

std::shared_ptr<TaskExecutor> g_exe;

int cmpInt(const void *l, const void *r)
{
	return (*(int *)l - *(int *)r);
}

int cmpDouble(const void *l, const void *r)
{
	return((*(double*)l - *(double*)r > 0.0) ? 1 : -1);
}

#define MySwap(a, b) \
	a = a ^ b; \
	b = a ^ b; \
	a = a ^ b;


#define Mid(a, b, c) \
	a > b ? (b > c ? b : (a > c ? c : a)) : (a > c ? a : (b > c ? c : b));

//template<typename T>
//void QuickSort1(T* arr, int l, int r)
void QuickSort1(int* arr, int l, int r)
{
	int i = l, j = r, x = arr[l];
	while (i < j) {
		while ((arr[j] >= x) && (i < j))	--j;
		arr[i] = arr[j];
		while ((arr[i] <= x) && (i < j))	++i;
		arr[j] = arr[i];
	}
	arr[i] = x;
	if (l < (i - 1)) QuickSort1(arr, l, i - 1);
	if ((i + 1) < r) QuickSort1(arr, i + 1, r);
}

void QuickSort2(int* arrI, int* arrJ)
{
	int *arrL = arrI, *arrR = arrJ, x = *arrI;
	while (arrI < arrJ) {
		while (*arrJ >= x && arrI < arrJ) --arrJ;
		*arrI = *arrJ;
		while (*arrI <= x && arrI < arrJ) ++arrI;
		*arrJ = *arrI;
	}
	*arrI = x;
	if (arrL < --arrI) QuickSort2(arrL, arrI);
	++arrI;
	if (++arrI < arrR) QuickSort2(arrI, arrR);
}

#include <thread>
int thread_count = 0;
void QuickSort3(int* arrI, int* arrJ)
{
	int *arrL = arrI, *arrR = arrJ, x = *arrI;
	while (arrI < arrJ) {
		while (*arrJ >= x && arrI < arrJ) --arrJ;
		*arrI = *arrJ;
		while (*arrI <= x && arrI < arrJ) ++arrI;
		*arrJ = *arrI;
	}
	*arrI = x;
	thread_count += 2;
	bool bT = false;
	if (thread_count <= 16) bT = true;
	std::thread *tr1 = nullptr, *tr2 = nullptr;

	if (arrL < --arrI)
	{
		if (bT)	tr1 = new std::thread(QuickSort3, arrL, arrI);
		else	QuickSort2(arrL, arrI);
	}
	arrI += 2;
	if (arrI < arrR)
	{
		if (bT)	tr2 = new std::thread(QuickSort3, arrI, arrR);
		else	QuickSort2(arrI, arrR);
	}
	if (tr1 != nullptr) { tr1->join(); delete tr1; }
	if (tr2 != nullptr) { tr2->join(); delete tr2; }
	thread_count -= 2;
}

void QuickSort4(int* arrI, int* arrJ)
{
	int *arrL = arrI, *arrR = arrJ, x = *arrI;
	while (arrI < arrJ) {
		while (*arrJ >= x && arrI < arrJ) --arrJ;
		*arrI = *arrJ;
		while (*arrI <= x && arrI < arrJ) ++arrI;
		*arrJ = *arrI;
	}
	*arrI = x;

	if (arrL < --arrI)
	{
		g_exe->commit(QuickSort4, arrL, arrI);
	}
	arrI += 2;
	if (arrI < arrR)
	{
		g_exe->commit(QuickSort4, arrI, arrR);
	}
}


void SangSort(int *a, int lo, int hi)
{
	if (hi <= lo)    return;
	int lt = lo, i = lo + 1, gt = hi;
	int v = a[lo];
	while (i <= gt){
		//int cmp = a[i].compareTo(v);
		if (a[i] < v){
			//exch(a, lt++, i++);
			std::swap(a[lt], a[i]);
			lt++;
			i++;
		}
		else if (a[i] > v){
			//exch(a, i, gt--);
			std::swap(a[i], a[gt]);
			gt--;
		}
		else{
			i++;
		}
	}//  现在a[lo...lt-1] < v = a[lt...gt] < a[gt+1...hi]成立
	SangSort(a, lo, lt - 1);
	SangSort(a, gt + 1, hi);
}

void TestSort()
{
	g_exe = std::shared_ptr<TaskExecutor>(new TaskExecutor(16));
	//int count = 1000000;
	//int *arr = new int[count];
	int count = 50;
	int arr[50];
	double dt = 0;
	int round = 1;
	int nsize = 0;
	for (int n = 0; n < round; n++) {
		for (int i = 0; i < count; i++) {
			arr[i] = rand() % (count / 1);
		}
		thread_count = 0;
		PrintTime pt;
		//QuickSort1(arr, 0, count - 1);
		//QuickSort4(arr, &arr[count - 1]);
		g_exe->commit(QuickSort4, arr, &arr[count - 1]);
		nsize = g_exe->size();
		dt += pt.CalcTime();
	}
	Sleep(1000);
	while (!g_exe->empty())
	{
		Sleep(1);
	}
	//pt.MessageBoxTime();
	CString str;
	str.Format(_T("%f"), dt / round);
	MessageBox(NULL, str, _T("Info"), MB_OK);
	g_exe->shutdown();
	//delete[] arr;
}
