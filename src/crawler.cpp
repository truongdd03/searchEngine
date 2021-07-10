#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <algorithm>

#include "parser.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

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

bool isExisted(std::string content, std::string link) {
    std::string title = parseTitle(content);
    
    if (titles.find(title) != titles.end()) return true; 

    if (links.find(link) != links.end() || link == "") return true; 

    titles.insert(title);
    links.insert(link);
    return false;
}

int siz = 1;

void crawl(std::string link) {
    std::string content = parseContent(link);

    if (isExisted(content, link)) {
        //cout << "EXISTED " << link << "\n\n";
        return;
    }

    std::cout << "NEW #" << siz++ << " " << link << "\n\n";

    std::set<std::string> linksOfPage = parseLinks(content);
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
    for (int i = 0; i < std::min(numberOfThreads, int(q.size())); ++i) {
        threads.push_back(std::thread(process));
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}
int main() {
    curlpp::Cleanup myCleanup;

    crawl("https://en.wikipedia.org/wiki/Main_Page");
    run();
    
    return 0;
}