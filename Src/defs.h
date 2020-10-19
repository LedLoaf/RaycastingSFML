#pragma once
#include <stdint.h>
#include <math.h>

constexpr float PI = 3.14159265f;
constexpr float TWO_PI = 6.28318530f;

constexpr int TILE_SIZE = 64;

constexpr float MINIMAP_SCALE_FACTOR = 0.2f;

constexpr int MAP_NUM_ROWS = 13;
constexpr int MAP_NUM_COLS = 20;
//#define WINDOW_WIDTH 1280
//#define WINDOW_HEIGHT 768
constexpr unsigned int WINDOW_WIDTH = (MAP_NUM_COLS * TILE_SIZE);
constexpr unsigned int WINDOW_HEIGHT = (MAP_NUM_ROWS * TILE_SIZE);

constexpr float FOV_ANGLE = (60.f * (PI / 180.f));

constexpr int NUM_RAYS = static_cast<int>(WINDOW_WIDTH);

float DIST_PROJ_PLANE = (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2);

constexpr int FPS = 30;
constexpr float FRAME_TIME_LENGTH = (1000 / FPS);
// texture size
constexpr int TEX_WIDTH = 64;
constexpr int TEX_HEIGHT = 64;
// number of textures
constexpr int NUM_TEXTURES = 9;			//! Only 8 exist in the textures.h folder && 9 in the folder
constexpr int NUM_RAW_TEXTURES = 8;


using color_t = uint32_t;

