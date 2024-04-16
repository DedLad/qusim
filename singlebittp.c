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
void quantumTeleportation(Qubit* sender_qubit, Qubit* receiver_qubit) {
    // Create an entangled qubit pair (Alice's and Bob's qubits)
    Qubit* alice_qubit = initializeQubit(1/sqrt(2), 0); // |+>
    Qubit* bob_qubit = initializeQubit(0, 1/sqrt(2));   // |1>

    // Apply Hadamard gate to Alice's qubit
    applyHadamardGate(alice_qubit);

    // Apply CNOT gate with Alice's qubit as control and Bob's qubit as target
    applyPauliXGate(alice_qubit); // Equivalent to a CNOT gate when the control qubit is |1>
    applyPauliXGate(bob_qubit);   // Equivalent to a CNOT gate when the control qubit is |1>

    // Apply controlled-Z gate with Alice's qubit as control and sender's qubit as target
    if (measureQubit(sender_qubit) == 1) {
        applyPauliXGate(bob_qubit); // Apply Pauli-X (bit flip) if sender's qubit is |1>
    }

    // Apply controlled-Z gate with Alice's qubit as control and Bob's qubit as target
    if (measureQubit(alice_qubit) == 1) {
        applyPauliXGate(receiver_qubit); // Apply Pauli-X (bit flip) if Alice's qubit is |1>
    }

    // Now, Alice's qubit is entangled with Bob's qubit. Measure Alice's qubit.
    int measured_bit_alice = measureQubit(alice_qubit);

    // Apply Pauli-X gate to Bob's qubit if Alice's qubit was |1>
    if (measured_bit_alice == 1) {
        applyPauliXGate(receiver_qubit);
    }

    // Apply Pauli-Z gate to Bob's qubit if Sender's qubit was |1>
    if (measureQubit(sender_qubit) == 1) {
        applyPauliXGate(receiver_qubit); // Apply Pauli-X (bit flip) if sender's qubit is |1>
    }
}

// Function to convert a binary string to a qubit state
Qubit* binaryStringToQubit(char* binary_string) {
    double alpha = 0, beta = 0;
    int length = strlen(binary_string);
    for (int i = 0; i < length; i++) {
        if (binary_string[i] == '1') {
            beta += pow(2, -i-1);
        }
    }
    alpha = sqrt(1 - pow(beta, 2));
    return initializeQubit(alpha, beta);
}

// Function to print the state of a qubit
void printQubit(Qubit* qubit) {
    printf("|0>: %.2f\n", qubit->alpha);
    printf("|1>: %.2f\n", qubit->beta);
}

// Function to decode the binary string from the qubit state
char* qubitToBinaryString(Qubit* qubit) {
    int bit;
    if (qubit->alpha > 0.5) {
        bit = 0;
    } else {
        bit = 1;
    }
    char* binary_string = (char*)malloc(2 * sizeof(char));
    binary_string[0] = bit + '0';
    binary_string[1] = '\0';
    return binary_string;
}

int main() {
    // Prompt the user to input a 4-bit binary string
    char binary_string[5];
    printf("Enter a 4-bit binary string: ");
    scanf("%4s", binary_string);

    // Convert the binary string to a qubit state
    Qubit* sender_qubit = binaryStringToQubit(binary_string);
    
    // Initialize receiver's qubit in the |0> state
    Qubit* receiver_qubit = initializeQubit(1, 0); // |0>
    
    // Print initial states of sender's and receiver's qubits
    printf("Initial state of sender's qubit:\n");
    printQubit(sender_qubit);
    printf("\nInitial state of receiver's qubit:\n");
    printQubit(receiver_qubit);

    // Simulate quantum teleportation
    quantumTeleportation(sender_qubit, receiver_qubit);

    // Print final states of sender's and receiver's qubits
    printf("\nFinal state of sender's qubit (after teleportation):\n");
    printQubit(sender_qubit);
    printf("\nFinal state of receiver's qubit (after teleportation):\n");
    printQubit(receiver_qubit);

    // Decode the data from the receiver's qubit state and print it
    char* decoded_data = qubitToBinaryString(receiver_qubit);
    printf("\nDecoded data from receiver's qubit: %s\n", decoded_data);

    // Free memory allocated for qubits
    free(sender_qubit);
    free(receiver_qubit);
    free(decoded_data);

    return 0;
}
