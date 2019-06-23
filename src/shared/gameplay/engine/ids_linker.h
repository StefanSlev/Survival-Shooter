#ifndef SHARED_GAMEPLAY_ENGINE_IDS_LINKER
#define SHARED_GAMEPLAY_ENGINE_IDS_LINKER

#include "shared/gameplay/gameplay_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

// KeyId and LinkedId must be integer types
template <class KeyId, class LinkedId> class IDsLinker
{
public:
	bool RegisterForIdLinkage(KeyId id, LinkedId linkedId);
	bool RemoveFromIdLinkage(KeyId id);

	bool GetLinkedId(KeyId id, LinkedId& linkedId) const;
	void ClearLinker();

private:
	std::unordered_map<KeyId, LinkedId> _IDs_linker;
};

template <class KeyId, class LinkedId> bool IDsLinker<KeyId, LinkedId>::RegisterForIdLinkage(KeyId id, LinkedId linkedId)
{
	if (id < 0 || linkedId < 0)
		return false;

	auto it = _IDs_linker.find(id);

	if (it == _IDs_linker.end())
	{
		_IDs_linker[id] = linkedId;
		return true;
	}

	return false;
}

template <class KeyId, class LinkedId> bool IDsLinker<KeyId, LinkedId>::GetLinkedId(KeyId id, LinkedId& linkedId) const
{
	auto it = _IDs_linker.find(id);

	if (it != _IDs_linker.end())
	{
		linkedId = it->second;
		return true;
	}

	return false;
}

template <class KeyId, class LinkedId> bool IDsLinker<KeyId, LinkedId>::RemoveFromIdLinkage(KeyId id)
{
	auto it = _IDs_linker.find(id);

	if (it != _IDs_linker.end())
	{
		_IDs_linker.erase(it);
		return true;
	}

	return false;
}

template <class KeyId, class LinkedId> void IDsLinker<KeyId, LinkedId>::ClearLinker()
{
	_IDs_linker.clear();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_IDS_LINKER