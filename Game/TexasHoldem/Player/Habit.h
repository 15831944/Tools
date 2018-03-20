#pragma once

namespace Analyze{
	class HabitInfo;
	class Habit
	{
	public:
		// map of win rate with bet habit
		std::map<double, std::shared_ptr<HabitInfo>> m_mpHabitInfo;

	public:
		Habit();
		~Habit();
	};
}
