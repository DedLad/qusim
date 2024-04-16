#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

// Function to simulate quantum teleportation
void quantumTeleportation(Qubit** sender_qubits, Qubit** receiver_qubits, int num_bits) {
    // Create entangled qubit pairs (Alice's and Bob's qubits)
    Qubit** alice_qubits = (Qubit**)malloc(num_bits * sizeof(Qubit*));
    Qubit** bob_qubits = (Qubit**)malloc(num_bits * sizeof(Qubit*));
    for (int i = 0; i < num_bits; i++) {
        alice_qubits[i] = initializeQubit(1/sqrt(2), 0); // |+>
        bob_qubits[i] = initializeQubit(0, 1/sqrt(2));   // |1>
    }

    // Apply Hadamard gate to Alice's qubits
    for (int i = 0; i < num_bits; i++) {
        applyHadamardGate(alice_qubits[i]);
    }

    // Apply CNOT gate with Alice's qubits as control and Bob's qubits as target
    for (int i = 0; i < num_bits; i++) {
        applyPauliXGate(alice_qubits[i]); // Equivalent to a CNOT gate when the control qubit is |1>
        applyPauliXGate(bob_qubits[i]);   // Equivalent to a CNOT gate when the control qubit is |1>
    }

    // Apply controlled-Z gate with Alice's qubits as control and sender's qubits as target
    for (int i = 0; i < num_bits; i++) {
        if (measureQubit(sender_qubits[i]) == 1) {
            applyPauliXGate(bob_qubits[i]); // Apply Pauli-X (bit flip) if sender's qubit is |1>
        }
    }

    // Apply controlled-Z gate with Alice's qubits as control and Bob's qubits as target
    for (int i = 0; i < num_bits; i++) {
        if (measureQubit(alice_qubits[i]) == 1) {
            applyPauliXGate(receiver_qubits[i]); // Apply Pauli-X (bit flip) if Alice's qubit is |1>
        }
    }

    // Measure Alice's qubits and teleport the state to Bob's qubits
    for (int i = 0; i < num_bits; i++) {
        int measured_bit_alice = measureQubit(alice_qubits[i]);
        if (measured_bit_alice == 1) {
            applyPauliXGate(receiver_qubits[i]); // Apply Pauli-X (bit flip) to Bob's qubit if Alice's qubit was |1>
        }
        if (measureQubit(sender_qubits[i]) == 1) {
            applyPauliXGate(receiver_qubits[i]); // Apply Pauli-X (bit flip) to Bob's qubit if Sender's qubit was |1>
        }
    }

    // Free memory allocated for Alice's and Bob's qubits
    for (int i = 0; i < num_bits; i++) {
        freeQubit(alice_qubits[i]);
        freeQubit(bob_qubits[i]);
    }
    free(alice_qubits);
    free(bob_qubits);
}

// Function to convert a binary string to an array of qubit states
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

// Function to print the state of a qubit
void printQubit(Qubit* qubit) {
    printf("|0>: %.2f\n", qubit->alpha);
    printf("|1>: %.2f\n", qubit->beta);
}

// Function to decode the binary string from an array of qubit states
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

int main() {
    // Prompt the user to input a 4-bit binary string
    char binary_string[5];
    printf("Enter a 4-bit binary string: ");
    scanf("%4s", binary_string);
    int num_bits = strlen(binary_string);

    // Convert the binary string to an array of qubit states
    Qubit** sender_qubits = binaryStringToQubits(binary_string, num_bits);
    
    // Initialize an array of receiver's qubits in the |0> state
    Qubit** receiver_qubits = (Qubit**)malloc(num_bits * sizeof(Qubit*));
    for (int i = 0; i < num_bits; i++) {
        receiver_qubits[i] = initializeQubit(1, 0); // |0>
    }
    
    // Print initial states of sender's and receiver's qubits
    printf("Initial states of sender's qubits:\n");
    for (int i = 0; i < num_bits; i++) {
        printf("Qubit %d:\n", i);
        printQubit(sender_qubits[i]);
        printf("\n");
    }
    printf("\nInitial states of receiver's qubits:\n");
    for (int i = 0; i < num_bits; i++) {
        printf("Qubit %d:\n", i);
        printQubit(receiver_qubits[i]);
        printf("\n");
    }

    // Simulate quantum teleportation
    quantumTeleportation(sender_qubits, receiver_qubits, num_bits);

    // Print final states of sender's and receiver's qubits
    printf("\nFinal states of sender's qubits (after teleportation):\n");
    for (int i = 0; i < num_bits; i++) {
        printf("Qubit %d:\n", i);
        printQubit(sender_qubits[i]);
        printf("\n");
    }
    printf("\nFinal states of receiver's qubits (after teleportation):\n");
    for (int i = 0; i < num_bits; i++) {
        printf("Qubit %d:\n", i);
        printQubit(receiver_qubits[i]);
        printf("\n");
    }

    // Decode the data from the receiver's qubits and print it
    char* decoded_data = qubitsToBinaryString(receiver_qubits, num_bits);
    printf("\nDecoded data from receiver's qubits: %s\n", decoded_data);

    // Free memory allocated for qubits
    for (int i = 0; i < num_bits; i++) {
        freeQubit(sender_qubits[i]);
        freeQubit(receiver_qubits[i]);
    }
    free(sender_qubits);
    free(receiver_qubits);
    free(decoded_data);

    return 0;
}
