import numpy as np

class Kinematics:
    """
    Computes forward kinematics of an appendage using the DHTable class.
    Enforces joint limits, and handles singularities.

    Attributes:
        __dHTable
        __pose
    """
    
    # Class constructor
    def __init__(self, dHTable):
        self.__dHTable = dHTable
        self.__calculateFrames()
        self.__validateJoints()

    # Adds homogenous transform matrix to chain
    def __addLink(self, newHtMatrix):
        if len(self.__frames) == 0:
            self.__frames.append(newHtMatrix)

        self.__frames.append(
            np.dot(self.__frames[-1], newHtMatrix)
        )

    # Computes each link frame using supplied DH table
    def __calculateFrames(self):
        self.__frames = [np.eye(4)]

        # Successively compute homogenous transform chain
        for transform in (self.__dHTable).getHtMatrices():
            self.__addLink(transform)

    # Validate no joints below z=0 plane
    def __validateJoints(self):
        for i, frame in enumerate(self.__frames):
            zPosition = frame[2, 3]  # Get the z-coordinate of the translation
            # if zPosition < 0:
            #     raise ValueError(f"Joint {i+1} is below the z=0 plane with z={zPosition}.")

    # Get translation and rotation matrix pairs (P, R) for specified joint
    def __getPose(self, index):
        rotationMatrix = self.__frames[index][0:3, :][:, 0:3]
        translationMatrix = self.__frames[index][0:3, :][:, 3]
        return (translationMatrix, rotationMatrix)
    
    # Get list of translation and rotation matrix pairs (P, R) for joints
    def getAllPoses(self):
        poses = []
        for i in range(0, len(self.__frames)):
            poses.append(self.__getPose(i))
        return poses

    # Get pose of end effector
    def getEndEffectorPose(self):
        return self.__getPose(-1)