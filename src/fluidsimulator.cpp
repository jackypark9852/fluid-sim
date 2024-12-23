#include "fluidsimulator.h"

FluidSimulator::FluidSimulator(unsigned int N)
{
}

const std::vector<float>& FluidSimulator::GetU() const
{
	// TODO: insert return statement here
}

const std::vector<float>& FluidSimulator::GetV() const
{
	// TODO: insert return statement here
}

const std::vector<float>& FluidSimulator::GetDens() const
{
	// TODO: insert return statement here
}

void FluidSimulator::Tick()
{
}

void FluidSimulator::AddSource(int N, std::vector<float>& x, const std::vector<float>& s, float dT)
{
}

void FluidSimulator::Diffuse(int N, BoundaryType b, std::vector<float>& x, const std::vector<float>& x0, float diff, float dt)
{
}

void FluidSimulator::Advect(int N, BoundaryType b, std::vector<float>& d, const std::vector<float>& d0, const std::vector<float>& u, const std::vector<float>& v, float dt)
{
}

void FluidSimulator::DensStep(int N, std::vector<float>& x, const std::vector<float>& x0, const std::vector<float>& u, const std::vector<float>& v, float diff, float dt)
{
}

void FluidSimulator::SetBoundaryConditions(int N, int b, std::vector<float>& x)
{
}
