#pragma once

namespace LeastSquare{
	// 最小二乘法
	class CLeastSquare
	{
	public:
		CLeastSquare(){ ; }
		static void CalEResult(std::vector<double> arrX, std::vector<double> arrY, int ecount, std::vector<double>& result);
	};
}
