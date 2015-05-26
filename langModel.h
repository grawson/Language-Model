//
//  langModel.h
//

#ifndef ____langModel__
#define ____langModel__

#include <cstdio>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <list>

struct languageModel {
    std::unordered_map<std::string, int> pairCount;
    std::unordered_map<std::string, int> startWord;
    std::unordered_map<std::string, std::list<std::string> > followWord;
};
typedef struct languageModel LanguageModel;

/*
 * Reads user input from stdin into a vector of strings.
 */
int readInput(std::vector<std::string> &v);

/* 
 * reads from vecto and places all possible pairs of words into vector v of strings. 
 * Returns 1 if successful, otherwise returns 0 (if vector was empty). Updates the 
 * value for number of pairs in vector.
 */
int readPairs(std::vector<std::string> in, std::vector<std::string> &pairs);

/*
 * Iterates through a vector of string pairs and updates the pairCount map in a languageModel
 * accordingly. returns 1 if successful, 0 if vector is empty.
 */
int updatePairCount(struct languageModel& lm, std::vector<std::string> v);

/*
 * Iterates through a vector of string pairs and updates the startWord map in a languageModel
 * accordingly. returns 1 if successful, 0 if vector is empty.
 */
int updateStartWord(LanguageModel& lm, std::vector<std::string> v);

/*
 * Iterates through a vector of string pairs and updates the followWord map in a languageModel
 * accordingly. returns 1 if successful, 0 if vector is empty.
 */
int updateFollowWord(LanguageModel& lm, std::vector<std::string> v);

/*
 * Creates a vector of new text based on the language model. Works by: 
 * -checks context word (already present word) and how many times that word appears
 *  in language model.
 * -generates a random number between 0 and the number found
 * -traverses through the linked list of follow words. The number of times the 
 *  pair (context word and word in linked list) appears is subtracted from the
 *  random number. If the number is less than 0, that word is added as the next word.
 * 
 * returns 1 if a word was added, 0 otherwise.
 */
int genNext(std::string startWord, LanguageModel& model, std::vector<std::string>& newText, int ran = -1);

/*
 * Prints the contents of the vector representing the newly generated text.
 * Does not print <START> or <END>
 */
void printText(std::vector<std::string> newText);

#endif /* defined(____langModel__) */
