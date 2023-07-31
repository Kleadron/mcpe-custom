/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Tile.hpp"
#include "Level.hpp"

SpawnerTile::SpawnerTile(int a, int b, Material* c) : HalfTransparentTile(a, b, c)
{
}

int SpawnerTile::getResource(int, Random* random)
{
	return 84; //@NOTE: Would be clay's item ID
}

int SpawnerTile::getResourceCount(Random* random)
{
	return 4;
}
