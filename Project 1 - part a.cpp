#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

//Declare Code will encapsulate data and behaviour related to the digits, the length, and operations
class Code {
private:
    int n; //store the code length
    int m; //store the digit range [0, m-1]
    vector<int> secret; //secret digits

public:
    //random codes and digits
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

    void initializeRandom() {
        secret.clear();
        for (int i = 0; i < n; i++) {
            secret.push_back(rand() % m);
        }
    }
    //count digits that match same position
    int checkCorrect(const Code& guess) const {
        if (guess.secret.size() != secret.size()) {
            throw std::invalid_argument("Guess and Secret Code lengths must be the same");
        }
        else {
            int correct = 0;
            for (int i = 0; i < n; i++) {
                if (secret[i] == guess.secret[i]) correct++;
            }
            return correct;
        }
        return -1;
    }

    //count digits correct but in the wrong position
    //Initialize counters for each possible digit in the range
    int checkIncorrect(const Code& guess) const {
        if (guess.secret.size() != secret.size()) {
            throw std::invalid_argument("Guess and Secret Code lengths must be the same");
        }
        else {
            vector<int> codeCount(m, 0), guessCount(m, 0);
            int incorrect = 0;
            //secret[i] and guess.secret[i] is the digit in the secret code that is not in the correct position
            for (int i = 0; i < n; i++) {
                if (secret[i] != guess.secret[i]) {
                    codeCount[secret[i]]++;
                    guessCount[guess.secret[i]]++;
                }
            }
            //Compare counts of the number of not matched for digit d is the minimum of how many times it appears in secret and guess
            for (int d = 0; d < m; d++) {
                incorrect += min(codeCount[d], guessCount[d]);
            }
            return incorrect;
        }
        return -1;
    }

    //manually set code
    void setCode(const vector<int>& c) {
        if ((int)c.size() == n) secret = c;
    }

    //print out
    void printCode() const {
        for (int d : secret) cout << d << " ";
        cout << "\n";
    }
};

int main() {
    srand(time(0));
    try {
        //create a code of length 5, range 0-9
        Code secretCode(5, 10);

        cout << "Secret code: ";
        secretCode.printCode();
        cout << endl;

        //test sample guess from part a. instruction
        vector<vector<int>> guesses = {
            {5, 0, 3, 2, 6},
            {2, 1, 2, 2, 2},
            {1, 3, 3, 4, 5}
        };
        //Create Code object for the sample guess
        for (int i = 0; i < (int)guesses.size(); i++) {
            Code guess(5, 10);
            guess.setCode(guesses[i]); //Set the guess digits from the guesses vector

            cout << "Guess " << i + 1 << ": ";
            guess.printCode();
            //Compare the guess with the secret code, and c counts digits correct and in the right position
            //ic counts digits correct but in the wrong position
            int c = secretCode.checkCorrect(guess);
            int ic = secretCode.checkIncorrect(guess);
            //Print out the results
            cout << "  correct (right spot): " << c << "\n";
            cout << "  correct (wrong spot): " << ic << "\n\n";
        }
    }
    catch (const std::invalid_argument& e) {
        cout << e.what();
    }
    return 0;
}
