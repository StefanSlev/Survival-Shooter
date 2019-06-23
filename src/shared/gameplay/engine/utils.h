#ifndef SHARED_GAMEPLAY_ENGINE_UTILS
#define SHARED_GAMEPLAY_ENGINE_UTILS

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/vector2d.h"

// -------------------------------------------------------------------------------------
// This file contains some useful functions
// Note: add hear other useful functions that don't belong to any objects in particular
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

void InitRand();
float Rand01();
float Rand(float min, float max);
float Lerp(float a, float b, float time);
int Lerp(int a, int b, float time);
Vector2D Lerp(Vector2D a, Vector2D b, float time);
long LerpColor(long a, long b, float time);
float Dist(Vector2D a, Vector2D b);
float DistSq(Vector2D a, Vector2D b);

long long GetTime();

struct sColor
{
	sColor(unsigned char a, unsigned char r, unsigned char g, unsigned char b) { A = a; R = r; G = g; B = b; }
	sColor(long color) { A = color >> 24; R = (color >> 16) & 0xFF; G = (color >> 8) & 0xFF; B = color & 0xFF; }

	long GetValue() { return (A << 24) | (R << 16) | (G << 8) | B; }
	long Mask (long mask)
	{
		sColor m(mask);
		m.A = (unsigned char)((m.A / 255.f * A / 255.f) * 255);
		m.R = (unsigned char)((m.R / 255.f * R / 255.f) * 255);
		m.G = (unsigned char)((m.G / 255.f * G / 255.f) * 255);
		m.B = (unsigned char)((m.B / 255.f * B / 255.f) * 255);
		return m.GetValue();
	}
	long MaskAlpha(unsigned char mask)
	{
		sColor m(mask, R, G, B);
		m.A = (unsigned char)((m.A / 255.f * A / 255.f) * 255);
		return m.GetValue();
	}

	unsigned char A;
	unsigned char R;
	unsigned char G;
	unsigned char B;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_UTILS