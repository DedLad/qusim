#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    double prob_0 = pow(qubit->alpha, 2);
    double prob_1 = pow(qubit->beta, 2);
    double rand_num = (double)rand() / RAND_MAX;

    if (rand_num < prob_0) {
        return 0; // Measured |0>
    } else {
        return 1; // Measured |1>
    }
}

// Function to print the state of a qubit
void printQubit(Qubit* qubit) {
    printf("|0>: %.2f\n", qubit->alpha);
    printf("|1>: %.2f\n", qubit->beta);
}

// Function to encode data in qubits
Qubit** encodeData(int* data, int size) {
    Qubit** qubits = (Qubit**)malloc(size * sizeof(Qubit*));
    for (int i = 0; i < size; i++) {
        if (data[i] == 0) {
            qubits[i] = initializeQubit(1.0, 0.0);
        } else {
            qubits[i] = initializeQubit(0.0, 1.0);
        }
    }
    return qubits;
}

// Function to decode qubits and retrieve data
int* decodeData(Qubit** qubits, int size) {
    int* data = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        data[i] = measureQubit(qubits[i]);
    }
    return data;
}

int main() {
    // Example usage
    int dataSize;
    printf("Enter the size of the data: ");
    scanf("%d", &dataSize);

    int* data = (int*)malloc(dataSize * sizeof(int));
    printf("Enter the data (0s and 1s): ");
    for (int i = 0; i < dataSize; i++) {
        scanf("%d", &data[i]);
    }

    Qubit** qubits = encodeData(data, dataSize);

    printf("Encoded Data:\n");
    for (int i = 0; i < dataSize; i++) {
        printf("%d ", data[i]);
    }
    printf("\n\n");

    printf("Qubit Parameters:\n");
    for (int i = 0; i < dataSize; i++) {
        printf("Qubit %d:\n", i);
        printQubit(qubits[i]);
        printf("\n");
    }

    int* decodedData = decodeData(qubits, dataSize);

    printf("Decoded Data:\n");
    for (int i = 0; i < dataSize; i++) {
        printf("%d ", decodedData[i]);
    }
    printf("\n");

    // Clean up memory
    for (int i = 0; i < dataSize; i++) {
        free(qubits[i]);
    }
    free(qubits);
    free(data);
    free(decodedData);

    return 0;
}
