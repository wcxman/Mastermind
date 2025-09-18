#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

class Code {
private:
    int n;                  //code length
    int m;                  //digit range [0, m-1]
    vector<int> secret;     //secret digits

public:
    //random codes
    Code(int length, int range) : n(length), m(range) {
        initializeRandom();
    }
    //random digits
    void initializeRandom() {
        secret.clear();
        for (int i = 0; i < n; i++) {
            secret.push_back(rand() % m);
        }
    }

    //count digits that match in the same position
    int checkCorrect(const Code& guess) const {
        int correct = 0;
        for (int i = 0; i < n; i++) {
            if (secret[i] == guess.secret[i]) correct++;
        }
        return correct;
    }

    //count digits that are correct but in the wrong position
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

    //create a code of length 5, range 0-9
    Code secretCode(5, 10);
    //fix the code to something known
    secretCode.setCode({3, 1, 4, 1, 9});

    cout << "Secret code: ";
    secretCode.printCode();
    cout << endl;

    //test
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
