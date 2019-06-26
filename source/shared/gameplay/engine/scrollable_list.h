#ifndef SHARED_GAMEPLAY_ENGINE_SCROLLABLE_LIST
#define SHARED_GAMEPLAY_ENGINE_SCROLLABLE_LIST

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/menu_item.h"
#include "shared/gameplay/engine/vector2d.h"

// -------------------------------------------------------------------------------------
// This is a scrollable list menu item
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Sprite;

class ScrollableList : public MenuItem
{
public:
	ScrollableList(Vector2D size = 0.f, int maxNrItems = defaultMaxNrItems);

	ObjectType GetType() const override { return ObjectType::ScrollableList; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::ScrollableList) || MenuItem::IsTypeOf(type); }

	void SetItemSprite(Sprite* setItemSprite) { this->itemSprite = setItemSprite; }
	void SetItemTextFormat(DWORD itemTextFormat);
	void SetItemFont(Font* itemFont);
	void SetScrolling(bool canScroll);

	void AddItem(const std::string& item);
	int GetNumberItems() const;
	void SelectItem(unsigned int index, bool select);
	void FocusItem(unsigned int index);

	// resets the list to its initial form
	void Reset();
	void Refresh();
	void Clear();

	void Update(float dt) override;
private:
	int maxNrItems;
	static const int defaultMaxNrItems = 10;

	// item background
	Sprite* itemSprite;

	// the visible items
	std::deque<MenuItem*> lines;

	// all the items in the list
	std::vector<std::string> items;
	std::vector<bool> selected;

	// scrollable list's properties
	Vector2D itemSize;
	DWORD itemTextFormat;
	Font* itemFont;

	float firstLinePosY;
	float lastLinePosY;
	float topHiddenLineY;
	float bottomHiddenLineY;

	// sliding-window endings in items vector
	int firstItem;
	int lastItem;

	// properties for scrolling
	float moveSpeed;
	Vector2D velocity;
	float dragCoef;
	float dampCoef;
	bool canScroll;

	// properties for fade in/out
	long visibleMask;
	long hiddenMask;

	void UpdateItems(float dt);
	void HandleScrolling();
	void ApplyEffects(MenuItem* item, unsigned int itemIndex);
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_SCROLLABLE_LIST