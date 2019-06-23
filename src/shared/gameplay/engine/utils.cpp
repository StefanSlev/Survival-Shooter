#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

void InitRand()
{
	time_t t;
	srand((unsigned int)time(&t));
}

float Rand01()
{
	return (float)rand() / (float)RAND_MAX;
}

float Rand(float min, float max)
{
	return min + Rand01() * (max - min);
}

float Lerp(float a, float b, float time)
{
	if (time <= 0) return a;
	if (time >= 1) return b;
	return (b - a) * time + a;
}

int Lerp(int a, int b, float time)
{
	if (time <= 0) return a;
	if (time >= 1) return b;
	return static_cast<int>((b - a) * time) + a;
}

Vector2D Lerp(Vector2D a, Vector2D b, float time)
{
	if (time <= 0) return a;
	if (time >= 1) return b;
	return a + (b - a) * time;
}

float Dist(Vector2D a, Vector2D b)
{
	return (b - a).Length();
}

float DistSq(Vector2D a, Vector2D b)
{
	return (b - a).LengthSq();
}

// returns time in milliseconds from
// high_resolution_clock epoch
long long GetTime()
{
	using namespace std::chrono;

	auto timepoint = high_resolution_clock::now();
	auto duration = timepoint.time_since_epoch();

	return duration_cast<milliseconds> (duration).count();
}

long LerpColor(long a, long b, float time)
{
	sColor colorA(a);
	sColor colorB(b);
	sColor ret(0);

	ret.A = (unsigned char) Lerp(colorA.A, colorB.A, time);
	ret.R = (unsigned char) Lerp(colorA.R, colorB.R, time);
	ret.G = (unsigned char) Lerp(colorA.G, colorB.G, time);
	ret.B = (unsigned char) Lerp(colorA.B, colorB.B, time);

	return ret.GetValue();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE