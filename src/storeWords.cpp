#include <iostream>
#include <mutex>
#include <vector>
#include <set>

#include "storeWords.h"

std::vector<std::set<PageInfo>> wordPositions;
std::set<Word> dict;
std::vector<std::string> words;

std::mutex dictMutex;

void updateDict(std::string s, int linkID) {

    dictMutex.lock();
    std::set<Word>::iterator itr = dict.find({s, 0});
    if (itr == dict.end()) {
        std::set<PageInfo> tmp;
        tmp.insert({linkID, 1});

        wordPositions.push_back(tmp);
        words.push_back(s);
        dict.insert({s, int(wordPositions.size()-1)});
    } else {
        int id = itr->id, value = 1;

        std::set<PageInfo>::iterator ptr = wordPositions[id].find({linkID, 0});
        if (ptr != wordPositions[id].end()) {
            value += ptr->value;
            wordPositions[id].erase(ptr);
        }
        wordPositions[id].insert({linkID, value});

        /*for (int i = 0; i < wordPositions[id].size(); ++i) {
            if (wordPositions[id][i].pageID == linkID) {
                wordPositions[id][i].value ++;
                dictMutex.unlock();
                return;
            }
        }
        wordPositions[id].push_back({linkID, 1});*/
    }   

    dictMutex.unlock();
}