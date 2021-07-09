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

const string src = "https://en.wikipedia.org/";
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
        cout << "LINK: " << link << " ERROR: " << e.what() << "\n\n";
    }

    catch( curlpp::LogicError &e ) {
        cout << "LINK: " << link << " ERROR: " << e.what() << "\n\n";
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

    for (itr = linksOfPage.begin(); itr != linksOfPage.end(); itr++) {
        myMutex.lock();
        q.push(reformat(*itr));
        myMutex.unlock();
    }

}

void process() {
    while (true) {

        myMutex.lock();
                
        if (q.empty()) {
            terminate();
            return;
        }
        
        string link = q.front();
        q.pop();
        myMutex.unlock();

        crawl(link);
        //cout << "YEAh\n";

    }
}

void run() {
    int numberOfThreads = 0;
    cin >> numberOfThreads;
    for (int i = 0; i < numberOfThreads; ++i) {
        threads.push_back(thread(process));
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}
int main() {
    curlpp::Cleanup myCleanup;

    q.push("https://en.wikipedia.org/wiki/Main_Page");

    q.push("https://en.wikipedia.org/wiki/Portal:The_arts");
    q.push("https://en.wikipedia.org/wiki/Portal:History");
    q.push("https://en.wikipedia.org/wiki/Portal:Society");
    q.push("https://en.wikipedia.org/wiki/Portal:Biography");
    q.push("https://en.wikipedia.org/wiki/Portal:Mathematics");
    q.push("https://en.wikipedia.org/wiki/Portal:Technology");
    q.push("https://en.wikipedia.org/wiki/Portal:Geography");
    q.push("https://en.wikipedia.org/wiki/Portal:Science");
    q.push("https://en.wikipedia.org/wiki/Wikipedia:Contents/Portals");

    q.push("https://en.wikipedia.org/wiki/Wikipedia:Contents");
    q.push("https://en.wikipedia.org/wiki/Portal:Current_events");
    q.push("https://en.wikipedia.org/wiki/Wikipedia:About");
    q.push("https://en.wikipedia.org/wiki/Wikipedia:Contact_us");
    q.push("https://en.wikipedia.org/wiki/Help:Contents");

    q.push("https://en.wikipedia.org/wiki/Special:WhatLinksHere/Help:Introduction");
    q.push("https://en.wikipedia.org/wiki/Wikipedia:Community_portal");
    q.push("https://en.wikipedia.org/wiki/Special:WhatLinksHere/Main_Page");
    q.push("https://en.wikipedia.org/wiki/Special:SpecialPages");
    q.push("https://en.wikipedia.org/w/index.php?title=Main_Page&action=info");

    run();

    return 0;
}