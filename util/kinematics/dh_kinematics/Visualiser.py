import numpy as np
import matplotlib.pyplot as plt
from constants import axisScaler, plotBounds

class Visualiser:
    """
    Generates a 3D rendering of a robotic arm by plotting spheres at the joints and lines connecting consecutive joints.
    """

    # Class constructor
    def __init__(self, poseList):
        self.__initialisePlot()

        for i in range(len(poseList)):
            self.__plotPose(*poseList[i])
            if i > 0:
                self.__plotLine(poseList[i][0], poseList[i-1][0])
        
        self.__ax.view_init(elev=10, azim=20)
        plt.show()

    # Add a coordinate frame to the plot
    def __plotPose(self, translation, rotation):
        # Translated coordinate frame origin
        origin = translation

        # Rotate coordinate frame axes
        axes_global = np.dot(rotation, axisScaler*(np.identity(3).T)).T
        
        # Plot frame axes (x, y, z)
        self.__ax.quiver(origin[0], origin[1], origin[2],
                         axes_global[0, 0], axes_global[0, 1], axes_global[0, 2],
                         color='r')
        self.__ax.quiver(origin[0], origin[1], origin[2],
                         axes_global[1, 0], axes_global[1, 1], axes_global[1, 2],
                         color='g')
        self.__ax.quiver(origin[0], origin[1], origin[2],
                         axes_global[2, 0], axes_global[2, 1], axes_global[2, 2],
                         color='b')

    # Plot a line between two points
    def __plotLine(self, point1, point2):
        # Ensure point1 and point2 are 1D arrays (extracting coordinates as flat lists)
        point1 = np.array(point1).flatten()
        point2 = np.array(point2).flatten()
        
        # Plot a line between two points using the correct 1D coordinates
        self.__ax.plot([point1[0], point2[0]], 
                       [point1[1], point2[1]], 
                       [point1[2], point2[2]], 
                       color='black',
                       lw=5,
                       alpha=0.5)
    
    # Initialises plot
    def __initialisePlot(self):
        self.__ax = plt.figure().add_subplot(111, projection='3d')
 
        self.__ax.set_xlim(-plotBounds, plotBounds)
        self.__ax.set_ylim(-plotBounds, plotBounds)
        self.__ax.set_zlim(0, 2*plotBounds)
        
        self.__ax.set_xlabel('X (m)')
        self.__ax.set_ylabel('Y (m)')
        self.__ax.set_zlabel('Z (m)')

        # Add invisible dummy plot for legend error fix
        self.__ax.plot([], [], [], 'r', label='X-Axis')
        self.__ax.plot([], [], [], 'g', label='Y-Axis')
        self.__ax.plot([], [], [], 'b', label='Z-Axis')

        self.__ax.legend(loc="upper left", bbox_to_anchor=(0.92, 0.55))