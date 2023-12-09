#include <iostream>
#include <random>

using namespace std;

bool withProbability(double p) {
    random_device rd; // random device object
    mt19937 gen(rd()); //initializes it with a seed obtained from the random device.
    uniform_real_distribution<> dis(0, 1); // declares a uniform real distribution with a range from 0 to 1
    return dis(gen) <= p;
}

int main() {
    double probability;
    cout << "Enter the probability: ";
    cin >> probability;

    if (withProbability(probability)) {
        cout << "The robot failed!" << endl;
    } else {
        cout << "Working..." << endl;
    }

    return 0;
}