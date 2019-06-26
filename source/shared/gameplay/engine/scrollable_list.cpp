#include "shared/gameplay/engine/scrollable_list.h"
#include "shared/gameplay/engine/sprite.h"
#include "shared/gameplay/engine/utils.h"
#include "shared/gameplay/engine/input.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

ScrollableList::ScrollableList(Vector2D size, int maxNrItems) : MenuItem()
{
	this->maxNrItems = maxNrItems;
	this->size = size;
	
	// scrollable list's properties
	itemSize = Vector2D(size.x, size.y / (2 * maxNrItems + 3));
	itemTextFormat = DT_CENTER | DT_VCENTER;
	itemFont = Font::g_DefaultSmallFont;
	firstLinePosY = size.y * 0.5f - 2.5f * itemSize.y;
	lastLinePosY = -size.y * 0.5f + 2.5f * itemSize.y;
	topHiddenLineY = (size.y - itemSize.y) * 0.5f;
	bottomHiddenLineY = (-size.y + itemSize.y) * 0.5f;

	// sliding-window endings in items vector
	firstItem = -1;
	lastItem = -1;

	// properties for scrolling
	dragCoef = 1.f;
	dampCoef = 0.05f;
	moveSpeed = 500.f;
	velocity = 0.f;
	canScroll = true;

	// properties for fade in/out
	visibleMask = sColor(255, 255, 255, 255).GetValue();
	hiddenMask = sColor(0, 255, 255, 255).GetValue();
}

void ScrollableList::SetScrolling(bool setCanScroll)
{
	this->canScroll = setCanScroll;
}

void ScrollableList::SetItemTextFormat(DWORD setItemTextFormat)
{
	this->itemTextFormat = setItemTextFormat;

	for (MenuItem* line : lines)
		line->textFormat = setItemTextFormat;
}

void ScrollableList::SetItemFont(Font* setItemFont)
{
	this->itemFont = setItemFont;

	for (MenuItem* line : lines)
		line->pFont = setItemFont;
}

void ScrollableList::Clear()
{
	for (MenuItem* line : lines)
	{
		line->Remove();
		delete line;
	}

	lines.clear();
	selected.clear();
	items.clear();
}

void ScrollableList::Refresh()
{
	if (firstItem >= 0 && lastItem < (signed) items.size())
	{
		int currItem = firstItem;

		for (auto it = lines.begin(); it != lines.end() && currItem <= lastItem; ++it, ++currItem)
		{
			MenuItem* line = *it;
			line->SetText(items[currItem].c_str());
			this->ApplyEffects(line, currItem);
		}
	}
}

void ScrollableList::Reset()
{
	for (bool select : selected)
		select = false;

	// reset sliding window
	if (items.size() > 0)
	{
		firstItem = 0;
		lastItem = static_cast<int> (lines.size() - 1);
	}
	else firstItem = lastItem = -1;

	this->Refresh();

	// reset velocity
	velocity = 0.f;

	if (lines.size() > 0)
	{
		auto actLine = lines.begin();
		(*actLine)->position = Vector2D(0, firstLinePosY);

		auto prevLine = actLine;
		actLine++;

		for (; actLine != lines.end(); prevLine = actLine, ++actLine)
			(*actLine)->position = (*prevLine)->position - Vector2D(0, 2.f * itemSize.y);
	}
}

void ScrollableList::ApplyEffects(MenuItem* item, unsigned int itemIndex)
{
	if (item != nullptr && itemIndex >= 0 && itemIndex < selected.size())
	{
		if (selected[itemIndex])
		{
			item->textColor = sColor(255, 255, 0, 0).GetValue();
			item->color = sColor(128, 255, 255, 255).GetValue();
		
		} else {

			item->textColor = sColor(255, 255, 255, 255).GetValue();
			item->color = sColor(128, 0, 0, 0).GetValue();
		}
	}
}

void ScrollableList::SelectItem(unsigned int index, bool select)
{
	if (index >= 0 && index < selected.size())
	{
		bool shouldChange = selected[index] ^ select;

		selected[index] = select;

		if (shouldChange && (signed) index >= firstItem && (signed) index <= lastItem)
		{
			MenuItem* selectedItem = lines[index - firstItem];
			this->ApplyEffects(selectedItem, index);
		}
	}
}

void ScrollableList::FocusItem(unsigned int index)
{
	// focus to last item
	if (items.size() > (unsigned) maxNrItems && lines.size() > 0 && index >= 0 && index < items.size())
	{
		int left = static_cast<int> ((lines.size() > 1) ? (lines.size() - 2) / 2 : 0);
		int right = static_cast<int> (lines.size() - left - 1);

		int newFirstItem = index - left;
		int newLastItem = index + right;

		if (newLastItem >= (signed) items.size())
		{
			int diff = static_cast<int> (newLastItem - items.size() + 1);
			newFirstItem -= diff;
			newLastItem -= diff;
		}
		
		if (newFirstItem < 0)
		{
			newLastItem -= newFirstItem;
			newFirstItem = 0;
		}

		if (firstItem != newFirstItem || lastItem != newLastItem)
		{
			this->Reset();
			
			firstItem = newFirstItem;
			lastItem = newLastItem;

			this->Refresh();
		}

		// update the lines to display the last item
		if (lastItem == items.size() - 1)
		{
			auto actLine = lines.rbegin();
			(*actLine)->position = Vector2D(0, lastLinePosY);

			auto prevLine = actLine;
			actLine++;

			for (; actLine != lines.rend(); prevLine = actLine, ++actLine)
				(*actLine)->position = (*prevLine)->position + Vector2D(0, 2.f * itemSize.y);
		}
	}
}

void ScrollableList::UpdateItems(float dt)
{
	Vector2D velDir = velocity;
	velDir.SafeNormalize();

	float curDragCoef = velocity.Length() / dt;

	curDragCoef = (curDragCoef > dragCoef) ? dragCoef : curDragCoef;

	// compute the acceleration
	Vector2D linearAcc = - curDragCoef * velDir;
	// compute the new velocity
	velocity += linearAcc * dt;
	
	// update in current frame
	Vector2D updateStep = velocity * dt;

	// remove small updates
	if (updateStep.LengthSq() < 1.f)
		updateStep = 0.f;

	// check for scrolling list's boundings
	if (lines.size() > 0)
	{
		if (updateStep.y < 0.f && firstItem == 0)
		{
			MenuItem* firstLine = lines.front();

			if (firstLine->position.y + updateStep.y < firstLinePosY)
				updateStep.y = -firstLine->position.y + firstLinePosY;

		}
		else if (updateStep.y > 0.f && lastItem == items.size() - 1) {

			MenuItem* lastLine = lines.back();

			if (lastLine->position.y + updateStep.y > lastLinePosY)
				updateStep.y = -lastLine->position.y + lastLinePosY;
		}
	}

	/** update lines positions **/
	for (MenuItem* line : lines)
		line->position += updateStep;

	/** update velocity with dampCoef **/
	velocity *= (1 - dampCoef);
}

void ScrollableList::HandleScrolling()
{
	if (items.size() > 0)
	{
		float firstLineY = lines.front()->position.y;

		// check for fade out/in for firstLine
		float firstProc = (topHiddenLineY - firstLineY) / itemSize.y;
		firstProc = fmaxf(firstProc, 0.f);
		firstProc = fminf(firstProc, 1.f);
		lines.front()->colorMask = LerpColor(hiddenMask, visibleMask, firstProc);

		// if firstLine exits the list, move it to the end, showing the next element
		// in the scrolling list on it
		if (lastItem < (signed) items.size() - 1 && firstLineY >= this->topHiddenLineY)
		{
			MenuItem* line = lines.front();
			lines.pop_front();

			++firstItem, ++lastItem;

			line->SetText(items[lastItem].c_str());
			this->ApplyEffects(line, lastItem);

			line->position = lines.back()->position - Vector2D(0, 2.f * itemSize.y);
			lines.push_back(line);
		}

		float lastLineY = lines.back()->position.y;

		// check for fade out/in for lastLine
		float lastProc = (lastLineY - bottomHiddenLineY) / itemSize.y;
		lastProc = fmaxf(lastProc, 0.f);
		lastProc = fminf(lastProc, 1.f);
		lines.back()->colorMask = LerpColor(hiddenMask, visibleMask, lastProc);

		// if lastLine exits the list, move it to the beginning, showing the previous element
		// in the scrolling list on it
		if (firstItem > 0 && lastLineY <= this->bottomHiddenLineY)
		{
			MenuItem* line = lines.back();
			lines.pop_back();

			--firstItem, --lastItem;

			line->SetText(items[firstItem].c_str());
			this->ApplyEffects(line, firstItem);

			line->position = lines.front()->position + Vector2D(0, 2.f * itemSize.y);
			lines.push_front(line);
		}
	}
}

void ScrollableList::Update(float dt)
{
	if (!active)
		return;

	/** scroll **/
	if (items.size() > (unsigned)maxNrItems)
	{
		if (canScroll)
		{
			Vector2D iniVelocity = Vector2D(0, -Input::GetGamepadRightStickY(-1));

			if (iniVelocity.LengthSq() == 0.f)
			{
				if (Input::IsKeyDown(VK_UP))
					iniVelocity.y -= 0.5f;
				if (Input::IsKeyDown(VK_DOWN))
					iniVelocity.y += 0.5f;
			}

			if (iniVelocity.LengthSq() > 0)
				velocity = iniVelocity * moveSpeed;

			this->UpdateItems(dt);
		}

		this->HandleScrolling();
	}
	
	MenuItem::Update(dt);
}

void ScrollableList::AddItem(const std::string& item)
{
	items.push_back(item);
	selected.push_back(false);

	if (lines.size() <= (unsigned) maxNrItems)
	{
		MenuItem* line = new MenuItem();
		line->pSprite = itemSprite;
		
		line->pFont = itemFont;
		line->textFormat = itemTextFormat;
		line->SetText(items.back().c_str());

		line->color = sColor(128, 0, 0, 0).GetValue();
		line->size = itemSize;
	
		if (lines.size() == 0)
		{
			line->position = Vector2D(0, firstLinePosY);
			// initialize the sliding window endings
			firstItem = lastItem = 0; 
		}
		else
		{
			line->position = lines.back()->position - Vector2D(0, 2.f * itemSize.y);
			// update the rightmost ending for the sliding window
			lastItem = (lastItem + 1 > maxNrItems) ? maxNrItems : lastItem + 1;
		}

		lines.push_back(line);
		this->AddChild(line);
	}
}

int ScrollableList::GetNumberItems() const
{
	return static_cast<int> (items.size());
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE