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

    dictionary.open(dictionary_file);
    if(dictionary.is_open()){                       //creating a hash table for dictionary
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
    document.open(document_file);
   
    if(document.is_open()){
        for(string line_of_sentence; getline(document, line_of_sentence);){
            stringstream ss(line_of_sentence);      //reading file line by line
            string word;
            while(ss >> word){              //converting each line into words
                    
                for(unsigned int i = 0; i < word.size(); i++){
                    if(isupper(word[i])){                       //parsing line base on space, comma, and period
                        word[i] = tolower(word[i]);
                    }
                    else if(word[i] == (',')){
                        word.erase(i,1);
                    }
                    else if(word[i] == ('.')){
                        word.erase(i,1);
                    }
                }
                if(dictionary.Contains(word)){                        //if the word is found in the dictionary cout "CORRECT"
                    cout << word << " is CORRECT" << endl;
                }
                else{
                    cout << word << " is INCORRECT" << endl;
                    //CASE A adding on char in any possible position
                    string case_a_word = word;
                    string alphabet_string = "abcdefghijklmnopqrstuvwxyz";
                    for(unsigned int i = 0; i <= word.size(); ++i){
                        for(unsigned int j = 0; j <= alphabet_string.size(); ++j){
                            string s;
                            stringstream ss;                //converting char into string
                            ss << alphabet_string[j];
                            ss >> s;
                            case_a_word.insert(i,s);        //inserting string into word
                            if(dictionary.Contains(case_a_word)){
                                cout << "** " << word << " -> " << case_a_word << " ** case A" << endl;
                            }
                            case_a_word = word;
                        }
                    }
                    //CASE B removing one charactor from the word using for loop
                    string case_b_word = word;
                    for(unsigned int i = 0; i <= word.size(); ++i){
                        case_b_word.erase(i,1);
                        if(dictionary.Contains(case_b_word)){
                            cout << "** " << word << " -> " << case_b_word << " ** case B" << endl;
                        }
                        case_b_word = word;
                    }
                    //CASE C swapping adjects characters in the word
                    string case_c_word = word;
                    for(unsigned int i = 1; i <= word.size(); ++i){
                        swap(case_c_word[i],case_c_word[i+1]);
                        if(dictionary.Contains(case_c_word)){
                            cout << "** " << word << " -> " << case_c_word << " ** case C" << endl;
                        }
                        case_c_word = word;
                    }
                }
            }
        }
        document.close();
    }
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
