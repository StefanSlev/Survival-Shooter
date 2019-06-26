#include "shared/gameplay/elements/menu_pages/leaderboard.h"
#include "shared/gameplay/engine/resource_manager.h"
#include "shared/online/leaderboard_service.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* Leaderboard::menuFileName = "Data\\Images\\white.png";

Leaderboard::Leaderboard(Vector2D size = 0.f, Vector2D position = 0.f) : MenuItem()
{
	this->pFont = Font::g_DefaultLargeFont;
	this->textFormat = DT_TOP | DT_CENTER;
	this->SetText("Leaderboard");
	this->size = size;
	this->position = position;

	Vector2D textSize = pFont->GetTextSize(text, this, textFormat);
	Vector2D listSize = Vector2D(0.75f * size.x, size.y - textSize.y - 10.f);
	Vector2D listPosition = Vector2D(0, (-this->size.y + listSize.y) * 0.5f);

	scrollableList = new ScrollableList(listSize);
	scrollableList->SetItemSprite(ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, menuFileName));
	scrollableList->position = listPosition;
	this->AddChild(scrollableList);
}

void Leaderboard::Refresh()
{
	scrollableList->Clear();

	// load leaderboard
	const auto& entries = GetLeaderboardService().Leaderboard();

	for (const auto& entry : entries)
	{
		std::stringstream stream;
		stream << entry.name << ": " << entry.score;

		scrollableList->AddItem(stream.str());
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE