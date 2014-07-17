/*
    Opens a file handle to the supplied filepath,
    reads the text into words and calculates their frequency.
    Ignores a set of words known as stop words. Displays the contents
    of the frequency mapping.

    Write to file:
    algorithms.exe [[input path]] > [[output path]]

*/

#include <algorithm>
#include <array>
#include <fstream>
#include <iterator>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <utility>

using namespace std;

// Synonyms
typedef string Token;
typedef int Frequency;
typedef int ReturnStatus;
typedef set<Token> BagOfWords;
typedef vector<Token> RunningText;
typedef vector<Token> EmptyText;
typedef map<Token, Frequency> FreqMap;
typedef istream_iterator<Token> FileIter;


bool is_stopword(const Token & word)
{
    static BagOfWords stop_words;
    if (!stop_words.size())
    {
        array<Token, 6> words = { "a","an","and","or","the","but" };
        stop_words = BagOfWords(begin(words), end(words));
    }

    // A word is a stop word if it has a count of one in stop_words
    return stop_words.count(word) == 1;
};

RunningText read_words(const char * filename)
{
    fstream f_in(filename);
    bool bad_file(!f_in);
    if (bad_file)
    {
        return RunningText();
    }

    return RunningText(FileIter(f_in), FileIter());
};

FreqMap count_words(const RunningText & words)
{
    FreqMap word_freqs;

    // C++11 lambda function. Braces capture outer values/references
    // parentheses are enclose the parameter list
    // curly braces enclose the impelementation.
    // Must use auto because only the compiler knows the type.
    auto accumulate =
        [&word_freqs]
        (const Token & word)
        {
            if (!is_stopword(word))
            {
                if (!word_freqs.count(word))
                        word_freqs[word] = 1;

                else
                    word_freqs.at(word)++;
            }
        };

    // Apply accumulate to every word in the running text.
    for_each(begin(words), end(words), accumulate);

    return move(word_freqs);
}

inline bool enough_args(int num_args)
{
    return num_args == 2;
}

ReturnStatus main(int argc, char ** argv)
{
    // Command line admin
    if (enough_args(argc))
    {
        cout << "Need a filename!\n";
        exit(1);
    }

    // Do the work
    RunningText words = read_words(argv[1]);
    FreqMap word_counts = count_words(words);

    // Show results
    for (auto iter = begin(word_counts); iter != end(word_counts); ++iter)
    {
        cout << iter->first << " " << iter->second << "\n";
    }
}