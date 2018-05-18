#include "stdafx.h"
#include "PID.h"

using namespace PID;
CPid::CPid()
	: _inputspeed(0)
	, _actualSpeed(0)
	, _detlaSpeed(0)
	, _lastDetlaSpeed(0)
	, _kp(0.1)
	, _ki(0.15)
	, _kd(0.1)
	, _voltage(0)
	, _integral(0){}

double CPid::Realize(double speed)
{
	_inputspeed = speed;
	_detlaSpeed = _inputspeed - _actualSpeed;
	_integral += _detlaSpeed;
	_voltage = _kp * _detlaSpeed + _ki * _integral + _kd * (_detlaSpeed - _lastDetlaSpeed);

	_lastDetlaSpeed = _detlaSpeed;
	_actualSpeed = _voltage;
	return _actualSpeed;
}