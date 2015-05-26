//
//  LM_Driver.cpp
//

#include <cstdio>
#include <iostream>
#include <vector>
#include "langModel.h"
#include <ctime>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;

int main() {
    
    vector<string> in;      //vector for inpout string
    vector<string> v;       //vector of all possible pairs
    LanguageModel model;
    vector<string> newText; //stores new text generated from model


    cout << "Please enter a paragraph of text to serve as a model:" <<endl;
    
    readInput(in);

    if (readPairs(in, v) == 0) {
        cout << "Error - No text was input." << endl;
        return 1;
    }
  
    if (updatePairCount(model, v) == 0) {
        cout << "(Error - When updating pairCount map, vector used was empty.)" << endl;
        return 1;
    }

    if (updateStartWord(model, v) == 0) {
        cout << "(Error - When updating startWord map, vector used was empty.)" << endl;
        return 1;
    }
    
    if (updateFollowWord(model, v) == 0) {
        cout << "(Error - When updating followWord map, vector used was empty.)" << endl;
        return 1;
    }
    
    srand(time(NULL)); //seed the genrator for the genNext function
    if (genNext("<START>", model, newText) == 0) {   //generate the next word based on <START>
        cout << "Next word based on context word '<START>' not added to new text generation vector." << endl;
        return 1;
    }
    
    while (newText.back().compare("<END>")) {  //while <END> has not been reached
        if(genNext(newText.back(), model, newText) == 0) {
            cout << "Next word based on context word '" << newText.back() << "' not added to new text generation vector." << endl;
            return 1;
        }
    }
    
    cout << endl << "New text based on language model created:" << endl << endl;
    printText(newText);
    
    cout << endl;
    
    return 0;
}
