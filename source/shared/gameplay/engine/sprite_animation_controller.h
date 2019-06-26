#ifndef SHARED_GAMEPLAY_ENGINE_SPRITE_ANIMATION_CONTROLLER
#define SHARED_GAMEPLAY_ENGINE_SPRITE_ANIMATION_CONTROLLER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/controller.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/sprite.h"
#include "shared/gameplay/engine/utils.h"
#include "shared/gameplay/engine/camera.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

template <class AnimationType> struct SpriteAnimationInfo
{
	AnimationType animationType;
	Sprite* animationSprite;
	Vector2D frameSize;
	int startIndex;
	int animationLength;
	float fps;
	bool looping;

	// callback functions for when the animation starts
	// or end
	std::function<void(Object&)> OnStart;
	std::function<void(Object&)> OnEnd;

	SpriteAnimationInfo()
	{
		animationSprite = nullptr;
		frameSize = 0;
		startIndex = -1;
		animationLength = 0;
		fps = 0.f;
		looping = false;

		OnStart = nullptr;
		OnEnd = nullptr;
	}
};

template<class AnimationType> class SpriteAnimationController : public Controller
{
public:
	SpriteAnimationController();
	~SpriteAnimationController();

	ObjectType GetType() const override { return ObjectType::SpriteAnimationController; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::SpriteAnimationController) || Controller::IsTypeOf(type); }

	void ClearAnimation();

	// the main loop of the animation
	void Update(float dt) override;
	void Render() override;

	void Load(const SpriteAnimationInfo<AnimationType>* animationInfo);
	void Play(AnimationType animationType);

	bool IsPlaying(AnimationType animationType);
	void UpdatePlaySpeed(float updatePlaySpeed);
	void Pause();
	void Continue();

private:
	const SpriteAnimationInfo<AnimationType> * currentAnimation;

	int currentIndex;
	int prevIndex;
	bool isAnimating;
	float waitTime;
	float playSpeed;

	std::unordered_map<AnimationType, const SpriteAnimationInfo<AnimationType>*> animations;

	bool CheckValidAnimation(const SpriteAnimationInfo<AnimationType>* animationInfo);

	void SelectFrame(int index);
	void UpdateFrameIndex();
};

template <class AnimationType> SpriteAnimationController<AnimationType>::SpriteAnimationController() : Controller()
{
	this->ClearAnimation();
}

template <class AnimationType> SpriteAnimationController<AnimationType>::~SpriteAnimationController()
{
	auto delete_animations = [](auto elem) { delete elem.second; };
	std::for_each(animations.begin(), animations.end(), delete_animations);

	animations.clear();
}

template <class AnimationType> void SpriteAnimationController<AnimationType>::ClearAnimation()
{
	currentAnimation = nullptr;
	currentIndex = -1;
	prevIndex = -1;
	isAnimating = false;
	waitTime = 0.f;
	playSpeed = 1.f;
}

template <class AnimationType> bool SpriteAnimationController<AnimationType>::IsPlaying(AnimationType animationType)
{
	return isAnimating && currentAnimation->animationType == animationType;
}

template <class AnimationType> void SpriteAnimationController<AnimationType>::UpdatePlaySpeed(float updatePlaySpeed)
{
	if (isAnimating)
		this->playSpeed = updatePlaySpeed;
}

template <class AnimationType> void SpriteAnimationController<AnimationType>::Pause()
{
	this->isAnimating = false;
}

template <class AnimationType> void SpriteAnimationController<AnimationType>::Continue()
{
	if (currentAnimation != nullptr)
		this->isAnimating = true;
}

template <class AnimationType> void SpriteAnimationController<AnimationType>::SelectFrame(int index)
{
	int lastIndex = currentAnimation->startIndex + currentAnimation->animationLength - 1;

	if (index < currentAnimation->startIndex || index > lastIndex)
		return;

	// update the next frame
	Vector2D frameSize = currentAnimation->frameSize;
	Vector2D spriteSize = currentAnimation->animationSprite->size;

	Vector2D framesXY = spriteSize / frameSize;
	int framesX = static_cast<int> (framesXY.x);

	int line = index / framesX;
	int column = index - line * framesX;

	object->spriteTopLeftUV = Vector2D(column * frameSize.x / spriteSize.x, line * frameSize.y / spriteSize.y);
	object->spriteBottomRightUV = object->spriteTopLeftUV + frameSize / spriteSize;
}

template <class AnimationType> void SpriteAnimationController<AnimationType>::UpdateFrameIndex()
{
	const float eps = 1e-6f;

	// update animation info
	int lastIndex = currentAnimation->startIndex + currentAnimation->animationLength - 1;

	bool endFirst = (playSpeed <= -eps && currentIndex == currentAnimation->startIndex);
	bool endLast = (playSpeed > -eps && currentIndex == lastIndex);

	if (endFirst || endLast)
	{
		if (currentAnimation->looping)
			currentIndex = endFirst ? lastIndex : currentAnimation->startIndex;
		else
		{
			if (currentAnimation->OnEnd)
				currentAnimation->OnEnd(*object);

			this->ClearAnimation();
		}
	}
	else
	{
		currentIndex += playSpeed > -eps ? 1 : -1;
		waitTime = 1.f / currentAnimation->fps;
	}
}

template <class AnimationType> void SpriteAnimationController<AnimationType>::Update(float dt)
{
	const float eps = 1e-6f;

	// update
	if (isAnimating)
	{
		if (fabsf(waitTime) < eps)
		{
			prevIndex = currentIndex;

			this->UpdateFrameIndex();

			if (!isAnimating)
				return;

			this->SelectFrame(currentIndex);
		}
		else
			waitTime = fmaxf(waitTime - dt * fabsf(playSpeed), 0.f);
	}
}

template <class AnimationType> void SpriteAnimationController<AnimationType>::Render()
{
	if (!object->visible || !isAnimating)
		return;

	this->SelectFrame(prevIndex);

	if (object->pSprite)
	{
		Vector2D spritePos;
		Vector2D spriteSize;
		float spriteRotation;
		Camera2D::g_pActiveCamera->WorldSpaceToScreenSpace(object, spritePos, spriteSize, spriteRotation);

		long hiddenMask = sColor(60, 255, 255, 255).GetValue();

		// draw the Sprite
		object->pSprite->Draw(spritePos, spriteSize, spriteRotation, sColor(object->color).Mask(hiddenMask), object->spriteTopLeftUV, object->spriteBottomRightUV);
	}

	this->SelectFrame(currentIndex);
}

template <class AnimationType> bool SpriteAnimationController<AnimationType>::CheckValidAnimation(const SpriteAnimationInfo<AnimationType>* animationInfo)
{
	bool validAnimation = true;

	validAnimation &= (animationInfo->startIndex >= 0);
	validAnimation &= (animationInfo->animationLength > 0);
	validAnimation &= (animationInfo->animationSprite != nullptr);
	validAnimation &= (animationInfo->fps > 0);
	validAnimation &= (animationInfo->frameSize.x > 0 && animationInfo->frameSize.y > 0);

	return validAnimation;
}

template <class AnimationType> void SpriteAnimationController<AnimationType>::Load(const SpriteAnimationInfo<AnimationType>* animationInfo)
{
	if (animationInfo != nullptr && CheckValidAnimation(animationInfo))
	{
		auto it = animations.find(animationInfo->animationType);

		if (it != animations.end())
			delete it->second;

		animations[animationInfo->animationType] = animationInfo;
	}
}

template <class AnimationType> void SpriteAnimationController<AnimationType>::Play(AnimationType animationType)
{
	// play
	if (currentAnimation != nullptr)
		this->ClearAnimation();

	auto it = animations.find(animationType);

	// if animation found
	if (it != animations.end())
	{
		currentAnimation = it->second;

		// load animation on player
		object->pSprite = currentAnimation->animationSprite;
		SelectFrame(currentAnimation->startIndex);

		if (currentAnimation->OnStart)
			currentAnimation->OnStart(*object);

		// set animation stats
		currentIndex = currentAnimation->startIndex;
		prevIndex = currentAnimation->startIndex;
		isAnimating = true;
		waitTime = 0.f;
		playSpeed = 1.f;
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_SPRITE_ANIMATION_CONTROLLER