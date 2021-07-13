#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <algorithm>
#include <fstream>

#include "parser.h"
#include "crawler.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

const std::string src = "https://en.wikipedia.org/";
std::vector<std::thread> threads;
std::queue<std::string> q;
std::mutex myMutex;
std::set<std::string> links, titles;
std::ofstream linksFile;
std::vector<std::string> listOfLinks;


std::string reformat(std::string link) {
    if (link[0] == '/') return src + link;
    if (link.rfind(src) != 0) return "";
    return link;
}

bool isExisted(std::string content, std::string link) {
    std::string title = parseTitle(content);
    
    myMutex.lock();
    if (titles.find(title) != titles.end()) { myMutex.unlock(); return true; }
    if (links.find(link) != links.end() || link == "") { myMutex.unlock(); return true; }

    titles.insert(title);
    links.insert(link);
    myMutex.unlock();
    return false;
}

void crawl(std::string link) {
    std::string content = parseContent(link);

    if (isExisted(content, link)) {
        //std::cout << "EXISTED " << link << "\n\n";
        return;
    }

    myMutex.lock();
    listOfLinks.push_back(link);
    threads.push_back(std::thread(parseString, content, listOfLinks.size()));
    
    std::cout << "#" << listOfLinks.size() << "\n";

    myMutex.unlock();

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
        if (q.empty() || listOfLinks.size() > 1000) {
            myMutex.unlock();
            return;
        }
        
        std::string link = q.front();
        q.pop();
        myMutex.unlock();

        crawl(link);
    }

    return;
}

void startCrawling(int numberOfThreads) {
    crawl("https://en.wikipedia.org/wiki/Main_Page");

    for (int i = 0; i < std::min(numberOfThreads, int(q.size())); ++i) {
        threads.push_back(std::thread(process));
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    linksFile.open("links.txt", std::ios::app);
    for (int i = 0; i < listOfLinks.size(); ++i) {
        linksFile << i+1 << "\n" << listOfLinks[i] << "\n";
    }
    linksFile.close();

}