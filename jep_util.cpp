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

	const pair<float, float> calculateLineFormula(const glm::vec2 &first, const glm::vec2 &second)
	{
		if (floatsAreEqual(first.y, second.y))
			return pair<float, float>(0.0f, second.y);

		float multiplier = (second.y - first.y) / (second.x - first.x);
		float y_offset = first.y - (first.x * multiplier);
		return pair<float, float>(multiplier, y_offset);
	}

	const glm::vec2 getIntersection(const pair<glm::vec2, glm::vec2> &line_one, const pair<glm::vec2, glm::vec2> &line_two)
	{
		pair<float, float> line_one_formula = calculateLineFormula(line_one.first, line_one.second);
		pair<float, float> line_two_formula = calculateLineFormula(line_two.first, line_two.second);

		if (floatsAreEqual(line_one_formula.first, line_two_formula.first))
			throw;

		float lhs = line_one_formula.first - line_two_formula.first;
		float rhs = line_two_formula.second - line_one_formula.second;

		float x = rhs / lhs;
		float y = (x * line_one_formula.first) + line_one_formula.second;
		return glm::vec2(x, y);
	}

	bool pointInPolygon(const std::vector<glm::vec2> &polygon_points, const glm::vec2 &test_point)
	{
		pair<glm::vec2, glm::vec2> horizontal_line(test_point, test_point + glm::vec2(1.0f, 0.0f));

		int intersections_left = 0;

		for (std::vector<glm::vec2>::const_iterator it = polygon_points.cbegin(); it != polygon_points.cend(); it++)
		{
			std::vector<glm::vec2>::const_iterator next;

			if (it + 1 == polygon_points.cend())
				next = polygon_points.cbegin();

			else next = it + 1;

			//skip horizontal lines, segments that don't cross the test line, and overlapping points
			if ((*it).y > test_point.y == (*next).y > test_point.y || floatsAreEqual((*it).y, (*next).y) || (*it) == (*next))
				continue;

			if (floatsAreEqual((*it).x, (*next).x))
			{
				if ((*it).x < test_point.x)
					intersections_left++;

				continue;
			}

			glm::vec2 first(*it);
			glm::vec2 second(*next);

			float delta_y(second.y - first.y);
			float delta_x(second.x - first.x);
			float slope = delta_y / delta_x;

			float new_delta_y = test_point.y - first.y;
			float new_delta_x = new_delta_y / slope;

			float intersection_x = first.x + new_delta_x;

			if (intersection_x < test_point.x)
				intersections_left++;
		}

		return intersections_left % 2 == 0;
	}

	bool pointInPolygon(const vector< pair<glm::vec3, glm::vec3> > &polygon_sides, const glm::vec2 &test_point)
	{
		pair<glm::vec2, glm::vec2> horizontal_line(test_point, test_point + glm::vec2(1.0f, 0.0f));

		int intersections_left = 0;

		for (vector< pair<glm::vec3, glm::vec3> >::const_iterator it = polygon_sides.cbegin(); it != polygon_sides.cend(); it++)
		{
			glm::vec3 first_point = (*it).first;
			glm::vec3 second_point = (*it).second;

			//skip horizontal lines, segments that don't cross the test line, and overlapping points
			if ((first_point.y > test_point.y == second_point.y > test_point.y) ||		//segment does not cross the test plane
				floatsAreEqual(first_point.y, second_point.y) ||						//segment is horizontal
				first_point == second_point)											//points are overlapping
				continue;

			//if the line is vertical, determine x intersection and continue
			if (floatsAreEqual(first_point.x, second_point.x))
			{
				if (first_point.x < test_point.x)
					intersections_left++;

				continue;
			}

			float delta_y(second_point.y - first_point.y);
			float delta_x(second_point.x - first_point.x);
			float slope = delta_y / delta_x;

			float new_delta_y = test_point.y - first_point.y;
			float new_delta_x = new_delta_y / slope;

			float intersection_x = first_point.x + new_delta_x;

			if (intersection_x < test_point.x)
				intersections_left++;
		}

		return intersections_left % 2 == 0;
	}

	date::date(int y, int m, int d)
	{
		setData(y, m, d);
	}

	date::date(string s)
	{
		if (s.length() < 8 || s.length() > 10)
		{
			setData(0, -1, -1);
			return;
		}

		//dates must be in yyyy/mm/dd or yyyymmdd formats
		s.erase(std::remove_if(s.begin(), s.end(), [](const char& c) {return c == '/'; }));

		string s_year = s.substr(0, 4);
		string s_month = s.substr(4, 2);
		string s_day = s.substr(6, 2);

		int year = std::stoi(s_year);
		int month = (s_month == "??" ? -1 : std::stoi(s_month));
		int day = (s_day == "??" ? -1 : std::stoi(s_day));

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

	bool date::operator > (const date &other) const
	{
		if (getYear() != other.getYear())
			return getYear() > other.getYear();

		if (getDayOfYear() != other.getDayOfYear())
			return getDayOfYear() > other.getDayOfYear();

		return false;
	}

	bool date::operator < (const date &other) const
	{
		if (getYear() != other.getYear())
			return getYear() < other.getYear();

		if (getDayOfYear() != other.getDayOfYear())
			return getDayOfYear() < other.getDayOfYear();

		return false;
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

	const float getLineAngle(glm::vec2 first, glm::vec2 second)
	{
		//assumes line is going from first to second
		//angle returned is counter-clockwise offset from horizontal axis to the right
		if (floatsAreEqual(first.x, second.x))
		{
			if (first.y < second.y)
				return 90.0f;

			else return 270.0f;
		}

		if (floatsAreEqual(first.y, second.y))
		{
			if (first.x < second.x)
				return 0.0f;

			else return 180.0f;
		}

		float tangent = ((second.y - first.y) / (second.x - first.x));
		float pi = 3.14159265;
		float angle = atan(abs(tangent)) * (180 / pi);

		if (tangent < 0)
		{
			if (first.x < second.x)
				return 360.0f - angle;

			else return 180.0f - angle;
		}

		else
		{
			if (first.x < second.x)
				return angle;

			else return 180.0f + angle;
		}
	}
}