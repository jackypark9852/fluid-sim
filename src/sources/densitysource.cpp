#include "sources\densitySource.h"

DensitySource::DensitySource(unsigned int N, SourceType type) :
    N(N), source(type == SourceType::THREED ? pow(N+2,3) : pow(N + 2, 2), 0.0)
{}

void DensitySource::Tick()
{
    return; 
}

const std::vector<double>& DensitySource::GetSource() const
{
    return source;
}
