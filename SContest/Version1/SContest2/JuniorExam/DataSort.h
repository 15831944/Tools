#pragma once

#include <stdlib.h>
#include <algorithm>
#include <memory>


void QuickSort(int* arrI, int* arrJ)
{
	int *arrL = arrI, *arrR = arrJ, x = *arrI;
	while (arrI < arrJ) {
		while (*arrJ >= x && arrI < arrJ) --arrJ;
		*arrI = *arrJ;
		while (*arrI <= x && arrI < arrJ) ++arrI;
		*arrJ = *arrI;
	}
	*arrI = x;
	if (arrL < --arrI) QuickSort(arrL, arrI);
	++arrI;
	if (++arrI < arrR) QuickSort(arrI, arrR);
}

#include <thread>
int thread_count = 0;
void QuickSort_Thread(int* arrI, int* arrJ)
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
		if (bT)	tr1 = new std::thread(QuickSort_Thread, arrL, arrI);
		else	QuickSort_Thread(arrL, arrI);
	}
	arrI += 2;
	if (arrI < arrR)
	{
		if (bT)	tr2 = new std::thread(QuickSort_Thread, arrI, arrR);
		else	QuickSort_Thread(arrI, arrR);
	}
	if (tr1 != nullptr) { tr1->join(); delete tr1; }
	if (tr2 != nullptr) { tr2->join(); delete tr2; }
	thread_count -= 2;
}
