#include <iostream>
#include <fstream>
#include <iterator>

#include <crawler.h>
#include <parser.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

int main() {
    curlpp::Cleanup myCleanup;

    startCrawling();

    std::ofstream myFile;
    myFile.open("words.txt", std::ios::app);
    std::set<std::string>::iterator itr;
    for (itr = dict.begin(); itr != dict.end(); ++itr)
        myFile << *itr << "\n";
    myFile.close();
    
    return 0;
}