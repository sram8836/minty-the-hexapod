import numpy as np

class DHTable:
    """
    DHTable: Stores and creates a DH-notation parameter table
             and computes the homogenous transform matrices.
             Degree unit is radians.
    
    Attributes:
        __htMatrices: list of homogenous transform matrices
    """
    
    # Class constructor
    def __init__(self, *args):

        self.__htMatrices = []
        self.__isStandardDh = True

        if len(args) == 2 and args[1] == False:
            self.__isStandardDh = False

        if len(args) == 0:
            # Case 1: Initialise empty DH table
            pass
        elif isinstance(args[0], list) and all(not isinstance(row, list) for row in args[0]):
            # Case 2: Initialise DH table with one entry
            self.addJoint(*args[0])
        elif isinstance(args[0], list) and all(isinstance(row, list) for row in args[0]):
            # Case 3: Initialise DH table with multiple entries
            for row in args[0]:
                self.addJoint(*row)
        else:
            raise TypeError("Invalid initialisation argument. Must be: None, list, or 2D list.")

    # Calculates homogenous transform matrices
    # Equation from lecture 2, slide 34
    def __getHtMatrixFromStandardDh(self, theta, d, a, alpha):
        alpha, theta = np.radians(alpha), np.radians(theta)
        c_alpha = np.cos(alpha)
        s_alpha = np.sin(alpha)
        c_theta = np.cos(theta)
        s_theta = np.sin(theta)

        return np.array([
            [c_theta, -s_theta*c_alpha, s_theta*s_alpha, a*c_theta],
            [s_theta, c_theta*c_alpha, -c_theta*s_alpha, a*s_theta],
            [0, s_alpha, c_alpha, d],
            [0, 0, 0, 1],
        ])
    
    def __getHtMatrixFromModifiedDh(self, theta, d, a, alpha):
        alpha, theta = np.radians(alpha), np.radians(theta)
        c_alpha = np.cos(alpha)
        s_alpha = np.sin(alpha)
        c_theta = np.cos(theta)
        s_theta = np.sin(theta)

        return np.array([
            [c_theta, -s_theta, 0, a],
            [s_theta * c_alpha, c_theta * c_alpha, -s_alpha, -d*s_alpha],
            [s_theta * s_alpha, c_theta * s_alpha, c_alpha, d*c_alpha],
            [0, 0, 0, 1]
        ])
    
    def addJoint(self, theta, d, a, alpha):
        # Enforce angle limits
        if theta < -180 or theta > 180:
            raise ValueError(f"Invalid joint angle {theta}°. Must be between -180° and 180°.")
    
        if self.__isStandardDh:
            htMatrix = self.__getHtMatrixFromStandardDh(theta, d, a, alpha)
        else:
            htMatrix = self.__getHtMatrixFromModifiedDh(theta, d, a, alpha)

        self.__htMatrices.append(htMatrix)
    
    # Returns homogenous transform matrices
    def getHtMatrices(self):
        return self.__htMatrices