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

std::vector<std::string> pageLinks, wordsToFind;
std::vector<PageInfo> finalResults;
std::set<PageInfo> results;

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
            if (res.size() == 10) return res;
        }
        else {
            cur = cur * 10 + (s[i] - '0');
        }
    }

    return res;
}

void merge(std::vector<int> &ids, std::vector<int> &values) {
    for (int i = 0; i < ids.size(); ++i) {
        std::set<PageInfo>::iterator itr = results.find({ids[i], 0});
        int cur = values[i];
        if (itr != results.end()) {
            cur += itr->value;
            results.erase(itr);
        }

        results.insert({ids[i], cur});
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

bool cmp1(PageInfo a, PageInfo b) {
    return a.value > b.value;
}

void printResult() {
    std::set<PageInfo>::iterator itr = results.begin();
    int cnt = 1;

    finalResults.clear();
    while (cnt <= 10 && itr != results.end()) {
        finalResults.push_back({itr->pageID, itr->value});
        cnt++;
        ++itr;
    }

    sort(finalResults.begin(), finalResults.end(), cmp1);
    for (int i = 0; i < finalResults.size(); ++i) {
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
