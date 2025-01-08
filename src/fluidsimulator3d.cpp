#include <imgui.h>
#include <glm/glm.hpp>
#include <iostream>

#include "fluidsimulator3d.h"
#include "sources/circularSource.h"
#include "sources/rectvelocitysource.h"

FluidSimulator3d::FluidSimulator3d(unsigned int N, GLuint densityTextureHandle, GLuint velocityTextureHandle) :
	N(N), diffusion(0.0001), viscosity(0), densityTextureHandle(densityTextureHandle), elemCount(N* N), densSources(),
	velocityTextureHandle(velocityTextureHandle), scenes(), activeScene(nullptr)
{
	int gridSize = (N + 2) * (N + 2) * (N + 2);
	u.resize(gridSize);
	v.resize(gridSize);
	w.resize(gridSize);
	u_prev.resize(gridSize);
	v_prev.resize(gridSize);
	w_prev.resize(gridSize);
	dens.resize(gridSize);
	dens_prev.resize(gridSize);

	InitializeScenes();
}

const std::vector<double>& FluidSimulator3d::GetU() const
{
	return u;
}

const std::vector<double>& FluidSimulator3d::GetV() const
{
	return v;
}

const std::vector<double>& FluidSimulator3d::GetDens() const
{
	return dens;
}

void FluidSimulator3d::Tick()
{
	if (densityTextureHandle > 10) return;
	double dt = 0.016;// ImGui::GetIO().DeltaTime;
	//todo: fix hardcoded window size
	int cursorX = ImGui::GetMousePos().x / 1200 * N;
	int cursorY = (1 - ImGui::GetMousePos().y / 1200) * N;
	//dens_prev[IX3D(cursorX, cursorY)] = 4;

	//VelStep(N, u, v, w, u_prev, v_prev, w_prev, viscosity, dt);
	DensStep(N, dens, dens_prev, u, v, w, diffusion, dt);
	UpdateDensityTexture();

	if (ImGui::IsMouseDragging(0) && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
		ImVec2 dragVec = ImGui::GetMouseDragDelta(0);
		// AddVel(cursorX, cursorY, dragVec.x, dragVec.y);
		if (cursorX >= 0 && cursorX <= N && cursorY >= 0 && cursorY <= N) {
			AddDens(cursorX, cursorY, abs((dragVec.x + dragVec.y) * 100));
		}
	}
	else if (ImGui::IsMouseDragging(1) && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
		ImVec2 dragVec = ImGui::GetMouseDragDelta(1);
		if (cursorX >= 0 && cursorX <= N && cursorY >= 0 && cursorY <= N) {
			AddVel(cursorX, cursorY, dragVec.x * 0.001, dragVec.y * 0.001);
		}
	}

	for (int x = 1; x <= N; ++x) {
		for (int y = 1; y <= N; y++) {
			//dens[IX3D(x, y)] *= 0.99;
		}
	}

	double densSum = 0;
	for (int i = 0; i < dens.size(); ++i) {
		densSum += dens[i];
	}
	std::cout << densSum << std::endl;
}

GLuint FluidSimulator3d::GetDensityTextureHandle() const
{
	return densityTextureHandle;
}

void FluidSimulator3d::AddSource(int N, std::vector<double>& x, const std::vector<double>& s, double dT)
{
	int cell, size = (N + 2) * (N + 2);
	for (cell = 0; cell < size; cell++) x[cell] += dT * s[cell];
}

void FluidSimulator3d::ApplyDensitySources(double dT)
{
	for (const DensitySource& densSource : densSources) {
		AddSource(N, dens, densSource.GetSource(), dT);
	}
}

void FluidSimulator3d::ApplyVelocitySources(double dT)
{
	for (const VelocitySource& velSource : velSources) {
		AddSource(N, u, velSource.GetHorizontalVelocitySource(), dT);
		AddSource(N, v, velSource.GetVerticalVelocitySource(), dT);
	}
}

void FluidSimulator3d::AddDens(int x, int y, float amt) {
	dens[IX3D(x, y, 1)] += glm::clamp(amt + (float)dens[IX3D(x, y, 1)], 0.f, 1.f);
}

void FluidSimulator3d::AddVel(int x, int y, float amtX, float amtY) {
	u[IX3D(x, y, 1)] += amtX;
	v[IX3D(x, y, 1)] += amtY;
}

void FluidSimulator3d::Diffuse(int N, BoundaryType b, std::vector<double>& x, const std::vector<double>& x0, double diff, double dt)
{
	int i, j, z, k;
	double a = dt * diff * N * N;
	//todo: the 20 here is essentially our time step. It should be a function of grid meter size, not a constant.
	for (k = 0; k < 20; k++) {
		for (i = 1; i <= N; ++i) {
			for (j = 1; j <= N; ++j) {
				for (z = 1; z <= N; ++z)
					x[IX3D(i, j, z)] = (x0[IX3D(i, j, z)] + a * (x[IX3D(i - 1, j, z)] + x[IX3D(i + 1, j, z)] +
						x[IX3D(i, j - 1, z)] + x[IX3D(i, j + 1, z)] + x[IX3D(i, j, z - 1)] + +x[IX3D(i, j, z + 1)])) / (1 + 6 * a);
			}
		}
		SetBoundaryConditions(N, b, x);
	}
}

void FluidSimulator3d::Advect(int N, BoundaryType b, std::vector<double>& d, const std::vector<double>& d0, const std::vector<double>& u, const std::vector<double>& v, const std::vector<double>& w, double dt)
{
	int i, j, k, i0, j0, k0, i1, j1, k1;
	double x, y, z, s0, t0, c0, s1, t1, c1, dt0;
	dt0 = dt * N;
	for (i = 1; i <= N; ++i) {
		for (j = 1; j <= N; ++j) {
			for(k = 1; k <= N; ++k)
			x = i - dt0 * u[IX3D(i, j, k)];
			y = j - dt0 * v[IX3D(i, j, k)];
			z = k - dt0 * v[IX3D(i, j, k)];

			if (x < 0.5) x = 0.5; if (x > N + 0.5) x = N + 0.5; i0 = (int)x; i1 = i0 + 1;
			if (y < 0.5) y = 0.5; if (y > N + 0.5) y = N + 0.5; j0 = (int)y; j1 = j0 + 1;
			if (z < 0.5) z = 0.5; if (z > N + 0.5) z = N + 0.5; k0 = (int)z; k1 = k0 + 1;


			s1 = x - i0; s0 = 1 - s1;
			t1 = y - j0; t0 = 1 - t1;
			c1 = z - k0; c0 = 1 - c1;


			d[IX3D(i, j, k)] =
				c0 * (s0 * (t0 * d0[IX3D(i0, j0, k0)] + t1 * d0[IX3D(i0, j1, k0)]) + s1 * (t0 * d0[IX3D(i1, j0, k0)] + t1 * d0[IX3D(i1, j1, k0)])) +
				c1 * (s0 * (t0 * d0[IX3D(i0, j0, k1)] + t1 * d0[IX3D(i0, j1, k1)]) + s1 * (t0 * d0[IX3D(i1, j0, k1)] + t1 * d0[IX3D(i1, j1, k1)]));
		}
	}
	SetBoundaryConditions(N, b, d);
}

void FluidSimulator3d::DensStep(int N, std::vector<double>& x, std::vector<double>& x0, const std::vector<double>& u, const std::vector<double>& v, const std::vector<double>& w, double diff, double dt)
{
	// AddSource(N, x, x0, dt);
	ApplyDensitySources(dt);
	SWAP(x, x0);
	Diffuse(N, BoundaryType::NONE, x, x0, diff, dt);
	SWAP(x, x0);
	Advect(N, BoundaryType::NONE, x, x0, u, v, w, dt);
}

void FluidSimulator3d::VelStep(int N, std::vector<double>& u, std::vector<double>& v, std::vector<double>& w, std::vector<double>& u0, std::vector<double>& v0, std::vector<double>& w0,
	double visc, double dt) {
	ApplyVelocitySources(dt);

	SWAP(u0, u);
	Diffuse(N, BoundaryType::HORIZONTAL, u, u0, visc, dt);

	SWAP(v0, v);
	Diffuse(N, BoundaryType::VERTICAL, v, v0, visc, dt);

	Project(N, u, v, w, u0, v0);
	SWAP(u0, u); SWAP(v0, v);

	//todo: this right?
	Advect(N, BoundaryType::HORIZONTAL, u, u0, u0, v0, w0, dt);
	Advect(N, BoundaryType::VERTICAL, v, v0, u0, v0, w0, dt);
	Advect(N, BoundaryType::FORWARD, w, w0, u0, v0, w0, dt);

	Project(N, u, v, w, u0, v0);
}

void FluidSimulator3d::Project(int N, std::vector<double>& u, std::vector<double>& v, std::vector<double>& w, std::vector<double>& p, std::vector<double>& div) {
	int i, j, k, timeStep;
	double h;
	h = 1.0 / N;
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			for (k = 1; k <= N; ++k) {
				div[IX3D(i, j, k)] = -0.33 * h * (u[IX3D(i + 1, j, k)] - u[IX3D(i - 1, j, k)] +
					v[IX3D(i, j + 1, k)] - v[IX3D(i, j - 1, k)] + w[IX3D(i, j, k + 1) - w[IX3D(i, j, k - 1)]]);
				p[IX3D(i, j, k)] = 0;
			}
		}
	}
	SetBoundaryConditions(N, BoundaryType::NONE, div); SetBoundaryConditions(N, BoundaryType::NONE, p);
	for (timeStep = 0; timeStep < 20; ++timeStep) {
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				for (k = 1; k <= N; ++k) {
					p[IX3D(i, j, k)] = (div[IX3D(i, j, k)] + p[IX3D(i - 1, j, k)] + p[IX3D(i + 1, j, k)] +
						p[IX3D(i, j - 1, k)] + p[IX3D(i, j + 1, k)] + p[IX3D(i, j, k - 1)] + p[IX3D(i, j, k + 1)]) / 6;
				}
			}
		}
		SetBoundaryConditions(N, BoundaryType::NONE, p);
	}
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			for (k = 1; k <= N; ++k) {
				u[IX3D(i, j, k)] -= 0.5 * (p[IX3D(i + 1, j, k)] - p[IX3D(i - 1, j, k)]) / h;
				v[IX3D(i, j, k)] -= 0.5 * (p[IX3D(i, j + 1, k)] - p[IX3D(i, j - 1, k)]) / h;
				w[IX3D(i, j, k)] -= 0.5 * (p[IX3D(i, j, k + 1)] - p[IX3D(i, j, k - 1)]) / h;

			}
		}
	}
	SetBoundaryConditions(N, BoundaryType::HORIZONTAL, u); SetBoundaryConditions(N, BoundaryType::VERTICAL, v);
}


void FluidSimulator3d::SetBoundaryConditions(int N, BoundaryType b, std::vector<double>& x)
{
	//todo: add wrapping boundary type
	int i, j;
	for (i = 1; i <= N; ++i) {
		for (j = 1; j <= N; ++j) {
			x[IX3D(0, i, j)] = b == BoundaryType::HORIZONTAL ? x[IX3D(1, i, j)] * -1 : x[IX3D(1, i, j)];
			x[IX3D(N + 1, i, j)] = b == BoundaryType::HORIZONTAL ? x[IX3D(N, i, j)] * -1 : x[IX3D(N, i, j)];

			x[IX3D(i, 0, j)] = b == BoundaryType::VERTICAL ? x[IX3D(i, 1, j)] * -1 : x[IX3D(i, 1, j)];
			x[IX3D(i, N + 1, j)] = b == BoundaryType::VERTICAL ? x[IX3D(i, N, j)] * -1 : x[IX3D(i, N, j)];

			x[IX3D(i, j, 0)] = b == BoundaryType::FORWARD ? x[IX3D(i, j, 1)] * -1 : x[IX3D(i, j, 1)];
			x[IX3D(i, j, N + 1)] = b == BoundaryType::FORWARD ? x[IX3D(i, j, N)] * -1 : x[IX3D(i, j, N)];
		}
	}
	x[IX3D(0, 0, 0)] = 0.33 * (x[IX3D(1, 0, 0)] + x[IX3D(0, 1, 0)] + x[IX3D(0, 0, 1)]);
	x[IX3D(0, N + 1, 0)] = 0.33 * (x[IX3D(1, N + 1, 0)] + x[IX3D(0, N, 0)] + x[IX3D(0, N + 1, 1)]);
	x[IX3D(N + 1, 0, 0)] = 0.33 * (x[IX3D(N, 0, 0)] + x[IX3D(N + 1, 1, 0)] + x[IX3D(N + 1, 0, 1)]);
	x[IX3D(0, 0, N + 1)] = 0.33 * (x[IX3D(0, 0, N)] + x[IX3D(0, 1, N + 1)] + x[IX3D(1, 0, N + 1)]);
	x[IX3D(N + 1, N + 1, N + 1)] = 0.33 * (x[IX3D(N, N + 1, N)] + x[IX3D(N + 1, N, N)] + x[IX3D(N, N, N + 1)]);
}

void FluidSimulator3d::UpdateDensityTexture() {
	std::vector<float> gradient(N * N * N * 4); // RGBA as doubles

	for (int y = 1; y <= N; ++y) {
		for (int x = 1; x <= N; ++x) {
			for (int z = 1; z <= N; ++z) {
				double pixelDensity = dens[IX3D(x, y,z)] / 2.5;
				int index = ((z - 1) * N * N + (y - 1) * N + (x - 1)) * 4;
				gradient[index] = pixelDensity;
				gradient[index + 1] = pixelDensity;
				gradient[index + 2] = pixelDensity;
				gradient[index + 3] = 1;
			}
		}
	}

	//todo:: send to gpu. better ways to do this
	glBindTexture(GL_TEXTURE_2D, densityTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, N, N, 0, GL_RGBA, GL_FLOAT, gradient.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}


void FluidSimulator3d::InitializeScenes()
{
	// Create two empty scenes and add them to the scene selector to test scene selection functionarlity
}


void FluidSimulator3d::SetDensityTextureHandle(GLuint handle) {
	densityTextureHandle = handle;
}

void FluidSimulator3d::UpdateVelocityTexture() {
	std::vector<float> field(N * N * N * 4); // RGBA as doubles

	for (int y = 1; y <= N; ++y) {
		for (int x = 1; x <= N; ++x) {
			for (int z = 1; z <= N; ++z) {
				glm::vec3 pixelVelocity = glm::vec3(u[IX3D(x, y, z)], v[IX3D(x, y, z)], 0);
				int index = ((z-1) * N * N + (y - 1) * N + (x - 1)) * 4;
				// mapping the vector vals [-1, 1] to [0, 1]
				field[index] = (glm::normalize(pixelVelocity).x + 1.f) * 0.5;
				field[index + 1] = (glm::normalize(pixelVelocity).y + 1.f) * 0.5;
				field[index + 2] = (glm::normalize(pixelVelocity).z + 1.f) * 0.5;
				// storing original length, dividing by 10 to ensure it is between 0 and 1, might need to change later
				field[index + 3] = glm::length(pixelVelocity) / 10.f;
			}
		}
	}

	//todo:: send to gpu. better ways to do this
	glBindTexture(GL_TEXTURE_2D, velocityTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, N, N, 0, GL_RGBA, GL_FLOAT, field.data());
	glBindTexture(GL_TEXTURE_2D, 1);
}

void FluidSimulator3d::SetVelocityTextureHandle(GLuint handle) {
	velocityTextureHandle = handle;
}

std::vector<std::string> FluidSimulator3d::GetSceneNames() const
{
	// Create a vector and populate it with scene names by iterating through the map 
	std::vector<std::string> sceneNames;
	for (auto kvp : scenes) { // kvp: (const char* name, Scene scene) 
		sceneNames.push_back(kvp.first);
	}
	return sceneNames;
}

void FluidSimulator3d::ActivateSceneByName(const std::string& sceneName)
{
	// Do nothing if it is already active
	if (activeScene != nullptr && activeScene->GetName() == sceneName) {
		return;
	}

	// Do nothing if not matching scene exists
	if (scenes.find(sceneName) == scenes.end()) {
		return;
	}
	else {
		// Activate the newly selected scene
		activeScene = &(scenes[sceneName]);

		// Remove density sources from the previous scene
		densSources.clear();
		velSources.clear();

		// Reset the density and the velocity of the current scene
		Reset();

		// Add the density and velocity sources from the new scene
		for (const DensitySource& densSource : activeScene->GetDensSources()) {
			densSources.push_back(densSource);
		}

		for (const VelocitySource& velSource : activeScene->GetVelocitySources()) {
			velSources.push_back(velSource);
		}
	}
}

void FluidSimulator3d::Reset()
{
	int gridSize = (N + 2) * (N + 2);
	u.assign(gridSize, 0.0);
	v.assign(gridSize, 0.0);
	u_prev.assign(gridSize, 0.0);
	v_prev.assign(gridSize, 0.0);
	dens.assign(gridSize, 0.0);
	dens_prev.assign(gridSize, 0.0);
}