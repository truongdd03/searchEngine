#include <iostream>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <queue>
#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;

const string src = "http://www.cplusplus.com";

set <string> links, titles;
queue <string> q;

string fetchTitle(int &i, string &content) {
    int num = 1; string title = "";

    while (true) {
        if (content[i] == '<') ++num;
        else if (content[i] == '>') --num;
        else title += content[i];

        string cur = "";
        for (int j = i+1; j <= i+8; ++j) {
            cur += content[j];
        }
        if (cur == "</title>") {
            i += 9;
            return title;
        }

        ++i;
    }
}

bool isExisted(string title) {
    if (titles.find(title) != titles.end()) return true;
    return false;
}

void skip(int &i, string &content) {
    int num = 1;
    while (num != 0) {
        if (content[i] == '<') ++num;
        if (content[i] == '>') --num;
        ++i;
    }
}

bool isLinkExisted(string link) {
    
    for (int i = 0; i < link.length(); ++i) {
        if (link[i] == ' ') return true;
    }

    if (links.find(link) != links.end()) return true;

    return false;
}

string fetchLink(string link) {
    curlpp::options::Url myUrl(link);
    curlpp::Easy myRequest;
    myRequest.setOpt(myUrl);
    ostringstream os;
    os << myRequest;

    return os.str();
}

void crawl(string link) {

    string content = fetchLink(link);

    int i = 5;
    while (i < content.length()) {
        string cur = "";
        for (int j = i-5; j <= i; ++j) {
            cur += content[j];
        }
        ++i;

        if (cur == "title>") {
            string title = fetchTitle(i, content);

            if (isExisted(title)) return;
            else {
                cout << title << "\n";
                cout << "#" << links.size() << "\n";
                cout << link << "\n\n";

                links.insert(link);
                titles.insert(title);
            }
        }

        string curb = cur.substr(1, cur.length()-1);
        if (cur == "<style" || curb == "<link") {
            skip(i, content);
        }

        if (cur == "href=\"") {
            string newLink = "";

            bool kt = true;
            while (content[i] != '\"') {
                newLink += content[i++];
                if ( newLink[0] != '/' || (newLink.length() >= 2 && newLink[1] == '/') ) {
                    kt = false;
                    break;
                }
            }

            if ((newLink[0] == '/' && newLink[1] != '/') && kt == true) {
                
                if (isLinkExisted(newLink) == false) {

                    //cout << newLink << "\n\n";

                    newLink = src + newLink;
                    q.push(newLink);
                    //links.insert(newLink);
                }

            }
        }
    }
}

int main() {
    curlpp::Cleanup myCleanup;

    //crawl("https://en.wikipedia.org/wiki/Main_Page");
    q.push("http://www.cplusplus.com");

    //int i = 1;
    while (!q.empty()) {
        string link = q.front();

        //cout << "#" << i++ << " " << q.size() << "\n";
        //cout << link << "\n\n";
        
        q.pop();
        crawl(link);
    }

    return 0;
}