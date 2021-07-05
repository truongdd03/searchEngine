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

bool validateLink(string link) {
    if (link [0] == '/' && link[1] == '/') {
        cout << "INVALID " << link << "\n\n";    
        return false;
    }

    if (link[0] != '/' && link.find("http://") != 0) {
        cout << "INVALID " << link << "\n\n";    
        return false;
    } 

    return true;
}

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

int siz = 1;

void crawl(string link) {
    string content = extractContent(link);

    string title = extractTitle(content);
    if (titles.find(title) != titles.end()) { 
        cout << "EXISTED " << link << "\n\n";
        return; 
    }

    if (links.find(link) != links.end() || link == "") { 
        cout << "EXISTED " << link << "\n\n";
        return; 
    }

    cout << "NEW #" << siz++ << " " << link << "\n\n";

    titles.insert(title);
    links.insert(link);

    set<string> linksOfPage = extractLinks(content);
    set<string>::iterator itr;

    for (itr = linksOfPage.begin(); itr != linksOfPage.end(); itr++) {
        string newLink = *itr;

        if (!validateLink(newLink)) return;

        newLink = reformat(newLink);
        q.push(newLink);
    }

}

int main() {
    curlpp::Cleanup myCleanup;

    //links.insert("https://en.wikipedia.org/wiki/Main_Page");
    q.push("http://www.cplusplus.com");

    //int i = 1;
    vector <thread> threads;
    while (!q.empty()) {
        //myMutex.lock();
        string link = q.front();
        //myMutex.unlock();

        //cout << "NEW #" << i++ << " " << link << "\n\n";

        //threads.push_back(thread(crawl, link));
        q.pop();

        //threads.back().join();

        crawl(link);
    }

    return 0;
}