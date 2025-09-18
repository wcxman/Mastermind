//Part b: function for playing
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

//Forward declaration
class code;

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

class code {
private:
    int n;
    int m;
    vector<int> digits;

public:
    //constructor
    code(int length, int range) : n(length), m(range) {
        if (n <= 0 || m <= 0) throw invalid_argument("Length and range must be positive");
        initializeRandom();
    }

    void initializeRandom() {
        digits.clear();
        for (int i = 0; i < n; i++) {
            digits.push_back(rand() % m);
        }
    }

    int checkCorrect(const code& guess) const {
        if (guess.n != n) throw invalid_argument("Guess length does not match");
        int count = 0;
        for (int i = 0; i < n; i++) {
            if (digits[i] == guess.digits[i]) {
                count++;
            }
        }
        return count;
    }

    int checkIncorrect(const code& guess) const {
        if (guess.n != n) throw invalid_argument("Guess length does not match");
        vector<int> secretCount(m, 0);
        vector<int> guessCount(m, 0);
        
        for (int i = 0; i < n; i++) {
            if (digits[i] != guess.digits[i]) {
                secretCount[digits[i]]++;
                guessCount[guess.digits[i]]++;
            }
        }
        
        int count = 0;
        for (int i = 0; i < m; i++) {
            count += min(secretCount[i], guessCount[i]);
        }
        return count;
    }

    void setCode(const vector<int>& c) {
        if (c.size() != n) throw invalid_argument("Code size does not match");
        digits = c;
    }

    void printCode() const {
        for (int d : digits) {
            cout << d << " ";
        }
        cout << endl;
    }

    const vector<int>& getDigits() const { return digits; }
    int getLength() const { return n; }
    int getRange() const { return m; }
};

class mastermind {
private:
    code secret;
    int n;
    int m;
    static const int MAX_ATTEMPTS = 10;

public:
    //two constructors
    mastermind() : n(5), m(10), secret(5, 10) {}  //default values
    
    mastermind(int n_val, int m_val) : n(n_val), m(m_val), secret(n_val, m_val) {}
    
    //prints the secret code
    void printSecretCode() const {
        cout << "Secret code: ";
        secret.printCode();
    }
    
    //reads a guess from keyboard
    code humanGuess() {
        vector<int> guessDigits;
        cout << "Enter your guess (" << n << " digits, 0 to " << m-1 << "): ";
        
        for (int i = 0; i < n; i++) {
            int digit;
            while (true) {
                cin >> digit;
                if (cin.fail() || digit < 0 || digit >= m) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid digit. Enter a number between 0 and " << m-1 << ": ";
                } else {
                    break;
                }
            }
            guessDigits.push_back(digit);
        }
        
        code guess(n, m);
        guess.setCode(guessDigits);
        return guess;
    }
    
    //returns a response
    response getResponse(const code& guess) {
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
        printSecretCode(); // Print secret code as required
        
        for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
            cout << "\nAttempt " << attempt << " of " << MAX_ATTEMPTS << endl;
            
            code guess = humanGuess();
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
    cout << "Enter code length (n): ";
    cin >> n;
    cout << "Enter digit range (m): ";
    cin >> m;
    
    try {
        mastermind game(n, m);
        game.playGame();
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
