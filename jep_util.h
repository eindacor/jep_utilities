#ifndef JEP_UTIL_H
#define JEP_UTIL_H

#include <string>
#include <vector>
#include <list>
#include <map>
using std::string;
using std::vector;
using std::list;
using std::map;

namespace jep
{
	const bool stringInVector(string s, const vector<string> &vec);
	const bool stringInList(string s, const list<string> &str);
	const bool floatsAreEqual(float first, float second);

	template<typename t>
	void removeFromVector(vector<t> &vec, t target)
	{
		vector<t>::iterator it = std::find(vec.begin(), vec.end(), target);
		if (it != vec.end())
			vec.erase(it);
	}

	template<typename t1, typename t2>
	void removeFromMap(map<t1, t2> &m, t1 target)
	{
		m.erase(it);
	}

	class avg_container;
}

#endif