#pragma once

namespace LeastSquare{
	// ��С���˷�
	class CLeastSquare
	{
	public:
		CLeastSquare(){ ; }
		static void CalEResult(std::vector<double> arrX, std::vector<double> arrY, int ecount, std::vector<double>& result);
	};
}
