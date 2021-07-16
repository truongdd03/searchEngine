#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

#include "crawler.h"
#include "parser.h"
#include "storeWords.h"
#include "query.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

std::vector<PageInfo> vt;

void resetFile(std::string name) {
    std::ofstream file;
    file.open(name);
    file << "";
    file.close();
}

void prepare() {
    int numberOfThreads = 0, numberOfLinks = 0;
    std::cout << "Number of links: "; std::cin >> numberOfLinks;
    startCrawling(numberOfLinks);

    std::ofstream myFile;
    myFile.open("positions.txt", std::ios::app);
    for (int i = 0; i < wordPositions.size(); ++i) {
        myFile << words[i] << "\n";

        for (int j = 0; j < wordPositions[i].size(); ++j)
            myFile << wordPositions[i][j].pageID << " ";
        myFile << "\n";
        for (int j = 0; j < wordPositions[i].size(); ++j)
            myFile << wordPositions[i][j].value << " ";

        myFile << "\n";
    }
    myFile.close();

    std::cout << "##### Finished #####\n";
}

void readQuery() {
    std::string goal; std::getline(std::cin, goal);
    while (true) {
        std::cout << "What are you searching? ";
        std::string goal; std::getline(std::cin, goal);
        std::cout << "Loading...\n";
        query(goal);
    }
}

int main() {
    curlpp::Cleanup myCleanup;
    buildStopWords();

    std::cout << "Do you want to crawl again? (y/n) ";
    char c; std::cin >> c;
    if (c == 'y') {
        resetFile("links.txt");
        resetFile("positions.txt");
        prepare();
    }

    prepareQuery();
    readQuery();

    return 0;
}