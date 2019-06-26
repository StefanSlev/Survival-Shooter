#include "shared/gameplay/engine/sprite.h"
#include "shared/gameplay/engine/engine.h"
#include "shared/gameplay/engine/constants.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

Sprite::Sprite(const char* fileName)
{
	D3DXIMAGE_INFO SrcInfo;      //Optional

	// Load Sprite from file
	if (FAILED(D3DXCreateTextureFromFileExA(Engine::g_pd3dDevice, fileName, 0, 0, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0 /*colorkey*/, &SrcInfo, NULL, &d3dTexture)))
	{
		d3dTexture = NULL;
		return;
	}
	size.x = (float)SrcInfo.Width;
	size.y = (float)SrcInfo.Height;

	name = fileName;
}

Sprite::~Sprite()
{
	if (d3dTexture)
	{
		d3dTexture->Release();
	}
}

void Sprite::Draw(const Vector2D & pos, float sizeP, float rotation, unsigned long color, const Vector2D & topLeftUV, const Vector2D & bottomRightUV)
{
	Draw(pos, Vector2D(sizeP), rotation, color, topLeftUV, bottomRightUV);
}

void Sprite::Draw(const Vector2D & pos, Vector2D sizeP, float rotation, unsigned long color, const Vector2D & topLeftUV, const Vector2D & bottomRightUV)
{
	if (!d3dTexture)
		return;

	sizeP *= 0.5f; // half size

	Vector2D vertices[4] = { Vector2D(-sizeP.x, -sizeP.y), Vector2D(sizeP.x, -sizeP.y), Vector2D(sizeP.x, sizeP.y), Vector2D(-sizeP.x, sizeP.y) };

	double fSinA = sin(rotation * M_PI / 180);
	double fCosA = cos(rotation * M_PI / 180);

	for (unsigned int i = 0; i < 4; i++)
	{
		if (rotation != 0)
			vertices[i].Set((float)(fCosA * vertices[i].x - fSinA * vertices[i].y), (float)(fSinA * vertices[i].x + fCosA * vertices[i].y));
		vertices[i] += pos;
	}

	unsigned long colors[4] = { color, color, color, color };

	Draw(vertices, colors, topLeftUV, bottomRightUV);
}

void Sprite::Draw(const Vector2D vertices[4], unsigned long colors[4], const Vector2D & topLeftUV, const Vector2D & bottomRightUV)
{
	Engine::TLVERTEX* tlvertices;
	//Lock the vertex buffer
	Engine::g_pVertexBuffer->Lock(0, 0, (void**)&tlvertices, NULL);

	float mappingU[4] = { topLeftUV.x, bottomRightUV.x, bottomRightUV.x, topLeftUV.x };
	float mappingV[4] = { topLeftUV.y, topLeftUV.y, bottomRightUV.y, bottomRightUV.y };

	for (int i = 0; i < 4; i++)
	{
		//Setup vertices
		//A -0.5f modifier is applied to vertex coordinates to match texture
		//and screen coords. Some drivers may compensate for this
		//automatically, but on others texture alignment errors are introduced
		//More information on this can be found in the Direct3D 9 documentation

		tlvertices[i].colour = colors[i];
		tlvertices[i].x = vertices[i].x - 0.5f;
		tlvertices[i].y = vertices[i].y - 0.5f;
		tlvertices[i].z = 0.0f;
		tlvertices[i].rhw = 1.0f;
		tlvertices[i].u = mappingU[i];
		tlvertices[i].v = mappingV[i];
	}

	//Unlock the vertex buffer
	Engine::g_pVertexBuffer->Unlock();

	//Set texture
	Engine::g_pd3dDevice->SetTexture(0, d3dTexture);

	//Draw Sprite
	Engine::g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE