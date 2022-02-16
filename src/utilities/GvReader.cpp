#include "GvReader.h"

void gvReader::read(std::string filename)
{
	std::ifstream in;
	in.open(filename);
	std::string input;
	while (std::getline(in, input)) {
		lines.push_back(std::string(input));
	}
	in.close();
}

void gvReader::write(std::string filename)
{
	std::ofstream out;
	out.open(filename);
	for (std::string str : lines) {
		std::string outstr = str + '\n';
		out << outstr;
	}
	out.close();
}

void gvReader::readLinesToValues()
{
	if (lines.empty()) return;
	for (std::string str : lines) {
		std::string key;
		std::string value;
		std::stringstream split(str);
		std::getline(split, key, '=');
		std::getline(split, value, '=');
		values[key] = value;
	}
}
void gvReader::readValuesToLines()
{
	lines.clear();
	for (const auto p : values) {
		std::string out = p.first + "=" + p.second;
		lines.push_back(out);
	}
}