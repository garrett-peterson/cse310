#include <iostream>
#include <string>
#include <list>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class HangmanGame
{
private:
    string secretWord;
    string hiddenWord;
    list<char> letterList;
    list<char> correctList;
    list<char> guessedList;

    int guessesLeft;
    bool gameOver;

public:
    HangmanGame(string word);

    string hideWord(const string& input);

    void displayGuessedLetters();

    bool alreadyGuessed(char guess);
    bool isCorrectGuess(char guess);

    void revealLetter(char guess);
    void handleCorrectGuess(char guess);
    void handleWrongGuess(char guess);

    void saveResult(const string& filename, bool won);

    bool hasWon();
    bool hasLost();

    void play();
};


// Constructor
HangmanGame::HangmanGame(string word)
{
    secretWord = word;
    hiddenWord = hideWord(word);

    letterList = list<char>(word.begin(), word.end());

    guessesLeft = 10;
    gameOver = false;
}


// Creates the hidden version of the word
string HangmanGame::hideWord(const string& input)
{
    return string(input.length(), '_');
}


// Displays all incorrect guessed letters
void HangmanGame::displayGuessedLetters()
{
    for (char c : guessedList)
    {
        cout << c << ", ";
    }

    cout << endl;
}


// Checks whether the user already guessed the letter
bool HangmanGame::alreadyGuessed(char guess)
{
    if (std::ranges::find(guessedList, guess) != guessedList.end() ||
        std::ranges::find(correctList, guess) != correctList.end())
    {
        return true;
    }

    return false;
}


// Checks whether the guess is in the remaining letters
bool HangmanGame::isCorrectGuess(char guess)
{
    return std::ranges::find(letterList, guess) != letterList.end();
}


// Reveals all matching letters in the hidden word
void HangmanGame::revealLetter(char guess)
{
    for (int i = 0; i < secretWord.length(); i++)
    {
        if (secretWord[i] == guess)
        {
            if (i == 0)
            {
                hiddenWord[i] = toupper(guess);
            }
            else
            {
                hiddenWord[i] = guess;
            }
        }
    }
}


// Handles a correct guess
void HangmanGame::handleCorrectGuess(char guess)
{
    letterList.remove(guess);

    correctList.push_back(guess);

    revealLetter(guess);

    if (hasWon())
    {
        gameOver = true;

        cout << "You guessed the word: " << secretWord << endl;

        saveResult("Sprint1/gameResults.txt", true);
    }
    else
    {
        cout << "You got one right" << endl;
    }
}


// Handles an incorrect guess
void HangmanGame::handleWrongGuess(char guess)
{
    guessesLeft--;

    guessedList.push_back(guess);

    cout << "Remaining guesses: " << guessesLeft << endl;
}


// Checks whether the player won
bool HangmanGame::hasWon()
{
    return letterList.size() == 0;
}


// Checks whether the player lost
bool HangmanGame::hasLost()
{
    return guessesLeft == 0;
}


// Runs the game
void HangmanGame::play()
{
    string input;

    while (!gameOver)
    {
        if (hasLost())
        {
            cout << "You ran out of guesses. The word was: "
                 << secretWord << endl;

            saveResult("Sprint1/gameResults.txt", false);

            gameOver = true;
        }
        else
        {
            cout << hiddenWord << endl;

            cout << "Guess a letter: ";
            cin >> input;

            if (input.length() != 1)
            {
                cout << "Please enter exactly one letter." << endl;
                continue;
            }

            if (!isalpha(input[0]))
            {
                cout << "Please enter a letter, not a number or symbol." << endl;
                continue;
            }

            char guess = tolower(input[0]);

            if (alreadyGuessed(guess))
            {
                cout << "You already guessed "
                     << guess << endl;
            }
            else if (isCorrectGuess(guess))
            {
                handleCorrectGuess(guess);
            }
            else
            {
                handleWrongGuess(guess);
            }
        }

        if (!gameOver)
        {
            cout << "Guessed Letters:" << endl;
            displayGuessedLetters();

            cout << endl;
        }
    }
}


// Loads possible words from a file
vector<string> loadWords(const string& filename)
{
    vector<string> words;
    ifstream file(filename);

    string word;

    while (file >> word)
    {
        words.push_back(word);
    }

    file.close();

    return words;
}


// Saves the result of the game to a file
void HangmanGame::saveResult(const string& filename, bool won)
{
    ofstream file(filename, ios::app);

    file << "Word: " << secretWord << endl;

    if (won)
    {
        file << "Result: Won" << endl;
    }
    else
    {
        file << "Result: Lost" << endl;
    }

    file << "Guesses Remaining: " << guessesLeft << endl;
    file << "--------------------" << endl;

    file.close();
}


int main()
{
    srand(time(0));

    vector<string> words = loadWords("Sprint1/words.txt");

    if (words.empty())
    {
        cout << "Could not find any words in words.txt" << endl;
        return 1;
    }

    string randomWord = words[rand() % words.size()];

    HangmanGame game(randomWord);

    game.play();

    return 0;
}