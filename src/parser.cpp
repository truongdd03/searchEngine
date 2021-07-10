#include <iostream>
#include <stdio.h>
#include <vector>
#include <regex>
#include <sstream>
#include <set>

#include "parser.h"

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
    catch( curlpp::RuntimeError &e ) {
        //cout << "LINK: " << link << " ERROR: " << e.what() << "\n\n";
    }

    catch( curlpp::LogicError &e ) {
        //cout << "LINK: " << link << " ERROR: " << e.what() << "\n\n";
    }

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

std::vector<std::string> parseString(std::string str) {
    std::vector<std::string> res;


    return res;
}
