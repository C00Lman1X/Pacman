#include "Vector2f.h"
#include "PathmapTile.h"

Vector2f::Vector2f(const TileCoord& tile)
    : myX(tile.x)
    , myY(tile.y)
{
    
}