#ifndef PARSER_H
#define PARSER_H

#include <set>

std::vector<std::string> parseString(std::string str);

std::string parseTitle(std::string str);

std::set<std::string> parseLinks(std::string str);

std::string parseContent(std::string link);

#endif