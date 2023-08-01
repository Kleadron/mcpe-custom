/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Tile.hpp"
#include "Level.hpp"

#ifdef MOD_EXTRA_BLOCKS
SoulSandTile::SoulSandTile(int a, Material* c) : Tile(a, c)
{
	m_TextureFrame = TEXTURE_SOULSAND;
	setShape(0, 0, 0, 1, 15.0f / 16.0f, 1.0f);
}

AABB* SoulSandTile::getAABB(Level*, int x, int y, int z)
{
	// a full block
	m_aabbReturned = AABB(
		float(x),
		float(y),
		float(z),
		float(x + 1),
		float(y + 1),
		float(z + 1)
	);

	return &m_aabbReturned;
}

int SoulSandTile::getResource(int x, Random* random)
{
	return Tile::dirt->getResource(x, random);
}

int SoulSandTile::getTexture(int dir, int data)
{
	return TEXTURE_SOULSAND;
}

bool SoulSandTile::isCubeShaped()
{
	return false;
}

bool SoulSandTile::isSolidRender()
{
	return false;
}
#endif