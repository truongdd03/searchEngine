#ifndef PARSER_H
#define PARSER_H

#include <set>

extern std::set<std::string> dict;

void parseString(std::string str);

std::string parseTitle(std::string str);

std::set<std::string> parseLinks(std::string str);

std::string parseContent(std::string link);

#endif