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

void prepareToUpdateDict(std::vector<std::string> &vt, int linkID) {
    sort(vt.begin(), vt.end());
    vt.push_back("~_~");
    int cur = 1;
    for (int i = 1; i < vt.size(); ++i) {
        if (vt[i] != vt[i-1]) {
            updateDict(vt[i-1], linkID, cur);
            cur = 1;
        } else {
            ++cur;
        }
    }
}

void simplifyWord(std::string &str) {
    trim(str);
    if (str.length() <= 1 || str.length() >= 15) return;

    for (int j = 0; j < str.length(); ++j) {
        if (str[j] >= 65 && str[j] <= 90) {
            str[j] = int(str[j]) + 32;
        } else if (str[j] < 97 || str[j] > 122) {
            return;
        }

        if (j >= 2 && str[j] == str[j-1] && str[j] == str[j-2]) {
            return;
        }
    }

    str = stem(str);
    //if (willStore) validateWord(str, linkID);
}

std::vector<std::string> splitWords(std::string str) {
    std::vector<std::string> res;
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(res));

    for (int i = 0; i < res.size(); ++i) {
        simplifyWord(res[i]);
    }

    return res;
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
    
    for (int i = 0; i < rres.size(); ++i) {
        simplifyWord(rres[i]);
    }

    prepareToUpdateDict(rres, linkID);

}
