#include <fstream>
#include <string>
#include <iterator>
#include <set>
#include <regex>
#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

using namespace std;

string extractContent(string link) {
    curlpp::options::Url myUrl(link);
    curlpp::Easy myRequest;
    myRequest.setOpt(myUrl);
    ostringstream os;
    os << myRequest;

    return os.str();
}

int main() {

    cout << extractContent("http://www.cplusplus.com/privacy.do");
    return 0;
}