#include "fluidsimulator.h"
#include <imgui.h>

FluidSimulator::FluidSimulator(unsigned int windowWidth, unsigned int windowHeight, unsigned int N, GLuint densityTextureHandle) :
	windowWidth(windowWidth), windowHeight(windowHeight), N(N), diffusion(10), viscosity(1), densityTextureHandle(densityTextureHandle), elemCount(N* N)
{
	int gridSize = (N + 2) * (N + 2);
	u.resize(gridSize);
	v.resize(gridSize);
	u_prev.resize(gridSize);
	v_prev.resize(gridSize);
	dens.resize(gridSize);
	dens_prev.resize(gridSize);

	for (int x = 1; x <= N; ++x) {
		for (int y = 1; y <= N; ++y) {
			if (y >= N / 2) {
				dens_prev[IX(x, y)] = 4;
			}
			else {
				dens_prev[IX(x, y)] = 1;

			}
		}
	}

}

const std::vector<double>& FluidSimulator::GetU() const
{
	return u;
}

const std::vector<double>& FluidSimulator::GetV() const
{
	return v;
}

const std::vector<double>& FluidSimulator::GetDens() const
{
	return dens;
}

void FluidSimulator::Tick()
{
	if (densityTextureHandle > 10) return;
	double dt = ImGui::GetIO().DeltaTime;
	//todo: fix hardcoded window size
	//dens_prev[IX(cursorX, cursorY)] = 4;

	//VelStep(N, u, v, u_prev, v_prev, viscosity, dt);
	DensStep(N, dens, dens_prev, u, v, diffusion, dt);
	UpdateDensityTexture();

	/*double densSum = 0;
	for (int i = 0; i < dens.size(); ++i) {
		densSum += dens[i];
	}
	densSum += 1;
	*/
}

void FluidSimulator::AddSource(int N, std::vector<double>& x, const std::vector<double>& s, double dT)
{
	int cell, size = (N + 2) * (N + 2);
	for (cell = 0; cell < size; cell++) x[cell] += dT * s[cell];
}

void FluidSimulator::Diffuse(int N, BoundaryType b, std::vector<double>& x, const std::vector<double>& x0, double diff, double dt)
{
	int i, j, k;
	double a = dt * diff * N * N;
	//todo: the 20 here is essentially our time step. It should be a function of grid meter size, not a constant.
	for (k = 0; k < 1; k++) {
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				double initial = x[IX(i, j)];
				double check = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
					x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
				x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
					x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
			}
		}
		SetBoundaryConditions(N, b, x);
	}

}

void FluidSimulator::Advect(int N, BoundaryType b, std::vector<double>& d, const std::vector<double>& d0, const std::vector<double>& u, const std::vector<double>& v, double dt)
{
	int i, j, i0, j0, i1, j1;
	double x, y, s0, t0, s1, t1, dt0;
	dt0 = dt * N;
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			x = i - dt0 * u[IX(i, j)]; y = j - dt0 * v[IX(i, j)];
			if (x < 0.5) x = 0.5; if (x > N + 0.5) x = N + 0.5; i0 = (int)x; i1 = i0 + 1;
			if (y < 0.5) y = 0.5; if (y > N + 0.5) y = N + 0.5; j0 = (int)y; j1 = j0 + 1;
			s1 = x - i0; s0 = 1 - s1; t1 = y - j0; t0 = 1 - t1;
			d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
				s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
		}
	}
	SetBoundaryConditions(N, b, d);
}

void FluidSimulator::DensStep(int N, std::vector<double>& x, std::vector<double>& x0, const std::vector<double>& u, const std::vector<double>& v, double diff, double dt)
{
	
	AddSource(N, x, x0, dt);
	//SWAP(x,x0); 
	//Diffuse(N, BoundaryType::NONE, x, x0, diff, dt);
	//SWAP(x,x0); 
	//Advect(N, BoundaryType::NONE, x, x0, u, v, dt);
}

void FluidSimulator::VelStep(int N, std::vector<double>& u, std::vector<double>& v, std::vector<double>& u0, std::vector<double>& v0,
	double visc, double dt) {
	AddSource(N, u, u0, dt); 
	AddSource(N, v, v0, dt);

	SWAP(u0, u); 
	Diffuse(N, BoundaryType::HORIZONTAL, u, u0, visc, dt);

	SWAP(v0, v); 
	Diffuse(N, BoundaryType::VERTICAL, v, v0, visc, dt);

	Project(N, u, v, u0, v0);
	SWAP(u0, u); SWAP(v0, v);
	Advect(N, BoundaryType::HORIZONTAL, u, u0, u0, v0, dt); 
	Advect(N, BoundaryType::VERTICAL, v, v0, u0, v0, dt);
	Project(N, u, v, u0, v0);

}

void FluidSimulator::Project(int N, std::vector<double>& u, std::vector<double>& v, std::vector<double>& p, std::vector<double>& div) {
	int i, j, k;
	double h;
	h = 1.0 / N;
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			div[IX(i, j)] = -0.5 * h * (u[IX(i + 1, j)] - u[IX(i - 1, j)] +
				v[IX(i, j + 1)] - v[IX(i, j - 1)]);
			p[IX(i, j)] = 0;
		}
	}
	SetBoundaryConditions(N, BoundaryType::NONE, div); SetBoundaryConditions(N, BoundaryType::NONE, p);
	for (k = 0; k < 1; k++) {
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
					p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
			}
		}
		SetBoundaryConditions(N, BoundaryType::NONE, p);
	}
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			u[IX(i, j)] -= 0.5 * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
			v[IX(i, j)] -= 0.5 * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
		}
	}
	SetBoundaryConditions(N, BoundaryType::HORIZONTAL, u); SetBoundaryConditions(N, BoundaryType::VERTICAL, v);
}


void FluidSimulator::SetBoundaryConditions(int N, BoundaryType b, std::vector<double>& x)
{
	//todo: add wrapping boundary type
	int i;
	for (i = 1; i <= N; i++) {
		x[IX(0, i)] = b == BoundaryType::HORIZONTAL ? x[IX(1, i)] * -1 : x[IX(1, i)];
		x[IX(N + 1, i)] = b == BoundaryType::HORIZONTAL ? x[IX(N, i)] * -1 : x[IX(N, i)];
		x[IX(i, 0)] = b == BoundaryType::VERTICAL ? x[IX(i, 1)] * -1 : x[IX(i, 1)];
		x[IX(i, N + 1)] = b == BoundaryType::VERTICAL ? x[IX(i, N)] * -1 : x[IX(i, N)];
	}
	x[IX(0, 0)] = 0.5 * (x[IX(1, 0)] + x[IX(0, 1)]);
	x[IX(0, N + 1)] = 0.5 * (x[IX(1, N + 1)] + x[IX(0, N)]);
	x[IX(N + 1, 0)] = 0.5 * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
	x[IX(N + 1, N + 1)] = 0.5 * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}

void FluidSimulator::UpdateDensityTexture() {
	std::vector<float> gradient(N * N * 4); // RGBA as doubles
	for(int y = 1; y <= N; ++y){
		for (int x = 1; x <= N; ++x) {
			double pixelDensity = dens[IX(x, y)] / 5.0;
			int index = ((y-1) * N + (x-1)) * 4;
			gradient[index] = pixelDensity;
			gradient[index + 1] = pixelDensity;
			gradient[index + 2] = pixelDensity;
			gradient[index + 3] = 1;
		}
	}

	//todo:: send to gpu. better ways to do this
	glBindTexture(GL_TEXTURE_2D, densityTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, N, N, 0, GL_RGBA, GL_FLOAT, gradient.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FluidSimulator::UpdateDensitySourceFromMouse(std::vector<float>& source)
{
	int cursorX = ImGui::GetMousePos().x / 1920 * N;
	int cursorY = (1 - ImGui::GetMousePos().y / 1080) * N;



}

void FluidSimulator::SetDensityTextureHandle(GLuint handle) {
	densityTextureHandle = handle;
}