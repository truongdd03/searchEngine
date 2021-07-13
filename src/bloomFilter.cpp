#include <iostream>
#include <stdio.h>
#include <vector>
#include <mutex>
#include <set>

#include "bloomFilter.h"
#include "parser.h"

int cmodSize = 4;
int cmod[4] = {1000003, 1222003, 1030033, 1029323};
int base = 311;
bool titlesID[2000000];
std::set<std::string> txt;
std::mutex bloomMutex;

std::vector<int> getBloomID(std::string str) {
    std::vector<int> res;
    std::vector<long long> cur;
    for (int i = 0; i < cmodSize; ++i) {
        res.push_back(0);
        cur.push_back(1);
    }

    for (int i = 0; i < str.length(); ++i) {
        for (int j = 0; j < cmodSize; ++j) {
            res[j] += (cur[j] * str[i]) % cmod[j];
            res[j] %= cmod[j];
            cur[j] = (cur[j] * base) % cmod[j];
        }
    }

    return res;
}

bool isExisted(std::string content) {
    std::string title = parseTitle(content);
    std::vector<int> titleID = getBloomID(title);

    bloomMutex.lock();
    bool kt = true;
    for (int i = 0; i < titleID.size(); ++i)
        if (!titlesID[titleID[i]]) {
            titlesID[titleID[i]] = true;
            kt = false;
        }
    bloomMutex.unlock();
    
    return kt;
}