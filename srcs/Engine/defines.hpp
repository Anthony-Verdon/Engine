#pragma once

//#define FULL_SCREEN
// window
#ifdef FULL_SCREEN
constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;
#else
constexpr int WINDOW_WIDTH = 1080;
constexpr int WINDOW_HEIGHT = 720;
#endif

// box2d
constexpr float PIXEL_PER_METER = 50;

// sprite
constexpr float TILE_SIZE = 16;
constexpr float SCALE_FACTOR = 4;
constexpr float SPRITE_SIZE = TILE_SIZE * SCALE_FACTOR;