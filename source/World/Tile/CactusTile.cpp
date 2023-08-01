/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Tile.hpp"
#include "Level.hpp"

#ifdef MOD_EXTRA_BLOCKS
CactusTile::CactusTile(int id, Material* c) : HalfTransparentTile(id, TEXTURE_CACTUS_SIDE, c)
{
	m_TextureFrame = TEXTURE_CACTUS_SIDE;
	//setTicking(true);
	setShape(1.f / 16.f, 0, 1.f / 16.f, 1 - (1.f / 16.f), 1, 1 - (1.f / 16.f));
}

//bool CactusTile::isSolidRender()
//{
//	return false;
//}
//
//bool CactusTile::isCubeShaped()
//{
//	return false;
//}


int CactusTile::getResource(int i, Random* random)
{
	return Tile::dirt->getResource(i, random);
}

int CactusTile::getTexture(LevelSource* level, int x, int y, int z, int dir)
{
	if (dir == DIR_YPOS)
		return TEXTURE_CACTUS_TOP;

	if (dir == DIR_YNEG)
		return TEXTURE_CACTUS_BOTTOM;

	return TEXTURE_CACTUS_SIDE;
}

//bool CactusTile::shouldRenderFace(LevelSource* level, int x, int y, int z, int dir)
//{
//	if (dir == DIR_XPOS || dir == DIR_XNEG || dir == DIR_ZPOS || dir == DIR_ZNEG)
//		return true;
//
//	/*if (!Tile::shouldRenderFace(level, x, y, z, dir))
//		return false;*/
//
//	if (dir == DIR_YNEG)
//		return true;
//
//	return level->getTile(x, y, z) != m_ID;
//}

//void CactusTile::tick(Level* level, int x, int y, int z, Random* random)
//{
//}
#endif