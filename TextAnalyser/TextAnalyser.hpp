#pragma once
#include <string>
#include <vector>
#include <unordered_map>

/// Class that provides various information regarding the contents of a text document ///

class TextAnalyser {
private:
    using entry = std::pair<std::string, int>;

    std::string path;   

    size_t max_length {};   // Max length a word can have based on input

    std::vector<std::string> words; // Words of the text
    
    std::unordered_map<std::string, int> occurrences;   // Maps each words to its no_occurrences in the text
    
    std::vector<entry> entries; // Stores the pairs of occurences, because we want to sort them in decreasing order based on value

public:
    explicit TextAnalyser(const std::string& in_path);

    size_t get_max_length() const { return max_length; }

    std::string get_most_used_word() const { return entries[0].first; }  // entries is sorted in decreasing order

    size_t get_no_occurrences(const std::string& word);

    std::vector<std::string> get_most_used_words(size_t limit);
    
    void print_histogram() const;
};

// TODO: take into account cases like we've,that's etc.
// Currently in those cases after the removal of punctuation the remaining word will be weve, thats etc.