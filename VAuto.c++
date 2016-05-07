// Includes for code
#include <string>
#include <cctype>
#include <iterator>
#include <iostream>
#include <sstream>
#include <cctype>
#include <map>
#include <cassert>

/*
  Main design of my code.
  Need a function that takes a string and steps through chains of alpha characters.
  Once I encounter a non alpha replace with the length in between the start and the end.
  Replace all non alpha characters as you see them in their new spots. If the number of
  characters in between is larger then 9 we need to fill up multiple indicies to make up
  the number i.e. 11 takes up two characters in the string.
*/

using namespace std;

/*
  Argument takes in a string to be modified
  Return argument is string reference, the conversion. It's modifying
  the argument in place. Can not be called with an r-value
*/
string& convertTarget(string& input){
    map<char, int> distinctChar;
    // variable denotes if you are in a chain of alpha characters
    bool inChain = false;
    // variable denotes the start of a chain
    int startOfChain = 0;
    // variable denotes the length of a chain
    int chainLength = 0;
    // variable to denote the number of distinct characters
    int numOfDistinct = 0;

    // stepping through the input
    // This part is O(n), n is size of string
    for (int i = 0; i < input.length(); ++i)
    {
        if(!inChain)
        {
            // if we see an alpha we are on a new Chain
            if(isalpha(input[i]))
            {
                inChain = true;
            }
            // Replacing values to equal the new character it should be
            input[startOfChain++] = input[i];
        }
        else{
            if(!isalpha(input[i]))
            {
                // Non alpha terminates the chain, need to replace the word
                // in string
                switch(chainLength)
                {
                    case 0:
                        // If one character, replace in string at new position
                        input[startOfChain++] = input[i];
                        break;
                    case 1:
                        input[startOfChain++] = input[i-1];
                        input[startOfChain++] = input[i];
                        break;
                    default:
                        // If word, do a conversion into proper format and replace
                        // in string at new position
                        stringstream ss;
                        if(distinctChar[tolower(input[i - 1])] > 1)
                            ss << numOfDistinct;
                        else
                            ss << numOfDistinct - 1;
                        string buffer = ss.str();
                        for (int i = 0; i < buffer.length(); ++i)
                        {
                            input[startOfChain++] = buffer[i];
                        }
                        input[startOfChain++] = input[i - 1];
                        input[startOfChain++] = input[i];
                        break;
                }
                // End of chain, rest chain length
                inChain = false;
                chainLength = 0;
                numOfDistinct = 0;
                distinctChar.clear();
            }
            else
            {
                // incrementing the chain length until next non alpha
                // character
                // count only distinct characters
                if(distinctChar[tolower(input[i])] == 0)
                {
                    distinctChar[tolower(input[i])] = 1;
                    ++numOfDistinct;
                }
                else
                {
                    ++distinctChar[tolower(input[i])];
                }
                ++chainLength;
            }
        }
    }

    // If we exit the for loop and ended in a chain, final replacment
    if(inChain && chainLength)
    {
        if(chainLength > 1)
        {
            stringstream ss;
            if(distinctChar[tolower(input[input.length() - 1])] > 1)
                ss << numOfDistinct;
            else
                ss << numOfDistinct - 1;
            string buffer = ss.str();
            for (int i = 0; i < buffer.length(); ++i)
            {
                input[startOfChain++] = buffer[i];
            }
        }
        input[startOfChain++] = input[input.length() - 1];
    }

    // deleting trailing characters, this method call is O(n), where n
    // the size of the new string
    return input.erase(startOfChain, input.length() - startOfChain);
}

/*
  Takes an r-value reference to modify an string literal
*/
string convertTarget(string&& input){
    // string move constructor called to create a variable of the
    // r-value reference
    string copyInput(input);
    return convertTarget(copyInput);
}

int main(int argc, char const *argv[]){
    string str("Andrew Vohl");
    convertTarget(str);
    cout << str << endl;
    string str2("Sue Anne Richardson Davis");
    cout << convertTarget(str2) << endl;
    // r-value call
    cout << convertTarget("This Needs To WORK") << endl;
    cout << convertTarget("This is going to be a long woooooorrrrrrrrrddddd") << endl;
    string example("Automotive parts");
    cout << convertTarget(example) << endl;
    cout << convertTarget("Noow thiss issss gooing tooo be distinct. ABCDEFGHIJKLMNOPQRSTUVWXYZ") << endl;
    return 0;
}
