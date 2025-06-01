#include "Leg.h"

// Member functions
Leg::Leg( int index, float mountAngle, StateTransmitter* stateManager )
: index( index ),
  mountAngle( mountAngle ),
  stepSize( 0.0f ),
  stepAngle ( 0.0f ),
  stepPercent ( 0.0f ),
  stateManager( stateManager )
{   
    regenerateTrajectory();
    std::cout << "Leg " << index << " created" << std::endl;
}


Leg::~Leg() {
    std::cout << "Leg " << index << " destroyed" << std::endl;
}

float Leg::getBaseHeight() {
    return baseHeight;
}


void Leg::setStepAngle( float angle )
{
    stepAngle = angle;
    regenerateTrajectory();
    updateJointAngles();
}


void Leg::setStepSize( float size )
{   
    stepSize = size;
    regenerateTrajectory();
    updateJointAngles();
}


void Leg::setStepPercent( float stepPercent ) {
    this->stepPercent = stepPercent;
    updateJointAngles();
}


void Leg::setZNom( float newZNom) {
    zNom = newZNom;
    xNom = sqrt(pow(200.0f, 2.0f) - pow(zNom, 2.0f));
    updateJointAngles();
}


float Leg::getFlipFactor()
{   
    int isLeft = index == 0 || index == 3 || index == 4;
    int isRight = index == 1 || index == 2 || index == 5;
    return static_cast<float>(isRight - isLeft);
}


void Leg::regenerateTrajectory() {
    float x, y, z;
    float angle = mountAngle - stepAngle;
    float flip = getFlipFactor();
    trajectory.clear();

    // Half Slide 1
    for (int i = 0; i <= numSamples/4; ++i) {
        float t = static_cast<float>(i) / (numSamples/2); // t in (-0.5, 0)
        x = xNom - flip*t*stepSize*std::sin(angle);
        y = yNom - flip*t*stepSize*std::cos(angle);
        z = zNom;
        trajectory.emplace_back(x, y, z);
    }

    // Swing
    for (int i = 0; i <= numSamples/2; ++i) {
        float t = static_cast<float>(i) / (numSamples/2); // t in (-0.5, 0.5)
        x = xNom + flip*(t-0.5)*stepSize*std::sin(angle);
        y = yNom + flip*(t-0.5)*stepSize*std::cos(angle);
        z = zNom + 4 * (stepSize/2) * t * (1 - t); // Parabola
        trajectory.emplace_back(x, y, z);
    }

    // Half Slide 2
    for (int i = 0; i <= numSamples/4; ++i) {
        float t = static_cast<float>(i) / (numSamples/2); // t in (0, 0.5)
        x = xNom + flip*(0.5-t)*stepSize*std::sin(angle);
        y = yNom + flip*(0.5-t)*stepSize*std::cos(angle);
        z = zNom;
        trajectory.emplace_back(x, y, z);
    }
}


void Leg::saveTrajectory() {
    std::ofstream outFile("step_xyz.csv");
    outFile << "X, Y, Z\n";

    float x,y,z;
    for (int i=0; i < trajectory.size(); i++) {
        std::tie(x,y,z) = trajectory[i];
        outFile << x << "," << y << "," << z << std::endl;
    }

    outFile.close();
}


void Leg::updateJointAngles() {

    int i = static_cast<int>(stepPercent * numSamples / 100.0f);
    float x,y,z;
    std::vector<float> angles;

    std::tie(x,y,z) = trajectory[i];
    angles = solveInverseKinematics(x,y,z);
    stateManager->updateAngles( index, angles );
}


std::vector<float> Leg::solveInverseKinematics(float x, float y, float z) {

    // joint 1, axis rotates about z
    float theta1 = std::atan2(y, x);

    // convert input coordinates to range
    float x1 = std::sqrt((x*x)+(y*y)) - l1;
    float z1 = z;

    // distance from joint 2
    float r = std::sqrt((x1*x1)+(z1*z1)); 

    // check reachability
    if ((r > (l2+l3)) || (r < std::abs(l2-l3))) {
        throw std::domain_error("Bad coordinates");
    }

    // Law of Cosines for elbow angle (theta3)
    float cosTheta3 = (r * r - l2 * l2 - l3 * l3) / (2 * l2 * l3);
    float theta3 = std::acos(cosTheta3);

    // Elbow-down configuration
    theta3 = -theta3 + ELBOW_OFFSET_RAD;

    // Law of Cosines for shoulder angle (theta2)
    float k1 = l2 + l3 * std::cos(theta3 - ELBOW_OFFSET_RAD);
    float k2 = l3 * std::sin(theta3 - ELBOW_OFFSET_RAD);
    float theta2 = std::atan2(z1, x1) - std::atan2(k2, k1) + SHOULDER_OFFSET_RAD;

    return {radToDeg(-theta3), radToDeg(theta2), radToDeg(theta1)};
}
