#include <iostream>
#include <stdio.h>
#include <vector>
#include <regex>
#include <sstream>
#include <set>
#include <iterator>
#include <fstream>

#include "parser.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

std::set<std::string> dict;

std::string parseContent(std::string link) {
    try {
        curlpp::Cleanup myCleanup;
        {
          std::ostringstream os;
          os << curlpp::options::Url(link);
          return os.str();
        }
    }
    catch( curlpp::RuntimeError &e ) {}
    catch( curlpp::LogicError &e ) {}

    return "";
}

std::set<std::string> parseLinks(std::string str) {
    static const std::regex hl_regex( "<a href=\"(.*?)\"", std::regex_constants::icase  ) ;

    return { 
        std::sregex_token_iterator( str.begin(), str.end(), hl_regex, 1 ),
        std::sregex_token_iterator{} 
    };
}

std::string parseTitle(std::string str) {
    static const std::regex hl_regex( "<title>(.*?)</title>", std::regex_constants::icase  ) ;
    std::smatch match;

    if ( regex_search(str, match, hl_regex) ) {
        return match[0];
    } else {
        return "ERROR";
    }
}

bool isLetter(char chr) {
    if (chr >= 65 && chr <= 90) return true;
    if (chr >= 97 && chr <= 122) return true;
    return false;
}

void updateDict(std::string s, bool kt) {
    if (kt == false || s.length() == 1) return;

    std::string ending = std::string() + s[s.length()-2] + s[s.length()-1];
    if (ending == "er" || ending == "es" || ending == "ed") {
        s.erase(s.length()-2, 2);
    } else if (s[s.length()-1] == 's') {
        s.erase(s.length()-1, 1);
    } else if (s.length() >= 3) {
        ending = s[s.length()-3] + ending;
        if (ending == "ing") {
            s.erase(s.length()-3, 3);
        }
    }

    if (dict.find(s) == dict.end() && s != "") {
        dict.insert(s);
    }
}

void trim(std::vector<std::string> &words) {
    for (int i = 0; i < words.size(); ++i) {
        bool kt = true;

        if (words[i].length() >= 15) 
            kt = false;    
        else {
            if (words[i] == "") continue;
            if (!isLetter(words[i][0])) words[i].erase(0, 1);

            if (words[i] == "") continue;
            if (!isLetter(words[i][words[i].length()-1])) words[i].erase(words[i].length()-1, 1);

            for (int j = 0; j < words[i].length(); ++j) {
                char chr = words[i][j];

                if (chr >= 65 && chr <= 90) {
                    words[i][j] = int(chr) + 32;
                } else if (chr < 97 || chr > 122) {
                    kt = false;
                    break;
                }
            }
        }

        updateDict(words[i], kt);
    }
}

void parseString(std::string str) {
    static const std::regex hl_regex( ">(.*?)<", std::regex_constants::icase );
    std::set<std::string> res = { 
        std::sregex_token_iterator( str.begin(), str.end(), hl_regex, 1 ),
        std::sregex_token_iterator{} 
    };

    std::set<std::string>::iterator itr;
    std::vector<std::string> rres;
    for (itr = res.begin(); itr != res.end(); ++itr) {
        std::istringstream iss(*itr);
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(rres));
    }
    trim(rres);
}
