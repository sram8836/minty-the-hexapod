import sys
import numpy as np
from pathlib import Path
import time

def isCommandLineArgument(string):
    for arg in sys.argv:
        if arg == string:
            return True
    return False

def getAnglesFromFile(fileName):
    file_path = Path(fileName)
    
    if not file_path.exists():
        raise FileNotFoundError(f"File not found: {file_path}")
    
    with file_path.open("r", encoding="utf-8") as file:
        file = file.read()

    return [float(item.strip()) for item in file.split(",")]
    
def rotationVectorToMatrix(r):
    theta = np.linalg.norm(r)

    # Edge case: no rotation, return identity matrix
    if np.isclose(theta, 0):
        return np.eye(3)

    # Unit rotation axis
    r_hat = r / theta
    x, y, z = r_hat

    K = np.array([
        [0, -z, y],
        [z, 0, -x],
        [-y, x, 0]
    ])

    # Rodrigues' formula
    R = np.eye(3) + np.sin(theta) * K + (1 - np.cos(theta)) * np.dot(K, K)
    return R