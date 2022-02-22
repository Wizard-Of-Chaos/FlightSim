#pragma once
#ifndef GVREADER_H
#define GVREADER_H
#include "BaseHeader.h"

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

struct gvReader
{
	//Lines of a loaded file.
	std::vector<std::string> lines;
	//Map of values.
	std::map<std::string, std::string> values;

	//Reads the lines currently loaded to the map.
	void readLinesToValues();
	//Reads the map back out to lines of text.
	void readValuesToLines();
	//Reads in lines from a file.
	void read(std::string filename);
	//Writes lines to a file.
	void write(std::string filename);
};

#endif 