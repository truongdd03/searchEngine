#include <iostream>
#include <mutex>
#include <vector>
#include <set>
#include <fstream>

#include "storeWords.h"
#include "stemmer.h"

std::vector<std::set<PageInfo>> wordPositions;
std::set<Word> dict;
std::vector<std::string> words;
std::vector<std::string> stopWords;
std::mutex dictMutex;


bool isImportant(std::string s) {
    for (int i = 0; i < stopWords.size(); ++i) 
        if (stopWords[i] == s) return false;

    if (s.length() == 1) return false;

    for (int i = 0; i < s.length(); ++i) {
        if (s[i] < 'a' || s[i] > 'z') return false;
    }
    if (s == "") return false;

    return true;
}

void buildStopWords() {
    std::ifstream file("stopWords.txt");
    std::string word;
    while (std::getline(file, word)) {
        stopWords.push_back(stem(word));
    }
}

void updateDict(std::string s, int linkID, int value) {    
    if (!isImportant(s)) return;

    dictMutex.lock();
    std::set<Word>::iterator itr = dict.find({s, 0});
    if (itr == dict.end()) {
        std::set<PageInfo> tmp;
        tmp.insert({linkID, value});

        wordPositions.push_back(tmp);
        words.push_back(s);
        dict.insert({s, int(wordPositions.size()-1)});
    } else {
        int id = itr->id;

        std::set<PageInfo>::iterator ptr = wordPositions[id].find({linkID, 0});
        if (ptr != wordPositions[id].end()) {
            value += ptr->value;
            wordPositions[id].erase(ptr);
        }
        wordPositions[id].insert({linkID, value});
    }   

    dictMutex.unlock();
}