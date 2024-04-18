import numpy as np
import cirq
import tensorflow as tf
from tensorflow.keras.datasets import mnist

# Load MNIST dataset and preprocess
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()
train_images, test_images = train_images / 255.0, test_images / 255.0

# Define Quantum Circuit
def quantum_circuit(num_qubits, inputs):
    qc_list = []
    qubits = cirq.LineQubit.range(num_qubits)

    # Define a helper function to apply X gate if pixel value is above threshold
    def apply_x_if_greater_than_half(qc, qubit, pixel_value):
        if pixel_value > 0.5:
            qc.append(cirq.X(qubit))

    # Iterate over batch dimension to construct quantum circuits
    for img in tf.unstack(inputs):
        qc = cirq.Circuit()
        qc.append(cirq.H.on_each(*qubits))
        for qubit, pixel_value in zip(qubits, tf.unstack(img)):
            apply_x_if_greater_than_half(qc, qubit, pixel_value)
        # Apply ZZ gate between neighboring qubits
        for q1, q2 in zip(qubits, qubits[1:]):
            qc.append(cirq.ZZ(q1, q2) ** 0.5)
        qc_list.append(qc)

    return qc_list

# Define Classical Neural Network
def classical_nn(num_qubits):
    model = cirq.Circuit()
    qubits = cirq.LineQubit.range(num_qubits)
    for i in range(num_qubits - 1):
        model.append(cirq.ZZ(qubits[i], qubits[i + 1]) ** 0.5)
    return model

# Combine Quantum Circuit and Classical Neural Network
class QNN(tf.keras.Model):
    def __init__(self, num_qubits):
        super(QNN, self).__init__()
        self.num_qubits = num_qubits
        self.classical_nn = classical_nn(self.num_qubits)

    def call(self, inputs):
        quantum_data = quantum_circuit(self.num_qubits, inputs)
        return self.classical_nn(quantum_data)

# Train the QNN
num_qubits = 4  # Number of qubits in the quantum circuit
qnn = QNN(num_qubits)
optimizer = tf.keras.optimizers.Adam()  # Use Adam optimizer from TensorFlow
qnn.compile(optimizer=optimizer, loss='sparse_categorical_crossentropy', metrics=['accuracy'])
qnn.fit(train_images, train_labels, batch_size=32, epochs=5)

# Evaluate the QNN
test_loss, test_acc = qnn.evaluate(test_images, test_labels)
print('Test accuracy:', test_acc)
