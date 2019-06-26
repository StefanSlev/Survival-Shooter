#ifndef SHARED_GAMEPLAY_ENGINE_RESOURCE_MANAGER
#define SHARED_GAMEPLAY_ENGINE_RESOURCE_MANAGER

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// Class that loads and manages the sprites in the game
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

enum class ResourceType
{
	IMAGE,
	ANIMATION
};

class Sprite;

class ResourceManager
{
public:
	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	Sprite* GetSprite(std::string spriteName);
	Sprite* GetSprite(ResourceType type, std::string spriteName);
	void LoadSprite(ResourceType type, std::string spriteName);

	~ResourceManager();
private:
	std::unordered_map<std::string, Sprite*> images_sprites;
	std::unordered_map<std::string, Sprite*> anims_sprites;
	ResourceManager() {}
	ResourceManager(const ResourceManager&) = delete;
	void operator= (const ResourceManager&) = delete;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_RESOURCE_MANAGER