#include "PathmapTile.h"
#include "Vector2f.h"

bool TileCoord::operator==(const TileCoord &other) const
{
    return x == other.x && y == other.y;
}

bool TileCoord::operator!=(const TileCoord &other) const
{
    return !(*this == other);
}

const TileCoord TileCoord::operator-(const TileCoord &other) const 
{
    return {x - other.x, y - other.y};
}

const TileCoord TileCoord::operator+(const TileCoord &other) const 
{
    return {x + other.x, y + other.y};
}

const TileCoord TileCoord::operator*(int aInt) const
{
    return TileCoord{x * aInt, y * aInt};
}

const Vector2f TileCoord::operator*(float aFloat) const
{
    return Vector2f{x * aFloat, y * aFloat};
}


PathmapTile::PathmapTile(int anX, int anY, bool aIsBlockingFlag)
    : coord{anX, anY}
    , myIsBlockingFlag(aIsBlockingFlag)
{
}

PathmapTile::~PathmapTile(void)
{
}
