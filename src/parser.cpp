#include <iostream>
#include <stdio.h>
#include <vector>
#include <regex>
#include <sstream>
#include <set>
#include <iterator>
#include <fstream>
#include <mutex>

#include "parser.h"
#include "stemmer.h"
#include "storeWords.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

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

void trim(std::string &str) {
    if (str.length() >= 1 && (str[0] < 'a' || str[0] > 'z') && (str[0] < 'A' || str[0] > 'Z')) 
        str.erase(0, 1);

    int n = str.length() - 1;
    if (n >= 0 && (str[n] < 'a' || str[n] > 'z') && (str[n] < 'A' || str[n] > 'Z')) 
        str.erase(n, 1);
}

void validateWord(std::string s, int linkID) {
    if (s.length() == 1) return;

    std::string str = stem(s);

    for (int i = 0; i < str.length(); ++i) {
        if (str[i] < 'a' || str[i] > 'z') return;
    }
    if (str == "") return;

    updateDict(str, linkID);
}

void compress(std::vector<std::string> &words, int linkID) {
    for (int i = 0; i < words.size(); ++i) {
        bool kt = true;

        if (words[i].length() >= 15) 
            kt = false;    
        else {
            trim(words[i]);
            if (words[i].length() <= 1) continue;

            for (int j = 0; j < words[i].length(); ++j) {
                char chr = words[i][j];

                if (chr >= 65 && chr <= 90) {
                    words[i][j] = int(chr) + 32;
                } else if (chr < 97 || chr > 122) {
                    kt = false;
                    break;
                }

                if (j >= 2 && words[i][j] == words[i][j-1] && words[i][j] == words[i][j-2]) {
                    kt = false;
                    break;
                }
            }
        }

        if (kt) validateWord(words[i], linkID);
    }
}

void parseString(std::string str, int linkID) {
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
    compress(rres, linkID);
}
