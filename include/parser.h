#ifndef PARSER_H
#define PARSER_H

#include <set>
#include <vector>

void parseString(std::string str, int linkID);

std::string parseTitle(std::string str);

std::set<std::string> parseLinks(std::string str);

std::string parseContent(std::string link);

std::vector<std::string> splitWords(std::string str);

#endif