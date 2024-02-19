#pragma once
#include <glm/fwd.hpp>

#define TERRAIN_CELL_SIZE 8.0
#define TERRAIN_WIDTH 64
#define TERRAIN_CHUNK_COUNT (TERRAIN_WIDTH * TERRAIN_WIDTH)
typedef glm::fvec2 ChunkPosition;

#define TERRAIN_SINGLE_CHUNK_POINT_COUNT 64
static glm::vec2 GridPoints[8][8] =
{
	{ glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 2.0f), glm::vec2(0.0f, 3.0f), glm::vec2(0.0f, 4.0f), glm::vec2(0.0f, 5.0f), glm::vec2(0.0f, 6.0f), glm::vec2(0.0f, 7.0f) },
	{ glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 2.0f), glm::vec2(1.0f, 3.0f), glm::vec2(1.0f, 4.0f), glm::vec2(1.0f, 5.0f), glm::vec2(1.0f, 6.0f), glm::vec2(1.0f, 7.0f) },
	{ glm::vec2(2.0f, 0.0f), glm::vec2(2.0f, 1.0f), glm::vec2(2.0f, 2.0f), glm::vec2(2.0f, 3.0f), glm::vec2(2.0f, 4.0f), glm::vec2(2.0f, 5.0f), glm::vec2(2.0f, 6.0f), glm::vec2(2.0f, 7.0f) },
	{ glm::vec2(3.0f, 0.0f), glm::vec2(3.0f, 1.0f), glm::vec2(3.0f, 2.0f), glm::vec2(3.0f, 3.0f), glm::vec2(3.0f, 4.0f), glm::vec2(3.0f, 5.0f), glm::vec2(3.0f, 6.0f), glm::vec2(3.0f, 7.0f) },
	{ glm::vec2(4.0f, 0.0f), glm::vec2(4.0f, 1.0f), glm::vec2(4.0f, 2.0f), glm::vec2(4.0f, 3.0f), glm::vec2(4.0f, 4.0f), glm::vec2(4.0f, 5.0f), glm::vec2(4.0f, 6.0f), glm::vec2(4.0f, 7.0f) },
	{ glm::vec2(5.0f, 0.0f), glm::vec2(5.0f, 1.0f), glm::vec2(5.0f, 2.0f), glm::vec2(5.0f, 3.0f), glm::vec2(5.0f, 4.0f), glm::vec2(5.0f, 5.0f), glm::vec2(5.0f, 6.0f), glm::vec2(5.0f, 7.0f) },
	{ glm::vec2(6.0f, 0.0f), glm::vec2(6.0f, 1.0f), glm::vec2(6.0f, 2.0f), glm::vec2(6.0f, 3.0f), glm::vec2(6.0f, 4.0f), glm::vec2(6.0f, 5.0f), glm::vec2(6.0f, 6.0f), glm::vec2(6.0f, 7.0f) },
	{ glm::vec2(7.0f, 0.0f), glm::vec2(7.0f, 1.0f), glm::vec2(7.0f, 2.0f), glm::vec2(7.0f, 3.0f), glm::vec2(7.0f, 4.0f), glm::vec2(7.0f, 5.0f), glm::vec2(7.0f, 6.0f), glm::vec2(7.0f, 7.0f) }
};