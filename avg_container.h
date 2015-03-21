#ifndef AVG_CONTAINER_H
#define AVG_CONTAINER_H

#include "jep_util.h"

namespace jep
{
	class avg_container
	{
	public:
		avg_container() : average(0.0), count(0), highest_value_passed(0.0f),
			lowest_value_passed(0.0f), value_passed(false) {};
		avg_container(float avg, int c) : average(avg), count(c) {};
		~avg_container(){};

		const float combineAverages(float previous_avg, int previous_count, float new_avg, int new_count) const;
		void addValue(float value) {
			average = combineAverages(average, count, value, 1);
			count++;
			if (value_passed)
			{
				if (value >= highest_value_passed)
					highest_value_passed = value;

				if (value <= lowest_value_passed)
					lowest_value_passed = value;
			}

			else
			{
				value_passed = true;
				highest_value_passed = value;
				lowest_value_passed = value;
			}
		}
		void addAverage(float added_avg, int added_count) {
			average = combineAverages(average, count, added_avg, added_count); count += added_count;
		}
		void operator = (const avg_container &other) {
			average = other.getAverage(); count = other.getCount();
		}

		const float getAverage() const { return average; }
		const signed long getCount() const { return count; }
		const float getHighest() const { return highest_value_passed; }
		const float getLowest() const { return lowest_value_passed; }

	private:
		bool value_passed;
		float average;
		float highest_value_passed;
		float lowest_value_passed;
		signed long count;
	};
}
#endif