#pragma once

namespace AI2{
	class AIMgr
	{
	public:
		AIMgr(){ ; }
		static void CalEResult(std::vector<double> arrX, std::vector<double> arrY, int ecount, std::vector<double>& result);
	};
}
