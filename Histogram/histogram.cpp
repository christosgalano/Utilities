#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <algorithm>
#include <regex>

using namespace std;

// The following program does not take into account cases like we've,that's etc.
// In those cases after the removal of punctuation the remainig word will be weve, thats etc.

int main() {
    ifstream from {"sample_text_2.txt"};

    if (!from) {
        cerr << "Error: file could not be opened." << endl;
        return 1;
    }

    // Store the words - istream_iterator ignores white space
    vector<string> words {istream_iterator<string>{from}, istream_iterator<string>{}};

    // Make all words lower_case and remove punctuation
    for (auto& s : words) {
        for_each(s.begin(), s.end(), [] (char& c) { c = ::tolower(c); } );
        s.erase(remove_if(s.begin(), s.end(), ::ispunct), s.end());
    }
    
    // Perhaps a string consisted only of a "punctuation_symbol" so after we removed the punctuation_symbol that string will now be empty.
    // That is why we erase those strings - besides they have no use.
    words.erase(remove(words.begin(), words.end(), ""), words.end());

    // Remove numbers 
    auto remove_numbers = [] (string& s) -> bool { return regex_match(s, regex("^[[:digit:]]+$")); };
    words.erase(remove_if(words.begin(), words.end(), remove_numbers), words.end());
    
    // If you want to keep the numbers you must take into account that if the text has real numbers for example 10.23, then when
    // we remove punctuation 10.23 will become 1023. So if you want to find if a string contains a real number you can use this regex expression:
    // (regex_match(s, regex("^([[:digit:]]+)(\\.|\\,)([[:digit:]]+)$")))

    // Map each word to each number_of_occurrences
    map<string, int> occur;
    for (const auto& s : words)
        ++occur[s];

    using entry = pair<string, int>;

    // Push the pairs into a temporary vector because we want to sort them based on the values and not the keys
    vector<entry> vec;
    for (const auto& p : occur)
        vec.push_back(p);

    // Custom compare function: we sort in decreasing order based on number_of_occurrences.
    // If the have the same number_of_occurrences we sort them alphabetically.
    auto comp = [] (const entry& p1, const entry& p2) -> bool {
        return p1.second == p2.second ? p1.first < p2.first : p1.second > p2.second;
    };

    sort(vec.begin(), vec.end(), comp);

    // Find the word with the maximum length
    auto comp_pairs = [] (const entry& p1, const entry& p2) -> bool {
        return p1.first.length() < p2.first.length();
    };

    auto v = max_element(vec.begin(), vec.end(), comp_pairs);

    int max_len {static_cast<int>(v->first.length())};

    // Print horizontal histogram
    for (const auto& p : vec) {
        cout << p.first << ':';
        
        for (int i = p.first.length(); i <= max_len; ++i)
            cout << ' ';

        for (int i = 0; i < p.second; ++i)
            cout << "*";
        
        cout << endl;
    }
}
