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

	date::date(int y, int m, int d)
	{
		setData(y, m, d);
	}

	date::date(string s)
	{
		//dates must be in yyyy/mm/dd or yyyymmdd formats
		s.erase(std::remove_if(s.begin(), s.end(), [](const char& c) {return c == '/'; }));

		string s_year = s.substr(0, 4);
		string s_month = s.substr(4, 2);
		string s_day = s.substr(6, 2);

		int year = std::stoi(s_year);
		int month = std::stoi(s_month);
		int day = std::stoi(s_day);

		setData(year, month, day);
	}

	bool date::setData(int y, int m, int d)
	{
		leap_year = calcLeapYear(y);
		days_this_year = (leap_year ? 366 : 365);
		d_year = y;

		if (m == -1)
		{
			d_month = UNKNOWN_MONTH;
			day_of_year = 0;
			day_of_month = 0;
			day_of_week = UNKNOWN_DAY;
			return true;
		}

		else if (m < 1 || m > 12)
			return false;

		else d_month = (month)m;

		if (d == -1)
		{
			day_of_year = 0;
			day_of_month = 0;
			day_of_week = UNKNOWN_DAY;
			return true;
		}

		days_per_month[JAN] = 31;
		days_per_month[FEB] = (leap_year ? 29 : 28);
		days_per_month[MAR] = 31;
		days_per_month[APR] = 30;
		days_per_month[MAY] = 31;
		days_per_month[JUN] = 30;
		days_per_month[JUL] = 31;
		days_per_month[AUG] = 31;
		days_per_month[SEP] = 30;
		days_per_month[OCT] = 31;
		days_per_month[NOV] = 30;
		days_per_month[DEC] = 31;

		if (d > days_per_month.at(d_month) || d < 1)
			return false;

		else day_of_month = d;

		day_of_year = 0;
		for (int i = 1; i < 13; i++)
		{
			if ((int)d_month == i)
			{
				day_of_year += day_of_month;
				break;
			}

			else day_of_year += days_per_month.at((month)i);
		}

		//month_value is based on months table: https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week#Months_table
		unsigned short month_value = 0;

		switch (d_month)
		{
		case(JAN) :
			month_value = (leap_year ? 6 : 0);
			break;

		case(FEB) :
			month_value = (leap_year ? 2 : 3);
			break;

		case(MAR) :
			month_value = 3;
			break;

		case(APR) :
			month_value = 6;
			break;

		case(MAY) :
			month_value = 1;
			break;

		case(JUN) :
			month_value = 4;
			break;

		case(JUL) :
			month_value = 6;
			break;

		case(AUG) :
			month_value = 2;
			break;

		case(SEP) :
			month_value = 5;
			break;

		case(OCT) :
			month_value = 0;
			break;

		case(NOV) :
			month_value = 3;
			break;

		case(DEC) :
			month_value = 5;
			break;

		default: return false;
		}

		//below formula based on: https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
		short year_suffix = d_year % 100;
		short year_prefix = d_year / 100;

		int c = 6 - 2 * (year_prefix % 4);

		day_of_week = (day_name)((day_of_month + month_value + year_suffix + (year_suffix / 4) + c) % 7);

		return true;
	}

	string date::getMonthString() const
	{
		switch (d_month)
		{
		case(JAN) : return "January";
		case(FEB) : return "February";
		case(MAR) : return "March";
		case(APR) : return "April";
		case(MAY) : return "May";
		case(JUN) : return "June";
		case(JUL) : return "July";
		case(AUG) : return "August";
		case(SEP) : return "September";
		case(OCT) : return "October";
		case(NOV) : return "November";
		case(DEC) : return "December";
		case(UNKNOWN_MONTH) : return "Unknown";
		default: return "UNDEFINED";
		}
	}

	string date::getDayString() const
	{
		switch (day_of_week)
		{
		case(SUN) : return "Sunday";
		case(MON) : return "Monday";
		case(TUE) : return "Tuesday";
		case(WED) : return "Wednesday";
		case(THU) : return "Thursday";
		case(FRI) : return "Friday";
		case(SAT) : return "Saturday";
		case(UNKNOWN_DAY) : return "Unknown";
		default: return "UNDEFINED";
		}
	}

	const date& date::operator ++ (int)
	{
		if (day_of_week == UNKNOWN_DAY)
			return *this;

		if (day_of_week == SAT)
			day_of_week = SUN;

		else day_of_week = day_name((int)day_of_week + 1);

		if (day_of_year == days_this_year)
		{
			d_year++;
			day_of_year = 1;

			leap_year = calcLeapYear(d_year);
			days_this_year = (leap_year ? 366 : 365);
			d_month = JAN;
			day_of_month = 1;
		}

		else
		{
			if (day_of_month == days_per_month.at(d_month))
			{
				d_month = month((int)d_month + 1);
				day_of_month = 1;
			}

			else day_of_month++;

			day_of_year++;
		}	

		return *this;
	}

	const date& date::operator += (int n)
	{
		if (day_of_month == UNKNOWN_DAY)
			return *this;

		for (int i = 0; i < n; i++)
			(*this)++;

		return *this;
	}

	bool date::calcLeapYear(short y) const
	{
		//calc based on: https://support.microsoft.com/en-us/kb/214019
		if (y % 4 == 0)
		{
			if (y % 100 == 0)
				return (y % 400 == 0);

			else return true;
		}

		else return false;
	}

	csv_file::csv_file(const char* file_path)
	{
		std::fstream file;
		file.open(file_path, std::ifstream::in);

		if (!file.is_open())
		{
			string error = "unable to open csv file: ";
			error += file_path;
			std::cout << error << std::endl;
		}

		bool first_line = true;
		int row_counter = 0;

		while (!file.eof())
		{
			string line;
			std::getline(file, line, '\n');

			vector<string> cells(getRowCells(line));

			if (first_line && !cells.empty())
			{
				for (int i = 0; i < cells.size(); i++)
					column_headers[i] = cells.at(i);

				first_line = false;
				column_count = cells.size();
			}

			//add cell data to respective column
			for (int i = 0; i < cells.size(); i++)
				column_data[i].push_back(cells.at(i));

			if (!cells.empty())
			{
				row_data[row_counter] = cells;
				row_headers[row_counter++] = cells.front();
			}
		}

		row_count = row_counter;

		file.close();
	}

	vector<string> csv_file::getRowCells(string line)
	{
		vector<string> row_cells;
		bool in_quotes = false;
		string current_cell = "";
		for (int i = 0; i < line.size(); i++)
		{
			char c = line[i];
			if (c == '\"')
			{
				in_quotes = !in_quotes;
				continue;
			}

			bool add = !(c == ',' && !in_quotes);
			bool end = (c == ',' && !in_quotes) || (i == line.size() - 1);

			if (add)
				current_cell += c;

			if (end)
			{
				row_cells.push_back(current_cell);
				current_cell = "";
			}
		}

		return row_cells;
	}

	int csv_file::lookupRowIndexByHeader(string header) const
	{
		for (auto i : row_headers)
		{
			if (i.second == header)
				return i.first;
		}
	}

	int csv_file::lookupColumnIndexByHeader(string header) const
	{
		for (auto i : column_headers)
		{
			if (i.second == header)
				return i.first;
		}
	}

	vector< std::pair<string, string> > csv_file::getRowDataWithHeader(int index) const
	{
		vector<string> row = row_data.at(index);
		vector< std::pair<string, string> > data;

		//field data with corresponding header tag
		for (int i = 0; i < row.size(); i++)
			data.push_back(std::pair<string, string>(column_headers.at(i), row.at(i)));

		return data;
	}

	vector< std::pair<string, string> > csv_file::getColumnDataWithHeader(int index) const
	{
		vector<string> column = column_data.at(index);
		vector< std::pair<string, string> > data;

		//field data with corresponding header tag
		for (int i = 0; i < column.size(); i++)
			data.push_back(std::pair<string, string>(row_headers.at(i), column.at(i)));

		return data;
	}
}