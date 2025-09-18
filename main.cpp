#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

class Code {
private:
    int n;                  // code length
    int m;                  // digit range [0..m-1]
    vector<int> secret;     // secret digits

public:
    // constructor: auto-generate random code
    Code(int length, int range) : n(length), m(range) {
        initializeRandom();
    }

    // fill with random digits
    void initializeRandom() {
        secret.clear();
        for (int i = 0; i < n; i++) {
            secret.push_back(rand() % m);
        }
    }

    // count digits that match in the same position
    int checkCorrect(const Code& guess) const {
        int correct = 0;
        for (int i = 0; i < n; i++) {
            if (secret[i] == guess.secret[i]) correct++;
        }
        return correct;
    }

    // count digits that are correct but in the wrong position
    int checkIncorrect(const Code& guess) const {
        vector<int> codeCount(m, 0), guessCount(m, 0);
        int incorrect = 0;

        for (int i = 0; i < n; i++) {
            if (secret[i] != guess.secret[i]) {
                codeCount[secret[i]]++;
                guessCount[guess.secret[i]]++;
            }
        }
        for (int d = 0; d < m; d++) {
            incorrect += min(codeCount[d], guessCount[d]);
        }
        return incorrect;
    }

    // helper: manually set code
    void setCode(const vector<int>& c) {
        if ((int)c.size() == n) secret = c;
    }

    // print out the code
    void printCode() const {
        for (int d : secret) cout << d << " ";
        cout << "\n";
    }
};

int main() {
    srand(time(0));

    // create a code of length 5, range 0-9
    Code secretCode(5, 10);
    // for demo, fix the code to something known
    secretCode.setCode({3, 1, 4, 1, 9});

    cout << "Secret code: ";
    secretCode.printCode();
    cout << endl;

    // test guesses
    vector<vector<int>> guesses = {
        {5, 0, 3, 2, 6},
        {2, 1, 2, 2, 2},
        {1, 3, 3, 4, 5}
    };

    for (int i = 0; i < (int)guesses.size(); i++) {
        Code guess(5, 10);
        guess.setCode(guesses[i]);

        cout << "Guess " << i + 1 << ": ";
        guess.printCode();

        int c = secretCode.checkCorrect(guess);
        int ic = secretCode.checkIncorrect(guess);

        cout << "  correct (right spot): " << c << "\n";
        cout << "  correct (wrong spot): " << ic << "\n\n";
    }

    return 0;
}

//Part b: function for playing
class Response {
private:
    int numCorrect;
    int numIncorrect;

public:
    // Constructor
    Response(int correct = 0, int incorrect = 0)
        : numCorrect(correct), numIncorrect(incorrect) {}

    // Getters
    int getCorrect() const { return numCorrect; }
    int getIncorrect() const { return numIncorrect; }

    // Setters
    void setCorrect(int c) { numCorrect = c; }
    void setIncorrect(int ic) { numIncorrect = ic; }

    // Equality operator
    friend bool operator==(const Response &r1, const Response &r2) {
        return (r1.numCorrect == r2.numCorrect &&
                r1.numIncorrect == r2.numIncorrect);
    }

    // Print operator
    friend ostream& operator<<(ostream &out, const Response &r) {
        out << "Correct: " << r.numCorrect << ", Incorrect: " << r.numIncorrect;
        return out;
    }
};

// Object Code
class Code {
private:
    vector<int> digits;   // secret code
    int length;

public:
    Code(int n = 5, int m = 10) : length(n) {
        digits.resize(n);
        srand((unsigned) time(0));
        for (int i = 0; i < n; i++) {
            digits[i] = rand() % m; // numbers in [0, m-1]
        }
    }

    Code(const vector<int>& guess) {
        digits = guess;
        length = (int) guess.size();
    }

    void printCode() const {
        for (int d : digits) cout << d << " ";
        cout << endl;
    }

    const vector<int>& getDigits() const { return digits; }

    // compare guess with secret -> generate response
    Response compareWith(const Code &guess) const {
        int correct = 0;
        int incorrect = 0;
        vector<bool> usedSecret(length, false);
        vector<bool> usedGuess(length, false);

        // first pass: correct position
        for (int i = 0; i < length; i++) {
            if (digits[i] == guess.digits[i]) {
                correct++;
                usedSecret[i] = true;
                usedGuess[i] = true;
            }
        }

        // second pass: correct digit, wrong place
        for (int i = 0; i < length; i++) {
            if (usedGuess[i]) continue;
            for (int j = 0; j < length; j++) {
                if (!usedSecret[j] && guess.digits[i] == digits[j]) {
                    incorrect++;
                    usedSecret[j] = true;
                    break;
                }
            }
        }

        return Response(correct, incorrect);
    }
};

// Game play class
class Mastermind {
private:
    Code secret;
    int n, m;

public:
    // default constructor
    Mastermind() : n(5), m(10), secret(n, m) {}

    // custom constructor
    Mastermind(int nVal, int mVal) : n(nVal), m(mVal), secret(n, m) {}

    void printSecret() const {
        cout << "Secret code: ";
        secret.printCode();
    }

    Code humanGuess() {
        vector<int> guessDigits;
        cout << "Enter your guess (" << n << " numbers between 0 and " << m-1 << "): ";
        for (int i = 0; i < n; i++) {
            int val;
            cin >> val;
            guessDigits.push_back(val);
        }
        return Code(guessDigits);
    }

    Response getResponse(const Code &guess) const {
        return secret.compareWith(guess);
    }

    bool isSolved(const Response &r) const {
        return r.getCorrect() == n;
    }

    void playGame() {
        printSecret();  // required for testing
        int attempts = 0;
        while (true) {
            attempts++;
            Code guess = humanGuess();
            Response r = getResponse(guess);
            cout << r << endl;

            if (isSolved(r)) {
                cout << "You solved it in " << attempts << " attempts!" << endl;
                break;
            }
        }
    }
};

//Output
int main() {
    Mastermind game(4, 6); // example: length=4, range=0..5
    game.playGame();
    return 0;
}
