#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <ctype.h>

// Define the qubit structure
typedef struct {
    double alpha; // Complex coefficient for |0>
    double beta;  // Complex coefficient for |1>
} Qubit;

// Function to initialize a qubit
Qubit* initializeQubit(double alpha, double beta) {
    Qubit* qubit = (Qubit*)malloc(sizeof(Qubit));
    qubit->alpha = alpha;
    qubit->beta = beta;
    return qubit;
}

// Function to measure a qubit
int measureQubit(Qubit* qubit) {
    double prob_0 = qubit->alpha * qubit->alpha;
    double prob_1 = qubit->beta * qubit->beta;
    double rand_num = (double)rand() / RAND_MAX;

    if (rand_num < prob_0) {
        return 0; // Measured |0>
    } else {
        return 1; // Measured |1>
    }
}

// Function to apply the Pauli-X gate to a qubit (bit flip)
void applyPauliXGate(Qubit* qubit) {
    double temp = qubit->alpha;
    qubit->alpha = qubit->beta;
    qubit->beta = temp;
}

// Function to convert a string to MD5 hash
void md5Hash(const char *str, unsigned char *hash) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
    EVP_DigestUpdate(ctx, str, strlen(str));
    EVP_DigestFinal_ex(ctx, hash, NULL);
    EVP_MD_CTX_free(ctx);
}

// Function to simulate a brute-force attack using qubits
void bruteForceMD5(const char *target_hash) {
    char candidate[4] = {'a', 'a', 'a', '\0'}; // Start with 'aaa'
    unsigned char hash[MD5_DIGEST_LENGTH];

    // Iterate through all possible strings
    while (1) {
        // Calculate the MD5 hash of the candidate string
        md5Hash(candidate, hash);

        // Check if the hash matches the target hash
        int match = 1;
        for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
            if (hash[i] != target_hash[i]) {
                match = 0;
                break;
            }
        }

        // If a match is found, print the encoded message and the hash
        if (match) {
            printf("Encoded message: %s\n", candidate);
            printf("Hash: ");
            for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
                printf("%02x", hash[i]);
            }
            printf("\n");
            break;
        }

        // Increment the candidate string (brute-force)
        for (int i = 2; i >= 0; i--) {
            if (candidate[i] == 'z') {
                candidate[i] = 'a'; // Reset to 'a' if 'z' is reached
            } else {
                candidate[i]++;
                break;
            }
        }
    }
}

int main() {
    // Prompt the user to input a string
    char input_string[4]; // Restrict input to 3 characters
    printf("Enter a string (3 alphabetic characters only): ");
    scanf("%3s", input_string);

    // Convert input string to uppercase
    for (int i = 0; i < 3; i++) {
        input_string[i] = toupper(input_string[i]);
    }

    // Calculate the MD5 hash of the input string
    unsigned char target_hash[MD5_DIGEST_LENGTH];
    md5Hash(input_string, target_hash);

    // Print the target hash
    printf("Target MD5 hash: ");
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        printf("%02x", target_hash[i]);
    }
    printf("\n");

    // Simulate a brute-force attack using qubits
    bruteForceMD5(target_hash);

    return 0;
}
