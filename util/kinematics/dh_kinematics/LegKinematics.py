from DHTable import DHTable
from Kinematics import Kinematics
from Visualiser import Visualiser
import random
import sys
from constants import standardDhTable
from scipy.spatial.transform import Rotation as R
from util import isCommandLineArgument, getAnglesFromFile


if __name__ == "__main__":
        
    try:        
        if len(sys.argv) < 2:
            raise SystemError("Requires an argument. & (random|default)")

        isStandardForm = isCommandLineArgument('standard')
        dHTable = standardDhTable

        if isCommandLineArgument('random'):
            angles = [random.randint(-180,180) for i in range(3)]
        elif not isCommandLineArgument('default'):
            angles = getAnglesFromFile(f"test{sys.argv[2]}.txt")
        else:
            angles = getAnglesFromFile("default.txt")

        print("Angles\n", angles)
        for i in range(len(angles)):
            dHTable[i][0] = angles[i]

        dHTable = DHTable(dHTable, isStandardForm)
        leg = Kinematics(dHTable)
        poses = leg.getAllPoses()

        # Print final end effector pose
        position, rotationMatrix = leg.getEndEffectorPose()
        eulerAngles = R.from_matrix(rotationMatrix).as_rotvec()
        print("\nEnd Effector Position\n", position)
        print("\nEnd Effector Rotation\n", eulerAngles, "\n")
        
        Visualiation = Visualiser(poses)

    except ValueError as e:
        print(e)
