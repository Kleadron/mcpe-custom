/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Tile.hpp"
#include "Level.hpp"

ClothTile::ClothTile(int id) : Tile(id, TEXTURE_CLOTH_64, Material::cloth)
{
	//field_6C = type;

	//m_TextureFrame = getTexture(0, type);
}

int ClothTile::getTexture(int dir)
{
	return getTexture(dir, 0);
}

int ClothTile::getTexture(int dir, int data)
{
	switch (data)
	{
		case 0:	return 64; // white
		case 1:	return 210; // orange
		case 2:	return 194; // magenta
		case 3:	return 178; // light blue
		case 4: return 162; // yellow
		case 5: return 146; // lime
		case 6: return 130; // pink
		case 7: return 114; // gray
		case 8: return 225; // light grey
		case 9: return 209; // cyan
		case 10: return 193; // purple
		case 11: return 177; // blue
		case 12: return 161; // brown
		case 13: return 145; // green
		case 14: return 129; // red
		case 15: return 113; // black
	}

	return 64; // white (default)
}

int ClothTile::getSpawnResourcesAuxValue(int val)
{
	return val;
}
