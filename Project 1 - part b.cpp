//Part b: function for playing
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

//Forward declaration
class Code;

class response {
private:
    int correct;
    int incorrect;

public:
    //constructor
    response(int c = 0, int i = 0) : correct(c), incorrect(i) {}

    //set and get functions
    void setCorrect(int c) { correct = c; }
    void setIncorrect(int i) { incorrect = i; }
    int getCorrect() const { return correct; }
    int getIncorrect() const { return incorrect; }
};

//overloaded operator == (global)
bool operator==(const response& r1, const response& r2) {
    return r1.getCorrect() == r2.getCorrect() &&
        r1.getIncorrect() == r2.getIncorrect();
}

//overloaded operator << (global)
ostream& operator<<(ostream& os, const response& r) {
    os << r.getCorrect() << " correct, " << r.getIncorrect() << " incorrect";
    return os;
}

//Declare Code will encapsulate data and behaviour 
// related to the digits, the length, and operations
class Code {
private:
    int n; //store the Code length
    int m; //store the digit range [0, m-1]
    vector<int> secret; //secret digits

public:
    //random Codes and digits
    Code(int length, int range) {
        if (length <= 0) {
            throw std::invalid_argument("Length must be positive");
        }
        else if (range <= 0) {
            throw std::invalid_argument("Range must be positive");
        }
        else {
            m = range;
            n = length;
            initializeRandom();
        }
    }

    //initializes the secret Code
    void initializeRandom() {
        secret.clear();
        for (int i = 0; i < n; i++) {
            secret.push_back(rand() % m);
        }
    }
    //count digits that match same position
    int checkCorrect(const Code& guess) const {
        vector<int> guessCode = guess.getCode();
        if (guessCode.size() != secret.size()) {
            throw std::invalid_argument("Guess and Secret Code lengths must be the same");
        }
        else {
            int correct = 0;
            for (int i = 0; i < n; i++) {
                if (secret[i] == guessCode[i]) correct++;
            }
            return correct;
        }
        return -1;
    }

    //count digits correct but in the wrong position
    //Initialize counters for each possible digit in the range
    int checkIncorrect(const Code& guess) const {
        vector<int> guessCode = guess.getCode();
        if (guessCode.size() != secret.size()) {
            throw std::invalid_argument("Guess and Secret Code lengths must be the same");
        }
        else {
            vector<int> CodeCount(m, 0), guessCount(m, 0);
            int incorrect = 0;
            //secret[i] and guess.secret[i] is the digit in the secret 
            // Code that is not in the correct position
            for (int i = 0; i < n; i++) {
                if (secret[i] != guessCode[i]) {
                    CodeCount[secret[i]]++;
                    guessCount[guessCode[i]]++;
                }
            }
            //Compare counts of the number of not matched for digit d is the
            // minimum of how many times it appears in secret and guess
            for (int d = 0; d < m; d++) {
                incorrect += min(CodeCount[d], guessCount[d]);
            }
            return incorrect;
        }
        return -1;
    } //End checkIncorrect

    //manually set Code
    void setCode(const vector<int>& c) {
        if ((int)c.size() == n) secret = c;
    }

    //print out
    void printCode() const {
        for (int d : secret) cout << d << " ";
        cout << "\n";
    }

    //get length
    int getLength() const {
        return n;
    }

    //get range
    int getRange() const {
        return m;
    }

    //get Code
    vector<int> getCode() const {
        return secret;
    }
};

class mastermind {
private:
    Code secret;
    int n;
    int m;
    static const int MAX_ATTEMPTS = 10;

public:
    //two constructors
    mastermind() : n(5), m(10), secret(5, 10) {}  //default values

    mastermind(int n_val, int m_val) : n(n_val), m(m_val), secret(n_val, m_val) {}

    //prints the secret Code
    void printSecretCode() const {
        cout << "Secret Code: ";
        secret.printCode();
    }

    //reads a guess from keyboard
    Code humanGuess() {
        vector<int> guessDigits;
        cout << "Enter your guess (" << n << " digits, 0 to " << m - 1 << "): ";

        for (int i = 0; i < n; i++) {
            int digit;
            while (true) {
                cin >> digit;
                if (cin.fail() || digit < 0 || digit >= m) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid number, please enter a number between 0 and " << m - 1 << ": ";
                }
                else {
                    break;
                }
            }
            guessDigits.push_back(digit);
        }

        Code guess(n, m);
        guess.setCode(guessDigits);
        return guess;
    }

    //returns a response
    response getResponse(const Code& guess) {
        int correct = secret.checkCorrect(guess);
        int incorrect = secret.checkIncorrect(guess);
        return response(correct, incorrect);
    }

    //func isSolved() 
    bool isSolved(const response& r) const {
        return r.getCorrect() == n;
    }

    //func playGame()
    void playGame() {
        cout << "Welcome to Mastermind!" << endl;
        cout << "---------------------" << endl;
        printSecretCode(); // Print secret Code as required

        for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
            cout << "\nAttempt " << attempt << " of " << MAX_ATTEMPTS << endl;

            Code guess = humanGuess();
            response r = getResponse(guess);

            cout << "Response: " << r << endl;

            if (isSolved(r)) {
                cout << "\nCongratulations! You solved it in " << attempt << " attempts!" << endl;
                return;
            }

            cout << "Attempts remaining: " << (MAX_ATTEMPTS - attempt) << endl;
        }

        cout << "\nGame over! You are failed." << endl;
        printSecretCode();
    }
};

//func main
int main() {
    srand(time(0));

    int n, m;
    cout << "Enter Code length (n): ";
    cin >> n;
    cout << "Enter digit range (m): ";
    cin >> m;

    try {
        mastermind game(n, m);
        game.playGame();
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
