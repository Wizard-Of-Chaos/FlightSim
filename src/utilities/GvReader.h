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
		std::vector<std::string> lines;
		std::map<std::string, std::string> values;

		void readLinesToValues();
		void readValuesToLines();
		void read(std::string filename);
		void write(std::string filename);
};

#endif 