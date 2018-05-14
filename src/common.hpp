#pragma once

#include <string>
#include <sstream>

namespace common {

class Converter
{
public:
	template <typename ValueType>
	static std::string toString(const ValueType& value)
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
};

struct CodeLocation
{
	CodeLocation(const std::string& a_file, int a_line)
		: file(a_file)
		, line(a_line)
	{}

	std::string file;
	int line;
};

} // namespace common
