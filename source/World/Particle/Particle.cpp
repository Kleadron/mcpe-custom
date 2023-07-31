/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Particle.hpp"
#include "Level.hpp"

float Particle::xOff, Particle::yOff, Particle::zOff;

Particle::Particle(Level* level, float x, float y, float z, float vx, float vy, float vz) : Entity(level)
{
	setSize(0.2f, 0.2f);
	field_84 = 0.5f * field_8C;
	
	setPos(x, y, z);
	m_vel.x = vx + 0.4f * (2.0f * Mth::random() - 1.0f);
	m_vel.y = vy + 0.4f * (2.0f * Mth::random() - 1.0f);
	m_vel.z = vz + 0.4f * (2.0f * Mth::random() - 1.0f);

	float mult = 0.4f * 0.15f * (Mth::random() + Mth::random() + 1.0f) / Mth::sqrt(m_vel.x * m_vel.x + m_vel.y * m_vel.y + m_vel.z * m_vel.z);
	m_vel.x *= mult;
	m_vel.y = m_vel.y * mult + 0.1f;
	m_vel.z *= mult;

	field_E0 = 3.0f * sharedRandom.nextFloat();
	field_E4 = 3.0f * sharedRandom.nextFloat();
	field_F0 = 2.0f * (0.5f + 0.5f * sharedRandom.nextFloat());
	field_EC = int(4.0f / (0.1f + 0.9f * sharedRandom.nextFloat()));
}

int Particle::move(float x, float y, float z)
{
	float x_1 = x, z_1 = z;

	if (m_bNoCollision)
	{
		// just move it. Don't perform any kind of collision
		m_hitbox.min.x += x;
		m_hitbox.max.x += x;
		m_hitbox.min.y += y;
		m_hitbox.max.y += y;
		m_hitbox.min.z += z;
		m_hitbox.max.z += z;

		m_pos.x = (m_hitbox.max.x + m_hitbox.min.x) / 2;
		m_pos.z = (m_hitbox.max.z + m_hitbox.min.z) / 2;
		m_pos.y = m_hitbox.min.y + field_84 - field_A4;

		return 1300;
	}

	//@TODO: untangle the control flow

	float x1, x2, x3, x4, x5, x6;
	float x7, x8, x9, x10, x11, x12, x20;
	float x_2, z_2, x_3, z_3;
	float oldX, oldZ;
	bool b1, b2, b3, b4, b5, b6;
	AABB hitold;

	oldX = m_pos.x, oldZ = m_pos.z;

	x7 = m_hitbox.max.z;
	x8 = m_hitbox.max.y;
	x9 = m_hitbox.max.x;
	x10 = m_hitbox.min.z;
	x11 = m_hitbox.min.y;
	x12 = m_hitbox.min.x;

	x1 = m_hitbox.max.z;
	x2 = m_hitbox.max.y;
	x3 = m_hitbox.max.x;
	x4 = m_hitbox.min.z;
	x5 = m_hitbox.min.y;
	x6 = m_hitbox.min.x;

	z_2 = z_1;
	b1 = x_1 < 0.0f;
	b2 = x_1 > 0.0f;
	x_2 = x_1;
	b3 = z_1 < 0.0f;
	b4 = z_1 > 0.0f;
	b5 = false;

	if (b1) x6 += x_1;
	if (b2) x3 += x_1;
	if (y < 0.0f) x5 += y;
	if (y > 0.0f) x2 += y;
	if (b3) x4 += z;
	if (b4) x1 += z;

	AABB scanAABB(x6, x5, x4, x3, x2, x1);
	auto pCubes = m_pLevel->getCubes(this, scanAABB);

	float newY = y;
	for (const auto &aabb : (*pCubes))
		newY = aabb.clipYCollide(m_hitbox, newY);

	m_hitbox.move(0, newY, 0);

	if (!field_81 && newY != y)
	{
		z_1 = 0.0f;
		x_1 = 0.0f;
		newY = 0.0f;
	}

	if (field_7C)
	{
		b6 = true;
	}
	else
	{
		b6 = y < 0.0f;
		if (newY == y)
			b6 = 0;
	}

	for (const auto& aabb : (*pCubes))
		x_1 = aabb.clipXCollide(m_hitbox, x_1);

	m_hitbox.move(x_1, 0, 0);

	if (!field_81 && x_1 != x_2)
	{
		z_1 = 0.0f;
		x_1 = 0.0f;
		newY = 0.0f;
	}

	for (const auto& aabb : (*pCubes))
		z_1 = aabb.clipZCollide(m_hitbox, z_1);

	m_hitbox.move(0, 0, z_1);

	if (!field_81 && z_1 != z_2)
	{
		z_1 = 0.0f;
		x_1 = 0.0f;
		newY = 0.0f;
	}

	x20 = field_A8;
	if (x20 <= 0.0f || !b6)
	{
	label_44:
		z_3 = z_1;
		x_3 = x_1;
		goto label_45;
	}

	if (field_A4 >= 0.05f || x_2 == x_1 && z_2 == z_1)
		goto label_44;

	// oh come on, undoing all our work??
	hitold = m_hitbox;
	m_hitbox = AABB(x12, x11, x10, x9, x8, x7);

	if (b1) x12 += x_2;
	if (b2) x9 += x_2;
	x8 += x20; //@BUG: missing if (x20 > 0) check?
	if (x20 < 0.0f) x11 += x20;
	if (b3) x10 += z_2;
	if (b4) x7 += z_2;

	{
		AABB scanAABB2(x12, x11, x10, x9, x8, x7);

		//@BUG: useless copy
		//@BUG: this doesn't actually copy anything
		*pCubes = *m_pLevel->getCubes(this, scanAABB2);
	}

	for (const auto& aabb : (*pCubes))
		x20 = aabb.clipYCollide(m_hitbox, x20);

	m_hitbox.move(0, x20, 0);

	if (field_81 || x20 == y)
	{
		z_3 = z_2;
		x_3 = x_2;
	}
	else
	{
		x20 = 0.0f;
		z_3 = 0.0f;
		x_3 = 0.0f;
	}

	for (const auto& aabb : (*pCubes))
		x_3 = aabb.clipXCollide(m_hitbox, x_3);

	m_hitbox.move(x_3, 0, 0);

	if (!field_81 && x_2 != x_3)
	{
		x20 = 0.0f;
		z_3 = 0.0f;
		x_3 = 0.0f;
	}

	for (const auto& aabb : (*pCubes))
		z_3 = aabb.clipZCollide(m_hitbox, z_3);

	m_hitbox.move(0, 0, z_3);

	if (!field_81 && z_2 != z_3)
	{
		x20 = 0.0f;
		z_3 = 0.0f;
		x_3 = 0.0f;
	}

	// if we moved more this time than before?? no clue wtf this is about...
	if (z_1 * z_1 + x_1 * x_1 < z_3 * z_3 + x_3 * x_3)
	{
		field_A4 += 0.5f;
		newY = x20;
	}
	else
	{
		// don't get the rationale behind this at all...
		m_hitbox = hitold;
		z_3 = z_1;
		x_3 = x_1;
	}

label_45:
	m_pos.x = (m_hitbox.min.x + m_hitbox.max.x) / 2;
	m_pos.z = (m_hitbox.min.z + m_hitbox.max.z) / 2;
	m_pos.y = m_hitbox.min.y - field_A4 + field_84;

	field_7D = x_2 != x_3 || z_2 != z_3;
	field_7F = field_7D || newY != y;
	field_7C = y < 0.0f && newY != y;
	field_7E = newY != y;

	if (x_2 != x_3) m_vel.x = 0.0;
	if (newY != y)  m_vel.y = 0.0;
	if (z_2 != z_3) m_vel.z = 0.0;

	// Check whether the entity is inside of any tiles.

	int minX = Mth::floor(m_hitbox.min.x), minY = Mth::floor(m_hitbox.min.y), minZ = Mth::floor(m_hitbox.min.z),
		maxX = Mth::floor(m_hitbox.max.x), maxY = Mth::floor(m_hitbox.max.y), maxZ = Mth::floor(m_hitbox.max.z);

	if (m_pLevel->hasChunksAt(minX, minY, minZ, maxX, maxY, maxZ))
	{
		for (int x = minX; x <= maxX; x++)
			for (int y = minY; y <= maxY; y++)
				for (int z = minZ; z <= maxZ; z++)
				{
					TileID tileID = m_pLevel->getTile(x, y, z);
					if (tileID > 0)
						Tile::tiles[tileID]->entityInside(m_pLevel, x, y, z, this);
				}
	}

	field_A4 *= 0.4f;

	return 1300;
}

int Particle::getParticleTexture()
{
	return PT_PARTICLES;
}

Particle* Particle::scale(float f)
{
	setSize(0.2f * f, 0.2f * f);
	field_F0 *= f;
	return this;
}

Particle* Particle::setPower(float f)
{
	m_vel.x *= f;
	m_vel.y = (m_vel.y - 0.1f) * f + 0.1f;
	m_vel.z *= f;
	return this;
}

void Particle::render(Tesselator& t, float f, float a4, float a5, float a6, float a7, float a8)
{
	constexpr float C_MAGIC_1 = 0.062438f; // @BUG: Slightly bigger than 1/16.0f

	int texture = field_DC;
	int texX = texture % 16;
	if (texture < 0)
		texture += 15;

	float texU_1 = float(texX) / 16.0f;
	float texV_1 = float(texture >> 4) / 16.0f;

	float posX = Lerp(field_3C.x, m_pos.x, f) - xOff;
	float posY = Lerp(field_3C.y, m_pos.y, f) - yOff;
	float posZ = Lerp(field_3C.z, m_pos.z, f) - zOff;
	float fBright = getBrightness(f);

	float sizeX = a4 * field_F0 * 0.1f;
	float sizeY = a5 * field_F0 * 0.1f;
	float sizeZ = a6 * field_F0 * 0.1f;
	float siz2X = a7 * field_F0 * 0.1f;
	float siz2Z = a8 * field_F0 * 0.1f;

	t.color(field_F8 * fBright, field_FC * fBright, field_100 * fBright);
	t.vertexUV(posX - sizeX - siz2X, posY - sizeY, posZ - sizeZ - siz2Z, texU_1 + C_MAGIC_1, texV_1 + C_MAGIC_1);
	t.vertexUV(posX - sizeX + siz2X, posY + sizeY, posZ - sizeZ + siz2Z, texU_1 + C_MAGIC_1, texV_1);
	t.vertexUV(posX + sizeX + siz2X, posY + sizeY, posZ + sizeZ + siz2Z, texU_1,             texV_1);
	t.vertexUV(posX + sizeX - siz2X, posY - sizeY, posZ + sizeZ - siz2Z, texU_1,             texV_1 + C_MAGIC_1);
}

void Particle::tick()
{
	field_3C = m_pos;
	field_E8++;
	if (field_E8 >= field_EC)
		remove();
	
	m_vel.y -= field_F4 * 0.04f;
	move(m_vel.x, m_vel.y, m_vel.z);

	m_vel.x *= 0.98f;
	m_vel.y *= 0.98f;
	m_vel.z *= 0.98f;

	if (field_7C)
	{
		m_vel.x *= 0.7f;
		m_vel.z *= 0.7f;
	}
}
