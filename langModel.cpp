//
//  langModel.cpp
//  

#include <cstdio>
#include <iostream>
#include "langModel.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <cstdlib>
#include <ctime>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::unordered_map;
using std::list;

int readInput(vector<string>& v) {
    string str;
    v.push_back("<START>");
    
    while (cin >> str) {
        v.push_back(str);
    }
    v.push_back("<END>");
    
    return 1;
}

int readPairs(vector<string> in, vector<string> &pairs) {
    
    if ((int)in.size() <= 2) {
        return 0;
    }
    
    string firstWord;
    string secondWord;
    
    for (int i = 0; i < (int)in.size()-1; i++) {
        firstWord = in[i];
        secondWord = in[i+1];
        pairs.push_back(firstWord + " " + secondWord);
    }
    
    return 1;
}

int updatePairCount(LanguageModel& lm, vector<string> v) {
    
    if (v.size() == 0) {
        return 0;
    }
    unordered_map<string, int> pairCount;
    
    for (int i = 0; i < (int)v.size(); i++) {
        if (pairCount.find(v[i]) == pairCount.end()) { //if the pair has not yet been added
            pairCount[v[i]] = 1; //initialize pair
            
            for (int r = i+1; r < (int)v.size(); r++) {
                if (v[r].compare(v[i]) == 0) { //if there is another instance of the pair
                    pairCount[v[i]]++;
                }
            }
        }
    }
    lm.pairCount = pairCount;
    
    return 1;
}

int updateStartWord(LanguageModel& lm, vector<string> v) {
    
    if (v.size() == 0) {
        return 0;
    }
    
    unordered_map<string, int> startWord;
    
    string firstWord;  //first word in the pair
    int indexSpace;    //marks the index of the white space in the pair
    
    for (int i = 0; i < (int)v.size(); i++) {
        indexSpace = v[i].find(' ');
        firstWord = v[i].substr(0, indexSpace);
        
        if (startWord.find(firstWord) == startWord.end()) { //if the word has not yet been added
            startWord[firstWord] = 1; //initialize word
        } else { //if word is present in map
            startWord[firstWord]++;
        }
    }
    lm.startWord = startWord;
    
    return 1;
}

int updateFollowWord(LanguageModel& lm, vector<string> v) {
    if (v.size() == 0) {
        return 0;
    }
    
    unordered_map<string, list<string> > followWord;
    
    string firstWord;   //first word in the pair
    string secondWord;  //second word in the pair
    int indexSpace;     //marks the index of the white space in the pair
    bool added = false; //notes if the follow word has been added to the list
    
    for (int i = 0; i < (int)v.size(); i++) {
        indexSpace = v[i].find(' ');
        firstWord = v[i].substr(0, indexSpace);
        secondWord = v[i].substr(indexSpace+1);
     
        //if the word has not yet been added
        if (followWord.find(firstWord) == followWord.end()) {
            followWord[firstWord]; //initialize word
        }
        
        //traverse through list
        list<string>::const_iterator iterator;
        for (iterator = followWord[firstWord].begin(); iterator != followWord[firstWord].end(); iterator++) {
            
            //note if follow word has been added to list
            if ((*iterator).compare(secondWord) == 0) {
                added = true;
            }
        }
        
        //if follow word has not been added to the list, add it
        if (!added) {
            followWord[firstWord].push_back(secondWord);
        }

        added = false;
    }
    lm.followWord = followWord;
    
    return 1;
}

int genNext(string startWord, LanguageModel& model, vector<string>& newText, int ran) {
    
    int numPairs;                                       //number of times a pair appears
    int startWordCount = model.startWord[startWord];    //context word
    int tempRan = rand() % (startWordCount);            //random # based on times start word appears
    
    if (ran == -1) {    //if a number has not been passed in as a parameter
        ran = tempRan;
    }
    
    //traverse through linked list of follow words
    list<string>::const_iterator iterator;
    for (iterator = model.followWord[startWord].begin(); iterator != model.followWord[startWord].end(); iterator++) {
        
        string followWord = *iterator;

            numPairs = model.pairCount[startWord + " " + followWord];
        
            ran = ran - numPairs;
    
        if (ran < 0) {
            //add the previous element in the linked list to the new text vector
            newText.push_back(*iterator);
            return 1;
        }
    }
    return 0;
}

void printText(vector<string> newText) {
    for (int i = 0; i < (int)newText.size(); i++) {
        if (newText[i].compare("<END>") != 0) {
            cout << newText[i] << " ";
        }
    }
    cout << endl;
}



