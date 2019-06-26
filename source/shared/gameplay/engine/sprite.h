#ifndef SHARED_GAMEPLAY_ENGINE_SPRITE
#define SHARED_GAMEPLAY_ENGINE_SPRITE

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/vector2d.h"

// -------------------------------------------------------------------------------------
// This provides the loading, unloading and drawing of images.
// Loads one Sprite (D3D texture) and can render it multiple times
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Sprite
{
public:
	// Load the texture from the path. Supports all file types recognized by DX9 (.bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, .tga).
	Sprite(const char* fileName);
	// Unloads the Sprite
	~Sprite();

	bool IsValid() { return d3dTexture != nullptr; }
	const std::string GetName() { return name;  }

	// Draws the sprite centered at pos, with the given size (width = height), rotation and color (ARGB format)
	// The texture can be an atlas and you can specify the UV coordinates ([0, 1], top left & bottom right) where your sprite resides
	// You can call this many times per frame (presumably with different parameters)
	void Draw(const Vector2D& pos, float sizeP, float rotation = 0, unsigned long color = 0xffffffff, const Vector2D& topLeftUV = { 0, 0 }, const Vector2D& bottomRightUV = { 1, 1 });

	// Draws the Sprite centered at pos, with the given size (x is width and y is height), rotation and color (ARGB format)
	// The texture can be an atlas and you can specify the UV coordinates ([0, 1], top left & bottom right) where you Sprite resides
	// You can call this many times per frame (presumably with different parameters).
	void Draw(const Vector2D& pos, Vector2D sizeP, float rotation = 0, unsigned long color = 0xffffffff, const Vector2D& topLeftUV = { 0, 0 }, const Vector2D& bottomRightUV = { 1, 1 });

	Vector2D size;

protected:
	// Order of vertices is [TopLeft, TopRight, BottomRight, BottomLeft]
	// Color format is ARGB (the 4 colors are assigned to the vertices in the same order as above)
	// The texture can be an atlas and you can specify the UV coordinates ([0, 1], top left & bottom right) where you sprite resides
	// You can call this multiple times per frame (presumably with different parameters)
	void Draw(const Vector2D vertices[4], unsigned long colors[4], const Vector2D& topLeftUV, const Vector2D& bottomRightUV);
	
	// name of the sprite
	std::string			name;
	// The pointer to the D3D 9 texture
	IDirect3DTexture9 * d3dTexture;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_SPRITE