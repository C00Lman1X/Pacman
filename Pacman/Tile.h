#pragma once

#include "Vector2f.h"

struct Tile
{
    int x, y;

    Tile(const Vector2f& vec) : x(vec.myX), y(vec.myY) {}

    Tile operator+(const Tile& rhs) { return Tile{x + rhs.x, y + rhs.y}; }
    Tile operator-(const Tile& rhs) { return Tile{x - rhs.x, y - rhs.y}; }

    
}