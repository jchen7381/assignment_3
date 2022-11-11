// Jason Chen
// spell_check.cc: A simple spell checker.

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

// You can change to quadratic probing if you
// haven't implemented double hashing.
#include "double_hashing.h"
using namespace std;

// You can add more functions here.

// Creates and fills double hashing hash table with all words from
// dictionary_file
HashTableDouble<string> MakeDictionary(const string &dictionary_file) {
  HashTableDouble<string> dictionary_hash;
  // Fill dictionary_hash.
    dictionary_hash.MakeEmpty();
    //..Insert your own code
    fstream dictionary;

    dictionary.open("wordsEN.txt");
    if(dictionary.is_open()){
        for(string word; getline(dictionary, word);){
            dictionary_hash.Insert(word);
            
        }
    }
    
  return dictionary_hash;
}

// For each word in the document_file, it checks the 3 cases for a word being
// misspelled and prints out possible corrections
void SpellChecker(const HashTableDouble<string>& dictionary,
		  const string &document_file) {
    
    fstream document;
    document.open("document1.txt");
   
    if(document.is_open()){
        for(string line_of_sentence; getline(document, line_of_sentence);){
            std::stringstream ss(line_of_sentence);
            string word;
            while(getline(ss, word, (' '))){
                for(unsigned int i = 0; i <= word.size(); i++){
                    if(ispunct(word[i])){
                        word.erase(i--,1);
                    }
                }
                
                if(dictionary.Contains(word)){
                    cout << word << " is CORRECT" << endl;
                }
                else{
                    cout << word << " is INCORRECT" << endl;
                }
            }
        }
    }
    document.close();
    cout << endl;

}

// @argument_count: same as argc in main
// @argument_list: save as argv in main.
// Implements
int testSpellingWrapper(int argument_count, char** argument_list) {
    const string document_filename(argument_list[1]);
    const string dictionary_filename(argument_list[2]);

    // Call functions implementing the assignment requirements.
    HashTableDouble<string> dictionary = MakeDictionary(dictionary_filename);
    SpellChecker(dictionary, document_filename);

    return 0;
}

// Sample main for program spell_check.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testSpellingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>"
         << endl;
    return 0;
  }
  
  testSpellingWrapper(argc, argv);
  
  return 0;
}
