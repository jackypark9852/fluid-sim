#include "sources\rectvelocitysource.h"

RectVelocitySource::RectVelocitySource(unsigned int N, int width, int height, int x, int y, double uVel, double vVel)
    :
    VelocitySource(N, uVel, vVel),
    width(std::min(std::max(width, 0), (int)N)),
    height(std::min(std::max(height, 0), (int)N)),
    position(
        glm::ivec2(
            std::min(std::max(x, 0), (int)N),
            std::min(std::max(y, 0), (int)N)
        )
    )
{
    for (int i = position.x; i < N && i < position.x + width; ++i) {
        for (int j = position.y; j < N && j < position.y + height; ++j) {
            u[IX(i, j)] = uVel; 
            v[IX(i, j)] = vVel; 
        }
    }
}
