#include <iostream>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <queue>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

#include <lexbor/html/parser.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;

const string src = "https://en.wikipedia.org";
vector<string> links, titles;

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
    for (int i = 0; i < titles.size(); ++i) {
        if (title == titles[i]) {
            return true;
        }
    }
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
    for (int i = links.size(); i >=0 ; --i) {
        if (links[i].length() != link.length()) { continue; }

        bool kt = true;
        for (int j = link.length() - 1; j >= 24; ++j) {
            if (link[j] != links[i][j]) { 
                kt = false;
                break;
            }
        }

        if (kt == true) return true;
    }

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
                cout << "#" << links.size() << "\n";
                cout << link << "\n\n";

                links.push_back(link);
                titles.push_back(title);
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

            if (newLink[0] == '/' && newLink[1] != '/' && kt == true) {
                newLink = src + newLink;
                
                if (isLinkExisted(newLink) == false) {
                    //crawl(newLink);
                    cout << q.size() << "\n";
                    q.push(newLink);
                }

            }
        }
    }
}

int main() {
    curlpp::Cleanup myCleanup;

    //crawl("https://en.wikipedia.org/wiki/Main_Page");
    //q.push("https://en.wikipedia.org/wiki/Main_Page");

    //while (!q.empty()) {
    //    string link = q.front();
    //    q.pop();
    //    crawl(link);
    //}

    cout << fetchLink("https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/tutorial.html");

    return 0;
}