/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Timer.hpp"
#include "Utils.hpp"

float getAccurateTimeMs()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER currentCounter;
	QueryPerformanceCounter(&currentCounter);

	float ms = float(((double)currentCounter.QuadPart / (double)frequency.QuadPart) * 1000);
	return ms;
}

void Timer::advanceTime()
{
	float oneSecond = 1000;
	float timeMs = getAccurateTimeMs();

	if (timeMs - field_4 <= oneSecond)
	{
		if (timeMs - field_4 < 0)
		{
			field_4 = field_8 = timeMs;
		}
	}
	else
	{
		float diff1 = timeMs - field_4;
		float diff2 = timeMs - field_8;
		field_C += ((diff1 / diff2) - field_C) * 0.2f;
	}

	float diff = timeMs / oneSecond - field_0;
	field_0 = timeMs / oneSecond;

	float x1 = diff * field_C;
	if (x1 > 1) x1 = 1;
	if (x1 < 0) x1 = 0;

	float x2 = field_20 + x1 * field_1C * field_10;
	field_14 = int(x2);
	field_20 = x2 - field_14;
	field_18 = x2 - field_14;
	if (field_14 > 10)
		field_14 = 10;
}

Timer::Timer()
{
	field_4 = field_8 = getAccurateTimeMs();
}
