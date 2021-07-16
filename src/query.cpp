#include <iostream>
#include <stdio.h>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>
#include <thread>
#include <mutex>

#include "storeWords.h"
#include "query.h"
#include "parser.h"

struct searchResult {
    int pageID, value, wordsAppeared;

    bool operator< (const searchResult& a) const {
        return pageID < a.pageID;
    }

    bool operator== (const searchResult& a) const {
        return pageID == a.pageID;
    }

    bool operator!= (const searchResult& a) const {
        return pageID != a.pageID;
    }
};

std::vector<std::string> pageLinks, wordsToFind;
std::vector<searchResult> finalResults;
std::vector<PageInfo> currentResults;
int wordsAppeared = 0;

void buildLinks() {
    std::string n;
    pageLinks.push_back("");

    std::ifstream file("links.txt");
    while (std::getline(file, n)) {
        std::getline(file, n);
        pageLinks.push_back(n);
    }
}

void prepareQuery() {    
    buildLinks();
}

void resetQuery() {
    wordsAppeared = 0;
    wordsToFind.clear();
    finalResults.clear();
    currentResults.clear();
}

std::vector<int> fetchArray(std::string s) {
    std::vector<int> res;
    int cur = s[0] - '0';
    for (int i = 1; i < s.length(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            res.push_back(cur);
            cur = 0;
        }
        else {
            cur = cur * 10 + (s[i] - '0');
        }
    }

    return res;
}

void merge(std::vector<int> &ids, std::vector<int> &values) {
    for (int i = 0; i < ids.size(); ++i) {
        currentResults.push_back({ids[i], values[i]});
    }
}

void compare(std::string word, std::string idsString, std::string valuesString) {
    for (int i = 0; i < wordsToFind.size(); ++i) {
        if (word == wordsToFind[i]) {
            ++wordsAppeared;
            std::vector<int> ids = fetchArray(idsString);
            std::vector<int> values = fetchArray(valuesString);
            merge(ids, values);
        }
    }
}

bool cmp1(searchResult a, searchResult b) {
    return a.wordsAppeared > b.wordsAppeared || (a.wordsAppeared == b.wordsAppeared && a.value > b.value);
}

bool cmp2(PageInfo a, PageInfo b) {
    return a.pageID < b.pageID;
}

void printResult() {
    sort(currentResults.begin(), currentResults.end(), cmp2);
    currentResults.push_back({-1, -1});
    int curID = currentResults[0].pageID, curValue = currentResults[0].value, curAppeared = 1;
    for (int i = 1; i < currentResults.size(); ++i) {
        if (currentResults[i].pageID != currentResults[i-1].pageID) {
            finalResults.push_back({curID, curValue, curAppeared});
            curID = currentResults[i].pageID;
            curValue = currentResults[i].value;
            curAppeared = 1;
        } else {
            curValue += currentResults[i].value;
            ++curAppeared;
        }
    }

    sort(finalResults.begin(), finalResults.end(), cmp1);
    for (int i = 0; i < std::min(10, int(finalResults.size())); ++i) {
        //std::cout << finalResults[i].pageID << " " << finalResults[i].value << " " << finalResults[i].wordsAppeared << "\n";
        std::cout << "#" << i+1 << "\n" << pageLinks[finalResults[i].pageID] << "\n";
    }
}

void query(std::string goal) {
    resetQuery();
    wordsToFind = splitWords(goal);

    std::ifstream file("positions.txt");
    std::string word;
    while (std::getline(file, word)) {
        std::string idsString, valuesString;
        std::getline(file, idsString);
        std::getline(file, valuesString);

        if (wordsAppeared == wordsToFind.size()) {
            printResult();
            return;
        }

        compare(word, idsString, valuesString);
    }
    printResult();
}
