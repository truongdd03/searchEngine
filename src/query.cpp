#include <iostream>
#include <stdio.h>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>

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
std::set<searchResult> results;

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
    wordsToFind.clear();
    finalResults.clear();
    results.clear();
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
        std::set<searchResult>::iterator itr = results.find({ids[i], 0, 0});
        int cur = values[i], tmp = 1;
        if (itr != results.end()) {
            cur += itr->value;
            tmp += itr->wordsAppeared;
            results.erase(itr);
        }

        results.insert({ids[i], cur, tmp});
    }
}

void compare(std::string word, std::string idsString, std::string valuesString) {
    for (int i = 0; i < wordsToFind.size(); ++i) {
        if (word == wordsToFind[i]) {
            std::vector<int> ids = fetchArray(idsString);
            std::vector<int> values = fetchArray(valuesString);
            merge(ids, values);
        }
    }
}

bool cmp1(searchResult a, searchResult b) {
    return a.wordsAppeared > b.wordsAppeared || (a.wordsAppeared == b.wordsAppeared && a.value > b.value);
}

void printResult() {
    std::set<searchResult>::iterator itr = results.begin();
    for (itr = results.begin(); itr != results.end(); ++itr) {
        finalResults.push_back({itr->pageID, itr->value, itr->wordsAppeared});
    }

    sort(finalResults.begin(), finalResults.end(), cmp1);
    for (int i = 0; i < std::min(10, int(finalResults.size())); ++i) {
        std::cout << finalResults[i].pageID << " " << finalResults[i].value << " " << finalResults[i].wordsAppeared << "\n";
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

        compare(word, idsString, valuesString);
    }
    printResult();
}
