#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/evp.h> // Include the OpenSSL EVP header for MD5 functions

#define NUM_QUBITS 32 // Assuming each character of the input string is represented by a qubit

// Define qubit structure
typedef struct {
    double amplitude[2]; // Real amplitudes for |0⟩ and |1⟩ states
    double eigenvalues[2]; // Eigenvalues for |0⟩ and |1⟩ states
    int state; // Current state of the qubit: 0 or 1
} qubit;

// Function to create a new qubit
qubit* create_qubit() {
    qubit* q = (qubit*)malloc(sizeof(qubit));
    if (q == NULL) {
        printf("Error: Failed to allocate memory for qubit.\n");
        exit(1);
    }
    // Initialize with |0⟩ state
    q->amplitude[0] = 1.0;
    q->amplitude[1] = 0.0;
    q->eigenvalues[0] = 1.0;
    q->eigenvalues[1] = 0.0;
    q->state = 0;
    return q;
}

// Function to free memory allocated for a qubit
void destroy_qubit(qubit* q) {
    free(q);
}

// Function to apply Hadamard gate to a qubit
void hadamard_gate(qubit* q) {
    // Hadamard gate matrix
    double H[2][2] = {
        {1.0/sqrt(2), 1.0/sqrt(2)},
        {1.0/sqrt(2), -1.0/sqrt(2)}
    };
    // Apply Hadamard gate to the qubit
    double result[2] = {0.0, 0.0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result[i] += H[i][j] * q->amplitude[j];
        }
    }
    q->amplitude[0] = result[0];
    q->amplitude[1] = result[1];
    q->eigenvalues[0] = result[0] * result[0];
    q->eigenvalues[1] = result[1] * result[1];
    q->state = (q->amplitude[0] > q->amplitude[1]) ? 0 : 1;
}

// Function to measure a qubit (collapse its state to either |0⟩ or |1⟩)
int measure_qubit(qubit* q) {
    // Generate a random value between 0 and 1
    double random_val = (double)rand() / RAND_MAX;
    // Collapse the qubit's state based on the random value
    int result = (random_val < q->amplitude[0]) ? 0 : 1;
    // Update the qubit's state
    if (result == 0) {
        q->amplitude[1] = 0.0;
        q->eigenvalues[1] = 0.0;
    } else {
        q->amplitude[0] = 0.0;
        q->eigenvalues[0] = 0.0;
    }
    q->state = result;
    return result;
}

// Function to generate MD5 hash for a given input string
void md5_hash(const char *input, unsigned char *digest) {
    EVP_MD_CTX *context = EVP_MD_CTX_new(); // Create a new EVP_MD_CTX object
    EVP_DigestInit_ex(context, EVP_md5(), NULL); // Initialize the context with the MD5 algorithm
    EVP_DigestUpdate(context, input, strlen(input)); // Update the context with the input string
    EVP_DigestFinal_ex(context, digest, NULL); // Finalize the context and obtain the digest
    EVP_MD_CTX_free(context); // Free the EVP_MD_CTX object
}

// Function to brute-force a given MD5 hash using qubits
void brute_force_md5_qubits(const char *target_hash) {
    // Seed random number generator
    srand(time(NULL));

    // Create qubits for each character in the input string
    qubit* qubits[NUM_QUBITS];
    for (int i = 0; i < NUM_QUBITS; i++) {
        qubits[i] = create_qubit();
    }

    // Apply Hadamard gate to each qubit
    for (int i = 0; i < NUM_QUBITS; i++) {
        hadamard_gate(qubits[i]);
    }

    // Generate binary representation of each character and measure qubits
    unsigned char hash[MD5_DIGEST_LENGTH];
    int found_match = 0; // Flag to indicate if a matching input is found
    clock_t start_time = clock(); // Start the timer
    do {
        char input_str[NUM_QUBITS + 1];
        input_str[NUM_QUBITS] = '\0';
        for (int i = 0; i < NUM_QUBITS; i++) {
            int measurement = measure_qubit(qubits[i]);
            input_str[i] = (measurement == 0) ? '0' : '1';
        }
        // Compute MD5 hash for the generated input string
        md5_hash(input_str, hash);
        // Compare the computed hash with the target hash
        if (memcmp(hash, target_hash, MD5_DIGEST_LENGTH) == 0) {
            printf("Matching input found using qubits: %s\n", input_str);
            found_match = 1;
            break; // Break out of the loop once a matching input is found
        }
    } while (!found_match);
    clock_t end_time = clock(); // Stop the timer

    // Print qubit values
    for (int i = 0; i < NUM_QUBITS; i++) {
        printf("Qubit %d: |0⟩ amplitude = %f, |1⟩ amplitude = %f, Eigenvalues: |0⟩ = %f, |1⟩ = %f, State: %d\n", i, qubits[i]->amplitude[0], qubits[i]->amplitude[1], qubits[i]->eigenvalues[0], qubits[i]->eigenvalues[1], qubits[i]->state);
    }

    // Free memory allocated for qubits
    for (int i = 0; i < NUM_QUBITS; i++) {
        destroy_qubit(qubits[i]);
    }

    // Calculate and print the time taken
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time taken using qubits: %f seconds\n", time_taken);
}

// Function to brute-force a given MD5 hash using classical bits
void brute_force_md5_classical(const char *target_hash) {
    // Seed random number generator
    srand(time(NULL));

    // Create classical bits for each character in the input string
    int classical_bits[NUM_QUBITS];
    for (int i = 0; i < NUM_QUBITS; i++) {
        classical_bits[i] = rand() % 2;
    }

    // Generate binary representation of each character
    unsigned char hash[MD5_DIGEST_LENGTH];
    int found_match = 0; // Flag to indicate if a matching input is found
    clock_t start_time = clock(); // Start the timer
    do {
        char input_str[NUM_QUBITS + 1];
        input_str[NUM_QUBITS] = '\0';
        for (int i = 0; i < NUM_QUBITS; i++) {
            input_str[i] = (classical_bits[i] == 0) ? '0' : '1';
        }
        // Compute MD5 hash for the generated input string
        md5_hash(input_str, hash);
        // Compare the computed hash with the target hash
        if (memcmp(hash, target_hash, MD5_DIGEST_LENGTH) == 0) {
            printf("Matching input found using classical bits: %s\n", input_str);
            found_match = 1;
            break; // Break out of the loop once a matching input is found
        }
        // Increment classical bits for the next iteration
        for (int i = 0; i < NUM_QUBITS; i++) {
            classical_bits[i] = (classical_bits[i] + 1) % 2;
            if (classical_bits[i] == 1) {
                break;
            }
        }
    } while (!found_match);
    clock_t end_time = clock(); // Stop the timer

    // Calculate and print the time taken
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time taken using classical bits: %f seconds\n", time_taken);
}

int main() {
    // Target MD5 hash to match
    const char *target_hash = "47bce5c74f589f4867dbd57e9ca9f808"; // Example target hash for "0a1"

    // Brute-force the MD5 hash using qubits
    brute_force_md5_qubits(target_hash);

    // Brute-force the MD5 hash using classical bits
    brute_force_md5_classical(target_hash);

    return 0;
}
