#include "velocitySource.h"

VelocitySource::VelocitySource(unsigned int N, double uVel, double vVel):
    N(N), u((N + 2)* (N + 2), 0.0), v((N + 2)* (N + 2), 0.0), 
    uVel(uVel), vVel(vVel)
{}

VelocitySource::VelocitySource(unsigned int N, std::vector<double> uVec, std::vector<double> vVec):
    N(N), u(uVec), v(vVec), uVel(0), vVel(0)
{}


void VelocitySource::Tick()
{
    return;
}

const std::vector<double>& VelocitySource::GetHorizontalVelocitySource() const
{
    return u; 
}

const std::vector<double>& VelocitySource::GetVerticalVelocitySource() const
{
    return v; 
}
