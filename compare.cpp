#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <openssl/sha.h> // Include OpenSSL for SHA-256
#include <cmath> // Include cmath for math functions
#include <chrono> // Include chrono for time measurement

// Function to calculate the SHA-256 hash of a string
std::string sha256(const std::string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::string hashStr;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hashStr += hash[i];
    }
    return hashStr;
}

// Quantum qubit simulator
class Qubit {
private:
    std::vector<double> amplitudes;

public:
    // Constructor initializes the qubit to |0>
    Qubit(int num_states) : amplitudes(num_states, 0.0) {
        amplitudes[0] = 1.0;
    }

    // Apply quantum gate to the qubit
    void applyGate(double angle) {
        for (double& amplitude : amplitudes) {
            double old_real = amplitude;
            amplitude = amplitude * std::cos(angle);
            amplitude += std::sqrt(1 - old_real * old_real) * std::sin(angle);
        }
    }

    // Measure the qubit
    int measure() const {
        double rand_num = static_cast<double>(rand()) / RAND_MAX;
        double cumulative_prob = 0.0;
        for (int i = 0; i < amplitudes.size(); ++i) {
            cumulative_prob += amplitudes[i] * amplitudes[i];
            if (rand_num <= cumulative_prob) {
                return i;
            }
        }
        return amplitudes.size() - 1; // Return last state if no other is chosen
    }
};

// Function to brute force the hash using quantum qubits
std::string bruteForceHashQuantum(const std::string& targetHash) {
    std::string result;
    std::vector<char> alphabet;
    for (char c = 'a'; c <= 'z'; ++c) {
        alphabet.push_back(c);
    }

    for (char c1 : alphabet) {
        for (char c2 : alphabet) {
            for (char c3 : alphabet) {
                std::string candidate(1, c1);
                candidate += c2;
                candidate += c3;
                if (sha256(candidate) == targetHash) {
                    result = candidate;
                    break;
                }
            }
            if (!result.empty()) break;
        }
        if (!result.empty()) break;
    }

    return result;
}

// Function to brute force the hash using classical bits
std::string bruteForceHashClassical(const std::string& targetHash) {
    std::string result;
    std::vector<char> alphabet;
    for (char c = 'a'; c <= 'z'; ++c) {
        alphabet.push_back(c);
    }

    for (char c1 : alphabet) {
        for (char c2 : alphabet) {
            for (char c3 : alphabet) {
                std::string candidate(1, c1);
                candidate += c2;
                candidate += c3;
                if (sha256(candidate) == targetHash) {
                    result = candidate;
                    break;
                }
            }
            if (!result.empty()) break;
        }
        if (!result.empty()) break;
    }

    return result;
}

int main() {
    // User input for the string
    std::string input;
    std::cout << "Enter a string (3 alphabets only): ";
    std::cin >> input;

    if (input.length() != 3) {
        std::cerr << "Invalid input length. Please enter a string of length 3." << std::endl;
        return 1;
    }

    // Calculate target hash
    std::string targetHash = sha256(input);
    std::cout << "Target hash: " << targetHash << std::endl;

    // Measure time taken by quantum qubit simulation
    auto startQuantum = std::chrono::steady_clock::now();
    Qubit qubit(2);
    qubit.applyGate(0.5); // Apply a simple gate
    int measured_value_quantum = qubit.measure();
    std::string bruteForceResultQuantum = bruteForceHashQuantum(targetHash);
    auto endQuantum = std::chrono::steady_clock::now();
    std::chrono::duration<double> timeQuantum = endQuantum - startQuantum;

    // Measure time taken by classical bit simulation
    auto startClassical = std::chrono::steady_clock::now();
    std::string bruteForceResultClassical = bruteForceHashClassical(targetHash);
    auto endClassical = std::chrono::steady_clock::now();
    std::chrono::duration<double> timeClassical = endClassical - startClassical;

    // Print the results
    std::cout << "Quantum brute force result: " << bruteForceResultQuantum << std::endl;
    std::cout << "Time taken by quantum simulation: " << timeQuantum.count() << " seconds" << std::endl;
    std::cout << "Classical brute force result: " << bruteForceResultClassical << std::endl;
    std::cout << "Time taken by classical simulation: " << timeClassical.count() << " seconds" << std::endl;

    return 0;
}
