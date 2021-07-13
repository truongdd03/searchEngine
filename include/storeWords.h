#ifndef STOREWORDS_H
#define STOREWORDS_H

#include <set>
#include <vector>

struct Word {
    std::string word;
    int id;

    bool operator< (const Word& a) const {
        return word < a.word;
    }

    bool operator== (const Word& a) const {
        return word == a.word;
    }

    bool operator!= (const Word& a) const {
        return word != a.word;
    }
};

struct PageInfo {
    int pageID, value;

   /* bool operator< (const PageInfo& a) const {
        return pageID < a.pageID;
    }

    bool operator== (const PageInfo& a) const {
        return pageID == a.pageID;
    }

    bool operator!= (const PageInfo& a) const {
        return pageID != a.pageID;
    }*/
};

extern int numberOfWords;
extern std::vector<std::string> words;
extern std::vector<std::vector<PageInfo>> wordPositions;

extern std::set<Word> dict;

void updateDict(std::string s, int linkID);
#endif