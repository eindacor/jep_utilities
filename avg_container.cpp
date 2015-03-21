#include "avg_container.h"

namespace jep
{
	const float avg_container::combineAverages
		(float previous_avg, int previous_count, float added_avg, int added_count) const
	{
		if (added_count < 1){
			string error_string = "combineAverages cannot operate with a count less than 1";
			throw error_string;
		}
		int new_count = previous_count + added_count;
		float new_total = (previous_avg * float(previous_count)) + (added_avg * float(added_count));
		return float(new_total / float(new_count));
	}
}