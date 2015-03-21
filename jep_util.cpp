#include "jep_util.h"

namespace jep
{
	const bool stringInVector(string s, const vector<string> &vec)
	{
		return std::find(vec.begin(), vec.end(), s) != vec.end();
	}

	const bool stringInList(string s, const list<string> &str)
	{
		return std::find(str.begin(), str.end(), s) != str.end();
	}

	const bool floatsAreEqual(float first, float second)
	{
		return abs(first - second) < .00000001f;
	}
}