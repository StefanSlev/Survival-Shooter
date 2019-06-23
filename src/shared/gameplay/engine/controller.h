#ifndef SHARED_GAMEPLAY_ENGINE_CONTROLLER
#define SHARED_GAMEPLAY_ENGINE_CONTROLLER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/update_list.h"

// -------------------------------------------------------------------------------------
// Class that is used to extend an Object's behavior, if deriving the Object is not a good option.
// Mostly useful if the extended behavior offered by this controller is common to multiple
//	object types or you already have an instance of an object created by the engine and
//	deriving it is not an option.
// Overide the Update method to change the functionality of the parent object
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Object;

class Controller : public UpdateList
{
public:
	Controller();
	~Controller();

	virtual void SetObject(Object * _object) { object = _object; }
	virtual void Remove();

	virtual ObjectType GetType() const { return ObjectType::Controller; }
	virtual bool IsTypeOf(ObjectType type) const { return (type == ObjectType::Controller) || UpdateList::IsTypeOf(type); }

protected:
	Object* object;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_CONTROLLER