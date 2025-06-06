#pragma once
#include <iostream>
#include <array>
#include <fstream>
#include "math.h"
#include "StateTransmitter.h"

class Leg {
    public:
        // Constructor
        Leg( int index, float mountAngle, StateTransmitter* stateManager );

        // Destructor
        ~Leg();

        // Methods
        float getBaseHeight();

        void setStepAngle( float angle );

        void setStepSize( float size );

        void setStepPercent( float stepPercent );

        void setZNom( float newZNom );

        std::vector<float> solveInverseKinematics(float x, float y, float z);

        void saveTrajectory();

    private:
        // Variables
        const float numSamples = 100; // Hz
        const float baseHeight = -70.0f; // mm
        const float maxHeight = -50.0f;
        float xNom = 180.0f; // mm
        float yNom = 0.0f; // mm
        float zNom = -70.0f; // mm
        
        // DH parameters
        const float l1 = 43.0f;  // mm L1: Coxa
        const float l2 = 80.0f;  // mm L2: Femur
        const float l3 = 134.0f; // mm L3: Tibia
        const float SHOULDER_OFFSET_RAD = degToRad(-45.0f); // theta2
        const float ELBOW_OFFSET_RAD = degToRad(90.0f);    // theta3

        int index;
        float mountAngle;
        float stepAngle;
        float stepSize;
        float stepPercent;
        std::vector<std::tuple<float, float, float>> trajectory;
        StateTransmitter* stateManager;

        // Methods
        void regenerateTrajectory();

        void updateJointAngles();

        float radToDeg(float radians) { return radians * (180.0 / M_PI); };

        float degToRad(float degrees) { return degrees * (M_PI / 180.0); };

        float getFlipFactor();
};
