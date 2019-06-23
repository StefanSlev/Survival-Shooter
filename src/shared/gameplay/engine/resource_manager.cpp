#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/sprite.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

Sprite* ResourceManager::GetSprite(std::string spriteName)
{
	Sprite* imageSprite = GetSprite(ResourceType::IMAGE, spriteName);

	if (imageSprite != nullptr)
		return imageSprite;

	return GetSprite(ResourceType::ANIMATION, spriteName);
}

Sprite* ResourceManager::GetSprite(ResourceType type, std::string spriteName)
{
	// get the sprite asociated with the given spriteName
	// ResourceManager creates the sprite if it's not found initially

	this->LoadSprite(type, spriteName);
	auto& current_sprites = (type == ResourceType::IMAGE) ? images_sprites : anims_sprites;

	return current_sprites[spriteName];
}

void ResourceManager::LoadSprite(ResourceType type, std::string spriteName)
{
	auto& current_sprites = (type == ResourceType::IMAGE) ? images_sprites : anims_sprites;

	if (current_sprites.find(spriteName) == current_sprites.end())
		current_sprites[spriteName] = new Sprite(spriteName.c_str());
}

ResourceManager::~ResourceManager() {

	auto delete_sprite = [](auto elem) { delete elem.second; };

	std::for_each(images_sprites.begin(), images_sprites.end(), delete_sprite);
	std::for_each(anims_sprites.begin(), anims_sprites.end(), delete_sprite);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE