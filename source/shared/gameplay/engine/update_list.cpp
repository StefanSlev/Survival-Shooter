#include "shared/gameplay/engine/update_list.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

UpdateList::UpdateList()
{
	pParent = NULL;
	pBrothers = NULL;
	pChildren = NULL;
}

UpdateList::~UpdateList()
{
	Remove();
	while (pChildren)
		delete pChildren;
}

UpdateList* UpdateList::AddChild(UpdateList *child)
{
	if (!child) return NULL;

	child->pParent = this;
	child->pBrothers = pChildren;
	pChildren = child;

	child->OnAddedAsChild();

	return child;
}

void UpdateList::Remove()
{
	if (!pParent)
		return;

	UpdateList *trans = pParent->pChildren;
	if (trans == this) {
		pParent->pChildren = pBrothers;
	}
	else {
		while (trans) {
			if (trans->pBrothers == this) {
				trans->pBrothers = pBrothers;
				break;
			}
			trans = trans->pBrothers;
		}
	}
	pParent = NULL;
}

void UpdateList::Update(float dt)
{
	UpdateList *child = pChildren;
	while (child) {
		child->Update(dt);
		child = child->pBrothers;
	}
}

void UpdateList::Render()
{
	UpdateList *child = pChildren;
	while (child) {
		child->Render();
		child = child->pBrothers;
	}
}

void UpdateList::UpdateGlobal()
{
	UpdateList *child = pChildren;
	while (child) {
		child->UpdateGlobal();
		child = child->pBrothers;
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE