#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <sstream>
#include <regex>
#include <thread>
#include <mutex>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;

const string src = "http://www.cplusplus.com";
vector <thread> threads;
queue <string> q;
mutex myMutex;
set <string> links, titles;

string reformat(string link) {
    if (link[0] == '/') return src + link;
    return link;
}

string extractContent(string link) {

    try {
        curlpp::Cleanup myCleanup;

        {
          ostringstream os;
          os << curlpp::options::Url(link);
          return os.str();
        }

    }
    catch( curlpp::RuntimeError &e ) {
        cout << e.what() << "\n\n";
    }

    catch( curlpp::LogicError &e ) {
        cout << e.what() << "\n\n";
    }

    return "";
}

set<string> extractLinks(string str) {
    static const regex hl_regex( "<a href=\"(.*?)\"", regex_constants::icase  ) ;

    return { 
        sregex_token_iterator( str.begin(), str.end(), hl_regex, 1 ),
        sregex_token_iterator{} 
    } ;
}

string extractTitle(string str) {
    static const regex hl_regex( "<title>(.*?)</title>", regex_constants::icase  ) ;
    smatch match;

    if ( regex_search(str, match, hl_regex) ) {
        return match[0];
    } else {
        return "ERROR";
    }
}

bool isExisted(string content, string link) {
    string title = extractTitle(content);
    
    if (titles.find(title) != titles.end()) return true; 

    if (links.find(link) != links.end() || link == "") return true; 

    titles.insert(title);
    links.insert(link);
    return false;
}

int siz = 1;

void crawl(string link) {
    string content = extractContent(link);

    if (isExisted(content, link)) {
        cout << "EXISTED " << link << "\n\n";
        return;
    }

    cout << "NEW #" << siz++ << " " << link << "\n\n";

    set<string> linksOfPage = extractLinks(content);
    set<string>::iterator itr;

    for (itr = linksOfPage.begin(); itr != linksOfPage.end(); itr++)
        q.push(reformat(*itr));

}

void process() {
    while (!q.empty()) {
        myMutex.lock();
        string link = q.front();
        myMutex.unlock();

        q.pop();
        crawl(link);

    }
}
int main() {
    curlpp::Cleanup myCleanup;

    q.push("http://www.cplusplus.com");
    q.push("http://www.cplusplus.com/articles/");
    q.push("http://www.cplusplus.com/doc/");
    q.push("http://www.cplusplus.com/doc/");
    q.push("http://www.cplusplus.com/forum/");
    q.push("http://www.cplusplus.com/info/");
    q.push("http://www.cplusplus.com/articles/i86AC542/");
    q.push("http://www.cplusplus.com/articles/z1hv0pDG/");
    q.push("http://www.cplusplus.com/articles/y8vU7k9E/");
    q.push("http://www.cplusplus.com/articles/zTA0RXSz/");
    q.push("http://www.cplusplus.com/user/Viktar_Khutko/");

    for (int i = 0; i < 10; ++i) {
        threads.push_back(thread(process));
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    return 0;
}