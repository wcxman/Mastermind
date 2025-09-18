#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <stdexcept>
#include <algorithm>
using namespace std;

class Code {
private:
    int n; // Code length
    int m; // Digit range [0, m-1]
    vector<int> secretCode;

public:
    // Constructor that is passed values n and m
    Code(int length, int range) : n(length), m(range) {
        if (n <= 0) throw invalid_argument("Length must be positive");
        if (m <= 0) throw invalid_argument("Range must be positive");
        initializeRandom(); // Initialize the code randomly
    }

    // Function that initializes the code randomly
    void initializeRandom() {
        secretCode.clear();
        for (int i = 0; i < n; i++) {
            secretCode.push_back(rand() % m);
        }
    }

    // Function to manually set code (for testing)
    void setCode(const vector<int>& newCode) {
        if (newCode.size() != n) throw invalid_argument("Code size mismatch");
        for (int digit : newCode) {
            if (digit < 0 || digit >= m) throw out_of_range("Digit out of range");
        }
        secretCode = newCode;
    }

    // Function checkCorrect: returns number of correct digits in correct location
    int checkCorrect(const Code& guess) const {
        if (guess.n != n) throw invalid_argument("Guess length doesn't match");
        if (guess.m != m) throw invalid_argument("Guess range doesn't match");
        
        int count = 0;
        for (int i = 0; i < n; i++) {
            if (secretCode[i] == guess.secretCode[i]) {
                count++;
            }
        }
        return count;
    }

    // Function checkIncorrect: returns number of correct digits in incorrect location
    int checkIncorrect(const Code& guess) const {
        if (guess.n != n) throw invalid_argument("Guess length doesn't match");
        if (guess.m != m) throw invalid_argument("Guess range doesn't match");
        
        vector<int> codeCount(m, 0);
        vector<int> guessCount(m, 0);
        
        // Count occurrences of each digit (excluding exact matches)
        for (int i = 0; i < n; i++) {
            if (secretCode[i] != guess.secretCode[i]) {
                codeCount[secretCode[i]]++;
                guessCount[guess.secretCode[i]]++;
            }
        }
        
        // Count minimum occurrences for each digit
        int count = 0;
        for (int i = 0; i < m; i++) {
            count += min(codeCount[i], guessCount[i]);
        }
        return count;
    }

    // Getter functions
    vector<int> getCode() const { return secretCode; }
    int getLength() const { return n; }
    int getRange() const { return m; }

    // Print the code
    void printCode() const {
        for (int digit : secretCode) {
            cout << digit << " ";
        }
    }
};

// Part 1: Main function for testing
void part1Main() {
    cout << "=== PART 1: Testing Code Class ===" << endl;
    
    // Initialize secret code with specific values for testing
    Code secretCode(5, 10);
    vector<int> secretDigits = {3, 1, 4, 1, 9};
    secretCode.setCode(secretDigits);
    
    cout << "Secret code: ";
    secretCode.printCode();
    cout << endl << endl;
    
    // Test case 1: (5, 0, 3, 2, 6)
    Code guess1(5, 10);
    vector<int> guess1Digits = {5, 0, 3, 2, 6};
    guess1.setCode(guess1Digits);
    
    cout << "Guess 1: ";
    guess1.printCode();
    cout << endl;
    cout << "Correct positions: " << secretCode.checkCorrect(guess1) << endl;
    cout << "Correct digits wrong position: " << secretCode.checkIncorrect(guess1) << endl << endl;
    
    // Test case 2: (2, 1, 2, 2, 2)
    Code guess2(5, 10);
    vector<int> guess2Digits = {2, 1, 2, 2, 2};
    guess2.setCode(guess2Digits);
    
    cout << "Guess 2: ";
    guess2.printCode();
    cout << endl;
    cout << "Correct positions: " << secretCode.checkCorrect(guess2) << endl;
    cout << "Correct digits wrong position: " << secretCode.checkIncorrect(guess2) << endl << endl;
    
    // Test case 3: (1, 3, 3, 4, 5)
    Code guess3(5, 10);
    vector<int> guess3Digits = {1, 3, 3, 4, 5};
    guess3.setCode(guess3Digits);
    
    cout << "Guess 3: ";
    guess3.printCode();
    cout << endl;
    cout << "Correct positions: " << secretCode.checkCorrect(guess3) << endl;
    cout << "Correct digits wrong position: " << secretCode.checkIncorrect(guess3) << endl;
}

class Response {
private:
    int correct;
    int incorrect;

public:
    // Constructor
    Response(int c = 0, int i = 0) : correct(c), incorrect(i) {}
    
    // Setter functions
    void setCorrect(int c) { correct = c; }
    void setIncorrect(int i) { incorrect = i; }
    
    // Getter functions
    int getCorrect() const { return correct; }
    int getIncorrect() const { return incorrect; }
};

// Overloaded operator == that compares responses
bool operator==(const Response& r1, const Response& r2) {
    return (r1.getCorrect() == r2.getCorrect()) &&
           (r1.getIncorrect() == r2.getIncorrect());
}

// Overloaded operator << that prints a response
ostream& operator<<(ostream& os, const Response& response) {
    os << response.getCorrect() << " correct position, "
       << response.getIncorrect() << " correct digit(s) in wrong position";
    return os;
}

class Mastermind {
private:
    Code secretCode;
    const int MAX_GUESSES = 10;

public:
    // Constructor with parameters from keyboard
    Mastermind(int n, int m) : secretCode(n, m) {}
    
    // Constructor with default values: n=5, m=10
    Mastermind() : secretCode(5, 10) {}
    
    // Function that prints the secret code
    void printSecretCode() const {
        cout << "Secret code: ";
        secretCode.printCode();
        cout << endl;
    }
    
    // Function humanGuess: reads a guess from keyboard and returns a code object
    Code humanGuess() const {
        string input;
        while (true) {
            cout << "Enter your guess (" << secretCode.getLength()
                 << " digits, range 0-" << (secretCode.getRange() - 1) << "): ";
            cin >> input;
            
            // Validate input length
            if (input.length() != secretCode.getLength()) {
                cout << "Error: Guess must be exactly " << secretCode.getLength()
                     << " digits. Please try again." << endl;
                continue;
            }
            
            // Validate each digit
            bool valid = true;
            vector<int> guessDigits;
            for (char c : input) {
                if (!isdigit(c)) {
                    cout << "Error: Guess must contain only digits. Please try again." << endl;
                    valid = false;
                    break;
                }
                int digit = c - '0';
                if (digit < 0 || digit >= secretCode.getRange()) {
                    cout << "Error: Digit " << digit << " is out of range [0, "
                         << (secretCode.getRange() - 1) << "]. Please try again." << endl;
                    valid = false;
                    break;
                }
                guessDigits.push_back(digit);
            }
            
            if (!valid) continue;
            
            // Create and return code object
            Code guess(secretCode.getLength(), secretCode.getRange());
            guess.setCode(guessDigits);
            return guess;
        }
    }
    
    // Function getResponse: returns response for a guess
    Response getResponse(const Code& guess) const {
        int correct = secretCode.checkCorrect(guess);
        int incorrect = secretCode.checkIncorrect(guess);
        return Response(correct, incorrect);
    }
    
    // Function isSolved: returns true if response indicates solved
    bool isSolved(const Response& response) const {
        return response.getCorrect() == secretCode.getLength();
    }
    
    // Function playGame: main game loop
    void playGame() {
        cout << "\n=== MASTERMIND GAME ===" << endl;
        printSecretCode(); // Show secret code for testing
        cout << "You have " << MAX_GUESSES << " guesses to break the code!" << endl;
        
        for (int attempt = 1; attempt <= MAX_GUESSES; attempt++) {
            cout << "\nAttempt #" << attempt << "/" << MAX_GUESSES << endl;
            
            Code guess = humanGuess();
            Response response = getResponse(guess);
            
            cout << "Response: " << response << endl;
            
            if (isSolved(response)) {
                cout << "\nCongratulations! You broke the code in "
                     << attempt << " attempts!" << endl;
                return;
            }
            
            if (attempt < MAX_GUESSES) {
                cout << "Attempts remaining: " << (MAX_GUESSES - attempt) << endl;
            }
        }
        
        cout << "\nGame over! You failed to break the code." << endl;
        printSecretCode();
    }
};

// Part 2: Main function for the complete game
void part2Main() {
    srand(time(0)); // Seed random number generator
    
    try {
        int n, m;
        cout << "\nEnter code length (n): ";
        cin >> n;
        cout << "Enter digit range (m): ";
        cin >> m;
        
        if (n <= 0 || m <= 0) {
            throw invalid_argument("Both values must be positive integers");
        }
        
        Mastermind game(n, m);
        game.playGame();
        
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

// Main function to run both parts
int main() {
    srand(time(0)); // Seed random number generator once
    
    // Run Part 1 tests
    part1Main();
    
    cout << "\n" << string(50, '=') << "\n" << endl;
    
    // Run Part 2 game
    part2Main();
    
    return 0;
}
