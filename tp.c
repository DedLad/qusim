#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

// Function to free memory allocated for a qubit
void freeQubit(Qubit* qubit) {
    free(qubit);
}

// Function to measure a qubit
int measureQubit(Qubit* qubit) {
    double prob_0 = pow(qubit->alpha, 2);
    double prob_1 = pow(qubit->beta, 2);
    double rand_num = (double)rand() / RAND_MAX;

    if (rand_num < prob_0) {
        return 0; // Measured |0>
    } else {
        return 1; // Measured |1>
    }
}

// Function to apply the Hadamard gate to a qubit
void applyHadamardGate(Qubit* qubit) {
    double new_alpha = (qubit->alpha + qubit->beta) / sqrt(2);
    double new_beta = (qubit->alpha - qubit->beta) / sqrt(2);
    qubit->alpha = new_alpha;
    qubit->beta = new_beta;
}

// Function to apply the Pauli-X gate to a qubit (bit flip)
void applyPauliXGate(Qubit* qubit) {
    double temp = qubit->alpha;
    qubit->alpha = qubit->beta;
    qubit->beta = temp;
}

// Function to convert a string to binary
char* stringToBinary(const char* input_string) {
    int length = strlen(input_string);
    char* binary_string = (char*)malloc((length * 5 + 1) * sizeof(char)); // Each character takes 5 bits in binary representation
    binary_string[0] = '\0'; // Ensure the string is null-terminated

    for (int i = 0; i < length; i++) {
        char ch = toupper(input_string[i]);
        if (isalpha(ch)) {
            ch -= 'A'; // Convert to number (0-25)
            for (int j = 4; j >= 0; j--) {
                if (ch & (1 << j)) {
                    strcat(binary_string, "1");
                } else {
                    strcat(binary_string, "0");
                }
            }
        }
    }

    return binary_string;
}

// Function to convert binary string to an array of qubits
Qubit** binaryStringToQubits(char* binary_string, int num_bits) {
    Qubit** qubits = (Qubit**)malloc(num_bits * sizeof(Qubit*));
    for (int i = 0; i < num_bits; i++) {
        double alpha, beta;
        if (binary_string[i] == '1') {
            alpha = 0;
            beta = 1;
        } else {
            alpha = 1;
            beta = 0;
        }
        qubits[i] = initializeQubit(alpha, beta);
    }
    return qubits;
}

// Function to convert qubits to binary string
char* qubitsToBinaryString(Qubit** qubits, int num_bits) {
    char* binary_string = (char*)malloc((num_bits + 1) * sizeof(char));
    for (int i = 0; i < num_bits; i++) {
        if (qubits[i]->alpha > 0.5) {
            binary_string[i] = '0';
        } else {
            binary_string[i] = '1';
        }
    }
    binary_string[num_bits] = '\0';
    return binary_string;
}

// Function to perform quantum teleportation
void quantumTeleportation(Qubit** sender_qubits, Qubit** receiver_qubits, int num_bits) {
    // Apply CNOT gate with sender's qubits as control and receiver's qubits as target
    for (int i = 0; i < num_bits; i++) {
        if (measureQubit(sender_qubits[i]) == 1) {
            applyPauliXGate(receiver_qubits[i]); // Apply Pauli-X (bit flip) to receiver's qubit if sender's qubit is |1>
        }
    }

    // Apply Hadamard gate to sender's qubits
    for (int i = 0; i < num_bits; i++) {
        applyHadamardGate(sender_qubits[i]);
    }

    // Apply controlled-Z gate with sender's qubits as control and receiver's qubits as target
    for (int i = 0; i < num_bits; i++) {
        if (measureQubit(sender_qubits[i]) == 1) {
            applyPauliXGate(receiver_qubits[i]); // Apply Pauli-X (bit flip) to receiver's qubit if sender's qubit is |1>
        }
    }
}

// Function to print the state of a qubit
void printQubit(Qubit* qubit) {
    printf("|0>: %.2f\n", qubit->alpha);
    printf("|1>: %.2f\n", qubit->beta);
}

// Function to print the states of qubits
void printQubitStates(Qubit** qubits, int num_bits) {
    for (int i = 0; i < num_bits; i++) {
        printf("Qubit %d:\n", i);
        printQubit(qubits[i]);
        printf("\n");
    }
}

// Function to decode binary string to original string
char* binaryToString(char* binary_string) {
    int length = strlen(binary_string) / 5; // Each character takes 5 bits
    char* decoded_string = (char*)malloc((length + 1) * sizeof(char));
    decoded_string[length] = '\0';

    for (int i = 0; i < length; i++) {
        int char_value = 0;
        for (int j = 0; j < 5; j++) {
            char_value <<= 1;
            char_value |= (binary_string[i * 5 + j] == '1') ? 1 : 0;
        }
        decoded_string[i] = char_value + 'A'; // Convert back to character
    }

    return decoded_string;
}

int main() {
    // Get input word from user
    char input_word[4];
    printf("Enter a word of 3 characters (only alphabets): ");
    scanf("%s", input_word);

    // Convert input word to binary string
    char* binary_string = stringToBinary(input_word);

    // Create qubits from binary string
    int num_bits = strlen(binary_string);
    Qubit** sender_qubits = binaryStringToQubits(binary_string, num_bits);

    // Initialize receiver's qubits in the |0> state
    Qubit** receiver_qubits = (Qubit**)malloc(num_bits * sizeof(Qubit*));
    for (int i = 0; i < num_bits; i++) {
        receiver_qubits[i] = initializeQubit(1, 0); // |0>
    }

    // Print initial qubit states
    printf("Initial states of sender's qubits:\n");
    printQubitStates(sender_qubits, num_bits);
    printf("\n");

    printf("Initial states of receiver's qubits:\n");
    printQubitStates(receiver_qubits, num_bits);
    printf("\n");

    // Perform quantum teleportation
    quantumTeleportation(sender_qubits, receiver_qubits, num_bits);

    // Print final qubit states
    printf("Final states of sender's qubits (after teleportation):\n");
    printQubitStates(sender_qubits, num_bits);
    printf("\n");

    printf("Final states of receiver's qubits (after teleportation):\n");
    printQubitStates(receiver_qubits, num_bits);
    printf("\n");

    // Decode the binary string from receiver's qubits and print it
    char* decoded_string = binaryToString(qubitsToBinaryString(receiver_qubits, num_bits));
    printf("Decoded word from receiver's qubits: %s\n", decoded_string);

    // Free memory
    for (int i = 0; i < num_bits; i++) {
        freeQubit(sender_qubits[i]);
        freeQubit(receiver_qubits[i]);
    }
    free(sender_qubits);
    free(receiver_qubits);
    free(binary_string);
    free(decoded_string);

    return 0;
}
