#include <iostream>
#include <locale>
#include <codecvt>

#include <OleanderStemmingLibrary/include/olestem/stemming/english_stem.h>

std::string stem(std::string s) {
    stemming::english_stem<> StemEnglish;
    wchar_t* UnicodeTextBuffer = new wchar_t[s.length()+1];
    std::wmemset(UnicodeTextBuffer, 0, s.length()+1);
    std::mbstowcs(UnicodeTextBuffer, s.c_str(), s.length());
    std::wstring word = UnicodeTextBuffer;
    StemEnglish(word);

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string str = converter.to_bytes(word);

    return str;
}