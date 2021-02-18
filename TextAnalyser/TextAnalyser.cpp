#include "TextAnalyser.hpp"

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <regex>
#include <exception>


TextAnalyser::TextAnalyser(const std::string& in_path)
    : path{in_path}
{
    std::ifstream text{path};
    if (!text)
        throw std::runtime_error("Error upon opening the file...");
    
    // Read the text - istream_iterator ignores white space
    words = std::vector<std::string> { std::istream_iterator<std::string>{text}, std::istream_iterator<std::string>{} };

    // Everything that contains letters or space 
    std::regex exp {"[^a-zA-Z\\s$]"};

    // Make all words lower_case and replace anything that is not a letter or a space with ""
    for (auto& s : words) {
        for_each(s.begin(), s.end(), [] (char& c) { c = ::tolower(c); } );
        s = std::regex_replace(s, exp, "");  
    }

    // Erase the "" strings
    words.erase(remove(words.begin(), words.end(), ""), words.end());

    // Map each word to its no_occurrences
    for (const auto& s : words)
        ++occurrences[s];

    for (const auto& p : occurrences)
        entries.push_back(p);

    // Custom compare function: we sort in decreasing order based on no_occurrences.
    // If they have the same no_occurrences we sort them alphabetically.
    auto comp = [] (const entry& p1, const entry& p2) -> bool {
        return p1.second == p2.second ? p1.first < p2.first : p1.second > p2.second;
    };

    std::sort(entries.begin(), entries.end(), comp);

    // Find the word/s with the maximum length
    auto comp_pairs = [] (const entry& p1, const entry& p2) -> bool {
        return p1.first.length() < p2.first.length();
    };

    auto it = std::max_element(entries.begin(), entries.end(), comp_pairs);
    max_length = it->first.length();
}

size_t TextAnalyser::get_no_occurrences(const std::string& word) {
    auto iter = std::find(words.begin(), words.end(), word);
    return iter == words.end() ? 0 : occurrences[*iter]; 
}

std::vector<std::string> TextAnalyser::get_most_used_words(size_t limit) {
    if (!limit)
        throw std::invalid_argument("Number of words must be 1 or greater.");

    std::vector<std::string> most_used;
    for (size_t i = 0; i < limit; ++i)
        most_used.push_back(entries[i].first);
    return most_used;
}

void TextAnalyser::print_histogram() const {
    // Print horizontal histogram
    for (const auto& p : entries) {
        std::cout << p.first << ':';
        
        for (int i = p.first.length(); i <= max_length; ++i)
            std::cout << ' ';

        for (int i = 0; i < p.second; ++i)
            std::cout << "*";
        
        std::cout << std::endl;
    }
}
