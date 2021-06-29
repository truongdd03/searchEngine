#include <iostream>
#include <stdio.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <sstream>
#include <fstream>

using namespace std;

int main()
{
    curlpp::Cleanup myCleanup;

    curlpp::options::Url myUrl(std::string("https://en.wikipedia.org/wiki/Wikipedia:GLAM/Newsletter"));
    curlpp::Easy myRequest;
    myRequest.setOpt(myUrl);

    myRequest.perform();
    return 0;
}