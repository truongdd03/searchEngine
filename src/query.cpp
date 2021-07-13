#include <iostream>
#include <stdio.h>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <queue>

#include "storeWords.h"
#include "query.h"
#include "parser.h"

struct result {
    int linkID, point;
};
struct cmp {
    bool operator () (const result& a, const result& b) {
        return a.point < b.point;
    }
};

std::vector<std::string> pageLinks;
std::vector<std::string> wordsToFind;
std::priority_queue<result,std::vector<result>,cmp > pq;

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

void query(std::string goal) {
    wordsToFind.clear();
    wordsToFind = splitWords(goal);

    for (int i = 0; i < wordsToFind.size(); ++i) {
        std::cout << wordsToFind[i] << " ";
    }
    std::cout << "\n";

    std::ifstream file("positions.txt");
    std::string word;
    while (std::getline(file, word)) {
        std::string s;
        std::getline(file, s);

        if (word == goal) {
            std::vector<int> tmp = fetchArray(s);
            for (int i = 0; i < tmp.size(); ++i) {
                std::cout << "#" << i+1 << " " << pageLinks[tmp[i]] << "\n";
            }

            return;
        }
    }
}