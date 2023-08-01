/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Tile.hpp"
#include "Level.hpp"

#ifdef MOD_EXTRA_BLOCKS
CactusTile::CactusTile(int id, Material* c) : Tile(id, c)
{
	m_TextureFrame = TEXTURE_CACTUS_SIDE;
	//setTicking(true);
}

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

//void CactusTile::tick(Level* level, int x, int y, int z, Random* random)
//{
//}
#endif