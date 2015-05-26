//
//  testLanguageModel.cpp
//  

#include <cstdio>
#include "langModel.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <string>
#include <unordered_map>
#include <list>

using std::string;
using std::list;
using std::unordered_map;
using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;

void testReadPairs() {
   
    vector<string> in;
    vector<string> v;
    vector<string> empty;
  
    //test regular input
    in = {"<START>", "Hello", "my", "name", "is", "Gavi", "<END>"};
    
    readPairs(in, v);
    
    assert(v.size() == 6);
    assert(v[0].compare("<START> Hello") == 0);
    assert(v[1].compare("Hello my") == 0);
    assert(v[2].compare("my name") == 0);
    assert(v[3].compare("name is") == 0);
    assert(v[4].compare("is Gavi") == 0);
    assert(v[5].compare("Gavi <END>") == 0);
    
    //test one word
    v = empty;
    in = empty;
    in = {"<START>", "Hello", "<END>"};
    
    assert(readPairs(in, v) == 1);
    
    assert(v.size() == 2);
    assert(v[0].compare("<START> Hello") == 0);
    assert(v[1].compare("Hello <END>") == 0);
    
    //test empty string
    v = empty;
    in = empty;
    in = {"<START", "END"};
    assert(readPairs(in, v) == 0);
    
    //Test if in vector is empty
    in = empty;
    assert(readPairs(in, v) == 0);
}

void testUpdatePairCount() {
    LanguageModel model;
    LanguageModel emptyLM;
    vector<string> v;
    vector<string> emptyV;
    
    //test regular input
    v.push_back("<START> Hello"); v.push_back("Hello my"); v.push_back("Hello my");
    v.push_back("name is"); v.push_back("name is"); v.push_back("Gavi <END>");
    
    assert(updatePairCount(model, v) == 1);
    
    assert(model.pairCount["<START> Hello"] == 1);
    assert(model.pairCount["Hello my"] == 2);
    assert(model.pairCount["name is"] == 2);
    assert(model.pairCount["Gavi <END>"] == 1);
    assert(model.pairCount["test test"] == 0);
    
    //test one word
    v = emptyV;
    model = emptyLM;
    v = {"<START> Hello", "Hello <END>"};
    
    assert(updatePairCount(model, v) == 1);
    
    assert(model.pairCount["<START> Hello"] == 1);
    assert(model.pairCount["Hello <END>"] == 1);
    assert(model.pairCount["test test"] == 0);
    
    //test empty vector parameter
    v = emptyV;
    model = emptyLM;
    assert(updatePairCount(model, v) == 0);
}

void testUpdateStartWord() {
    LanguageModel model;
    LanguageModel emptyLM;
    vector<string> v;
    vector<string> emptyV;
    
    //test regular input
    v.push_back("<START> Hello"); v.push_back("Hello my"); v.push_back("Hello my");
    v.push_back("name is"); v.push_back("name is"); v.push_back("Gavi <END>");
    
    assert(updateStartWord(model, v) == 1);
    
    assert(model.startWord["<START>"] == 1);
    assert(model.startWord["Hello"] == 2);
    assert(model.startWord["name"] == 2);
    assert(model.startWord["Gavi"] == 1);
    assert(model.startWord["<END>"] == 0);
    
    //test one word
    v = emptyV;
    model = emptyLM;
    v = {"<START> Hello", "Hello <END>"};
    
    assert(updateStartWord(model, v) == 1);
    
    assert(model.startWord["<START>"] == 1);
    assert(model.startWord["Hello"] == 1);
    assert(model.startWord["<END>"] == 0);
    
    //test empty vector parameter
    v = emptyV;
    model = emptyLM;
    assert(updateStartWord(model, v) == 0);
}

void testUpdateFollowWord() {
    LanguageModel model;
    LanguageModel emptyLM;
    vector<string> v;
    vector<string> emptyV;
    
    //test regular input
    v.push_back("<START> Hello"); v.push_back("Hello three"); v.push_back("Hello four");
    v.push_back("name one"); v.push_back("name two"); v.push_back("Gavi <END>");
    
    assert(updateFollowWord(model, v) == 1);
    
    list<string>::const_iterator iterator;
    for (iterator = model.followWord["<START>"].begin(); iterator != model.followWord["<START>"].end(); iterator++) {
        assert((*iterator).compare("Hello") == 0);
    }
    for (iterator = model.followWord["Hello"].begin(); iterator != model.followWord["Hello"].end(); iterator++) {
        assert(((*iterator).compare("three") == 0) || ((*iterator).compare("four") == 0));
    }
    for (iterator = model.followWord["name"].begin(); iterator != model.followWord["name"].end(); iterator++) {
        assert(((*iterator).compare("one") == 0) || ((*iterator).compare("two") == 0));
    }
    for (iterator = model.followWord["Gavi"].begin(); iterator != model.followWord["Gavi"].end(); iterator++) {
        assert((*iterator).compare("<END>") == 0);
    }
    for (iterator = model.followWord["test"].begin(); iterator != model.followWord["test"].end(); iterator++) {
        assert((*iterator).compare("Hello") != 0);
    }
    
    //test one word
    v = emptyV;
    model = emptyLM;
    v = {"<START> Hello", "Hello <END>"};
    
    assert(updateFollowWord(model, v) == 1);
    
    list<string>::const_iterator iterator2;
    for (iterator2 = model.followWord["<START>"].begin(); iterator2 != model.followWord["<START>"].end(); iterator2++) {
        assert((*iterator2).compare("Hello") == 0);
    }
    for (iterator2 = model.followWord["Hello"].begin(); iterator2 != model.followWord["Hello"].end(); iterator2++) {
        assert((*iterator2).compare("<END>") == 0);
    }
    
    //test empty vector parameter
    v = emptyV;
    model = emptyLM;
    assert(updateFollowWord(model, v) == 0);
}

void testGenNext() {
    LanguageModel model;
    vector<string> v;
    vector<string> newText;
    vector<string> emptyV;
    LanguageModel emptyLM;
    
    //new test from model "I am writing this test to see what the test is going to do"
    v.push_back("<START> I"); v.push_back("I am"); v.push_back("am writing"); v.push_back("writing this");
    v.push_back("this test"); v.push_back("test to"); v.push_back("to see"); v.push_back("see what");
    v.push_back("what the"); v.push_back("the test"); v.push_back("test is"); v.push_back("is going");
    v.push_back("going to"); v.push_back("to do"); v.push_back("do <END>");
    
    updatePairCount(model, v);
    updateStartWord(model, v);
    updateFollowWord(model, v);
    
    assert(genNext("<START>", model, newText, 0) == 1);
    assert(newText.back().compare("I") == 0); //"<START> I"
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("am") == 0); //"<START> I am"
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("writing") == 0); //"<START> I am writing"
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("this") == 0); //"<START> I am writing this"
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("test") == 0); //"<START> I am writing this test"
    
    assert(genNext(newText.back(), model, newText, 1) == 1);
    assert(newText.back().compare("is") == 0); //"<START> I am writing this test is"
    newText.pop_back();
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("to") == 0); //"<START> I am writing this test to"
    
    assert(genNext(newText.back(), model, newText, 1) == 1);
    assert(newText.back().compare("do") == 0); //"<START> I am writing this test to do"
    newText.pop_back();
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("see") == 0); //"<START> I am writing this test to see"

    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("what") == 0); //"<START> I am writing this test to see what"
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("the") == 0); //"<START> I am writing this test to see what the"
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("test") == 0); //"<START> I am writing this test to see what the test"
    
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("to") == 0); //"<START> I am writing this test to see what the test to"
    newText.pop_back();
    assert(genNext(newText.back(), model, newText, 1) == 1);
    assert(newText.back().compare("is") == 0); //"<START> I am writing this test to see what the test is"
    
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("going") == 0); //"<START> I am writing this test to see what the test is going"
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("to") == 0); //"<START> I am writing this test to see what the test is going to"
    
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("see") == 0); //"<START> I am writing this test to see what the test is going to see"
    newText.pop_back();
    assert(genNext(newText.back(), model, newText, 1) == 1);
    assert(newText.back().compare("do") == 0); //"<START> I am writing this test to see what the test is going to do"
    
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("<END>") == 0);//"<START> I am writing this test to see what the test is going to do <END>"
    
    //new test one letter
    v = {"<START> a", "a <END>"};
    model = emptyLM;
    updatePairCount(model, v);
    updateFollowWord(model, v);
    updateStartWord(model, v);
    
    assert(genNext("<START>", model, newText, 0) == 1);
    assert(newText.back().compare("a") == 0); //"<START> a"
    assert(genNext(newText.back(), model, newText, 0) == 1);
    assert(newText.back().compare("<END>") == 0); //"<START> a <END>"
}

int main() {
    
    cout << endl << "Running readPairs tests..." << endl;
    testReadPairs();
    cout << "readPairs tests passed." << endl << endl;
    
    cout << "Running updatePairCount tests..." << endl;
    testUpdatePairCount();
    cout << "updatePairCount tests passed." << endl << endl;
    
    cout << "Running updateStartWord tests..." << endl;
    testUpdateStartWord();
    cout << "updateStartWord tests passed." << endl << endl;
    
    cout << "Running updateFollowWord tests..." << endl;
    testUpdateFollowWord();
    cout << "updateFollowWord tests passed." << endl << endl;
    
    cout << "Running genNext tests..." << endl;
    testGenNext();
    cout << "genNext tests passed." << endl << endl;
    
    return 0;
}


