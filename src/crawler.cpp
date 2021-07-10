#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <sstream>
#include <regex>
#include <thread>
#include <mutex>

#include "parser.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

const std::string src = "https://en.wikipedia.org/";
std::vector <std::thread> threads;
std::queue <std::string> q;
std::mutex myMutex;
std::set <std::string> links, titles;

std::string reformat(std::string link) {
    if (link[0] == '/') return src + link;
    if (link.rfind(src) != 0) return "";
    return link;
}

std::string extractContent(std::string link) {

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

std::set<std::string> extractLinks(std::string str) {
    static const std::regex hl_regex( "<a href=\"(.*?)\"", std::regex_constants::icase  ) ;

    return { 
        std::sregex_token_iterator( str.begin(), str.end(), hl_regex, 1 ),
        std::sregex_token_iterator{} 
    };
}

std::string extractTitle(std::string str) {
    static const std::regex hl_regex( "<title>(.*?)</title>", std::regex_constants::icase  ) ;
    std::smatch match;

    if ( regex_search(str, match, hl_regex) ) {
        return match[0];
    } else {
        return "ERROR";
    }
}

bool isExisted(std::string content, std::string link) {
    std::string title = extractTitle(content);
    
    if (titles.find(title) != titles.end()) return true; 

    if (links.find(link) != links.end() || link == "") return true; 

    titles.insert(title);
    links.insert(link);
    return false;
}

int siz = 1;

void crawl(std::string link) {
    std::string content = extractContent(link);

    if (isExisted(content, link)) {
        //cout << "EXISTED " << link << "\n\n";
        return;
    }

    std::cout << "NEW #" << siz++ << " " << link << "\n\n";

    std::set<std::string> linksOfPage = extractLinks(content);
    std::set<std::string>::iterator itr;

    for (itr = linksOfPage.begin(); itr != linksOfPage.end(); itr++) {
        myMutex.lock();
        std::string str = reformat(*itr);
        if (str != "") q.push(str);
        myMutex.unlock();
    }

}

void process() {
    while (true) {

        myMutex.lock();
                
        if (q.empty()) {
            std::cout << "########################EXIT THREAD#########################";
            std::terminate();
            return;
        }
        
        std::string link = q.front();
        q.pop();
        myMutex.unlock();

        crawl(link);
        //cout << "YEAh\n";

    }
}

void run() {
    int numberOfThreads = 0;
    std::cin >> numberOfThreads;
    for (int i = 0; i < numberOfThreads; ++i) {
        threads.push_back(std::thread(process));
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}
int main() {
    curlpp::Cleanup myCleanup;

    crawl("https://en.wikipedia.org/wiki/Main_Page");
    //run();

    parseString("HI HOW ARE U");

    return 0;
}