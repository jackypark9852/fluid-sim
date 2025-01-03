#include "densitySource.h"

DensitySource::DensitySource(unsigned int N) :
    N(N), source((N + 2)* (N + 2), 0.0)
{}

void DensitySource::Tick()
{
    return; 
}

const std::vector<double>& DensitySource::GetSource() const
{
    return source;
}
