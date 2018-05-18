#pragma once

namespace PID{
	class CPid
	{
	private:
		double _inputspeed;
		double _actualSpeed;
		double _detlaSpeed;
		double _lastDetlaSpeed;
		double _kp;
		double _ki;
		double _kd;
		double _voltage;
		double _integral;

	public:
		CPid();
		double Realize(double speed);
	};
}
