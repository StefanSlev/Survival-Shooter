#ifndef SHARED_GAMEPLAY_ENGINE_MENU_ITEM
#define SHARED_GAMEPLAY_ENGINE_MENU_ITEM

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/font.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

#define MAX_TEXT_LEN 256

// -------------------------------------------------------------------------------------
// Class that defines the basic Menu Item.
// Beside the image / sprite inherited from the Object, it also has a text.
// It adds the functionality of Rendering (the image & the text) in the Virtual Screen Space.
// It can be derrived to extend its functionality - see the EditBox class
// -------------------------------------------------------------------------------------

class MenuItem : public Object
{
public:
	MenuItem();

	virtual void Render();

	void SetText(const char* str);

	virtual ObjectType GetType() const { return ObjectType::MenuItem; }
	virtual bool IsTypeOf(ObjectType type) const { return (type == ObjectType::MenuItem) || Object::IsTypeOf(type); }

	unsigned long	textColor;
	DWORD			textFormat;
	Font*			pFont;

	char			text[MAX_TEXT_LEN];
	char *			pText;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_MENU_ITEM