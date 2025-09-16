#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <stdexcept>
using namespace std;



class code {
private:
	int m, n;
	std::vector<int> sequence;
	void initialize(int length, int range, std::vector<int>& empty) {
		for (int i = 0; i < length; i++) {
			empty.push_back((rand() % (range + 1)));
		}
	}
public:
	code(int length, int range) {
		if (length <= 0) {
			throw std::invalid_argument("Length must be positive");
		}
		else if (range <= 0) {
			throw std::invalid_argument("Range must be positive");
		}
		else {
			m = range;
			n = length;
			initialize(n, m, sequence);
		}
	}

	int checkCorrect(std::vector<int>& guess) {
		if (guess.size() != sequence.size()) {
			throw std::invalid_argument("Guess is wrong size");
		}
		else {
			int tor = 0;
			for (int i = 0; i < guess.size(); i++) {
				if (guess[i] == sequence[i]) {
					tor++;
				}
			}
			return tor;
		}
		return -1;
	}

	int checkInorrect(std::vector<int> guess) {
		std::vector<int> temp = getseq();
		if (guess.size() != temp.size()) {
			throw std::invalid_argument("Guess is wrong size");
		}
		else {
			int tor = 0;
			for (int i = 0; i < guess.size(); i++) {
				int loc = find(temp.front(), temp.back(), guess[i]);
				//TO ADD: vector of found indicies to prevent retracing steps
			}
			return tor;
		}
		return -1;
	}

	std::vector<int> getseq() {
		return sequence;
	}

	//debugging
	int getm() {
		return m;
	}

	int getn() {
		return n;
	}

	
};

int main() {
	srand(time(0));
	try {
		code mycode = code(8, 2);
		cout << mycode.getm() << endl;	
		cout << mycode.getn() << endl;
		std::vector<int> mystery = mycode.getseq();
		std::vector<int> guess = { 1,1,1,1,1 };
		for (int i = 0; i < mystery.size(); i++) {
			cout << mystery[i];
		}
		cout << endl << mycode.checkCorrect(guess);
	}
	catch (const std::invalid_argument& e) {
		cout << e.what();
	}
}