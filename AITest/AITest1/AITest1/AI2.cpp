#include "stdafx.h"
#include "AI2.h"

using namespace AI2;
std::vector<std::vector<double>> _eMatrix;

//ecount�η���
double RelatePow(std::vector<double> arrX, int ecount)
{
	double sum = 0;
	int n = (int)arrX.size();
	for (int i = 0; i < n; i++)
	{
		// ����arrX[i]��ecount�η�
		sum += pow(arrX[i], ecount);
	}
	return sum;
}

//x��ecount�η���y�ĳ˻����ۼ�  
double RelateMutiXY(std::vector<double> arrX, std::vector<double> arrY, int ecount)
{
	double dReMultiSum = 0;
	int n = (int)arrX.size();
	for (int i = 0; i < n; i++)
	{
		dReMultiSum += pow(arrX[i], ecount) * arrY[i];
	}
	return dReMultiSum;
}

//��ⷽ��  
void CalEquation(int ecount, std::vector<double>& result)
{
	for (int k = 1; k < ecount; k++) //��Ԫ����  
	{
		for (int i = k + 1; i < ecount + 1; i++)
		{
			double p1 = 0;

			if (_eMatrix[k][k] != 0)
				p1 = _eMatrix[i][k] / _eMatrix[k][k];

			for (int j = k; j<ecount + 2; j++)
				_eMatrix[i][j] = _eMatrix[i][j] - _eMatrix[k][j] * p1;
		}
	}
	result[ecount] = _eMatrix[ecount][ecount + 1] / _eMatrix[ecount][ecount];

	auto Y = [](std::vector<double>& result, int l, int ecount) -> double {
		double s = 0;
		for (int i = l; i <= ecount; i++) {
			s += _eMatrix[l - 1][i] * result[i];
		}
		return s;
	};
	for (int l = ecount - 1; l >= 1; l--){   //�ش����  
		result[l] = (_eMatrix[l][ecount + 1] - Y(result, l + 1, ecount)) / _eMatrix[l][l];
	}
}

//���㷽������������  
void AIMgr::CalEResult(std::vector<double> arrX, std::vector<double> arrY, int ecount, std::vector<double>& result)
{
	// ��ʼ�� _eMatrix
	_eMatrix.resize(ecount + 1);
	for (int i = 0; i < ecount + 1; i++) _eMatrix[i].resize(ecount + 2);

	// 
	for (int i = 1; i <= ecount; i++)
	{
		for (int j = 1; j <= ecount; j++)
		{
			_eMatrix[i][j] = RelatePow(arrX, i + j - 2);
		}
		_eMatrix[i][ecount + 1] = RelateMutiXY(arrX, arrY, i - 1);
	}
	_eMatrix[1][1] = (int)arrX.size();
	CalEquation(ecount, result);
}

