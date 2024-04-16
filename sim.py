import hashlib
import itertools
import time
import random
import cirq

# Function to generate all possible strings of given length and characters
def generate_strings(length, characters):
    return [''.join(p) for p in itertools.product(characters, repeat=length)]

# Function to hash a string using SHA-256
def hash_string_sha256(string):
    return hashlib.sha256(string.encode()).hexdigest()

# Function to perform classical brute force using a specified number of bits
def classical_brute_force(target_hash, num_bits, characters, num_iterations):
    start_time = time.time()

    possible_strings = generate_strings(num_bits, characters)
    random.shuffle(possible_strings)

    for string in possible_strings[:num_iterations]:
        hashed_string = hash_string_sha256(string)
        if hashed_string == target_hash:
            end_time = time.time()
            return string, end_time - start_time

    return None, time.time() - start_time

# Function to simulate the quantum circuit
def simulate_quantum_circuit(num_qubits, target_hash, characters, num_shots):
    start_time = time.time()

    possible_strings = generate_strings(num_qubits, characters)
    oracle = create_oracle(possible_strings, target_hash)

    grover_circuit = grover_iterations(oracle, num_qubits)

    simulator = cirq.Simulator()
    result = simulator.run(grover_circuit, repetitions=num_shots)
    counts = result.histogram(key='result')

    end_time = time.time()
    return counts, end_time - start_time

# Function to create the oracle circuit
def create_oracle(possible_strings, target_hash):
    oracle = cirq.Circuit()
    qubits = cirq.LineQubit.range(len(possible_strings))
    for i, char in enumerate(possible_strings):
        if hash_string_sha256(char) == target_hash:
            oracle.append(cirq.I(qubits[i]))  # Do nothing
        else:
            oracle.append(cirq.X(qubits[i]))  # Flip the qubit
    return oracle

# Function to create the Grover iterations
def grover_iterations(oracle, num_qubits):
    qubits = cirq.LineQubit.range(num_qubits)
    diffuser = create_diffuser(qubits)
    num_iterations = int((3.14 / 4) * (2 ** (num_qubits / 2)) ** 0.5)  # Optimal number of iterations
    grover_circuit = oracle
    for _ in range(num_iterations):
        grover_circuit += diffuser
        grover_circuit += oracle
    return grover_circuit

# Function to create the diffuser circuit
def create_diffuser(qubits):
    diffuser = cirq.Circuit()
    diffuser.append([cirq.H(q) for q in qubits])
    diffuser.append([cirq.X(q) for q in qubits])
    diffuser.append(cirq.ZPowGate(exponent=1.0).on(qubits[-1]))
    diffuser.append(cirq.H(qubits[-1]))
    diffuser.append(cirq.MCX(qubits[:-1], qubits[-1]))
    diffuser.append(cirq.H(qubits[-1]))
    diffuser.append([cirq.X(q) for q in qubits])
    diffuser.append([cirq.H(q) for q in qubits])
    return diffuser

# Main function
if __name__ == "__main__":
    target_string = input("Enter a string (1 alphabet only, lowercase): ")
    if len(target_string) != 1 or not target_string.islower():
        print("Invalid input. Please enter exactly 1 lowercase alphabet.")
    else:
        target_hash = hash_string_sha256(target_string)
        print("Target hash:", target_hash)

        characters = 'abcdefghijklmnopqrstuvwxyz'
        num_qubits = len(characters) ** 1  # Considering 1 character, each character with 26 possibilities
        num_iterations = 2 ** 10  # Number of iterations for classical brute force
        num_shots = 1024  # Number of shots for quantum simulation

        classical_result, classical_time = classical_brute_force(target_hash, num_qubits, characters, num_iterations)
        print("Classical brute force result:", classical_result)
        print("Time taken by classical brute force:", classical_time, "seconds")

        quantum_result, quantum_time = simulate_quantum_circuit(num_qubits, target_hash, characters, num_shots)
        print("Quantum brute force result:", quantum_result)
        print("Time taken by quantum simulation:", quantum_time, "seconds")
