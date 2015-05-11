#ifndef JEP_UTIL_H
#define JEP_UTIL_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
using std::string;
using std::vector;
using std::list;
using std::map;
using std::cout;
using std::endl;
using std::pair;

enum month { JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12 , UNKNOWN_MONTH = -1};
enum day_name { SUN = 0, MON = 1, TUE = 2, WED = 3, THU = 4, FRI = 5, SAT = 6, UNKNOWN_DAY = -1};

namespace jep
{
	const bool stringInVector(string s, const vector<string> &vec);
	const bool stringInList(string s, const list<string> &str);
	const bool floatsAreEqual(float first, float second);
	const pair<float, float> calculateLineFormula(pair<float, float> p1, pair<float, float> p2);

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

	class date
	{
	public:
		date(int y, int m, int d);
		date(string s);
		date() { setData(0, 1, 1); }
		~date(){};

		unsigned short getYear() const { return d_year; }
		month getMonth() const { return d_month; }
		unsigned short getDayOfMonth() const { return day_of_month; }
		unsigned short getDayOfYear() const { return day_of_year; }
		unsigned short getDaysThisYear() const { return days_this_year; }
		day_name getDayOfWeek() const { return day_of_week; }
		bool isLeapYear() const { return leap_year; }

		string getMonthString() const;
		string getDayString() const;

		void setDayOfMonth(int d) { setData(d_year, d_month, d); }
		void setMonth(month m) { setData(d_year, m, day_of_month); }

		const date& operator ++ (int);
		const date& operator += (int n);
		bool operator == (const date &other) const
		{
			return (other.getDayOfYear() == day_of_year && other.getYear() == d_year);
		}	

		bool operator != (const date &other) const
		{
			return (other.getDayOfYear() != day_of_year || other.getYear() != d_year);
		}
		bool operator > (const date &other) const;
		bool operator < (const date &other) const;

	private:
		unsigned short d_year;
		month d_month;
		unsigned short day_of_month;
		unsigned short day_of_year;
		day_name day_of_week;
		bool leap_year;
		unsigned short days_this_year;

		day_name calcDayName(short y, month m, short d, bool leap) const;
		bool calcLeapYear(short y) const;
		bool setData(int y, int m, int d);

		map<month, int> days_per_month;
	};

	class csv_file
	{
	public: 
		csv_file(const char* file_path);
		~csv_file(){};
		vector<string> getRow(int index) const { return row_data.at(index); }
		vector<string >getColumn(int index) const { return column_data.at(index); }
		vector<string> getRow(string r_header) const { return getRow(lookupRowIndexByHeader(r_header)); }
		vector<string> getColumn(string c_header) const { return getColumn(lookupColumnIndexByHeader(c_header)); }

		vector< std::pair<string, string> > getRowDataWithHeader(int index) const;
		vector< std::pair<string, string> > getColumnDataWithHeader(int index) const;
		vector< std::pair<string, string> > getRowDataWithHeader(string r_header) const 
			{ return getRowDataWithHeader(lookupRowIndexByHeader(r_header)); }
		vector< std::pair<string, string> > getColumnDataWithHeader(string c_header) const 
			{ return getColumnDataWithHeader(lookupColumnIndexByHeader(c_header)); }

		int getRowCount() const { return row_count; }
		int getColumnCount() const { return column_count; }

		int lookupRowIndexByHeader(string header) const;
		int lookupColumnIndexByHeader(string header) const;

		string getCell(int x, int y) const { return column_data.at(x).at(y); }

	private:
		int column_count;
		int row_count;	
		map<unsigned, string> column_headers;
		map<unsigned, string> row_headers;

		map< unsigned, vector<string> > row_data;
		map< unsigned, vector<string> > column_data;

		vector<string> getRowCells(string line);
	};
}

#endif