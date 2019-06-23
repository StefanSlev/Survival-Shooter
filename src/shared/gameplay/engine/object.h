#ifndef SHARED_GAMEPLAY_ENGINE_OBJECT
#define SHARED_GAMEPLAY_ENGINE_OBJECT

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/transform2d.h"
#include "shared/gameplay/engine/update_list.h"

// -------------------------------------------------------------------------------------
// Class that defines the most basic game Object.
// The object has an associated sprite (image), a size, a transform (position, rotation & scale) and a color.
// An Object's functionality can be extended through the use of Controller(s), if deriving is not an option.
// More advanced game objects are derived from the object class, like the WorldObject & MenuItem.
//
// Note: an Object has in fact 2 transforms - the implicit (from Transform2D inheritance) is the local space
//	transform, and is used during update, and the explicit "global", that is the global space transform and is
//	used during rendering; the global is computed based on the local (implicit) one in the UpdateGlobal method,
//	aftr the game's Update step
//
// Note: See also WorldObject below!!!
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class ObjectGlobalValues : public Transform2D
{
public:
	ObjectGlobalValues(Vector2D _position = { 0 , 0 }, float _rotation = 0, Vector2D _scale = { 1, 1 }, long _colorMask = 0xFFFFFFFF) : Transform2D(_position, _rotation, _scale)
	{
		colorMask = _colorMask;
	}

	long			colorMask;

	void Init() override;
};

class Sprite;
class Controller;

class Object : public UpdateList, public ObjectGlobalValues
{
public:
	Object(Sprite *sprite = NULL);
	~Object();

	// hierarchy management functions
	virtual UpdateList* AddChild(UpdateList *child);

	// Use this function to add controllers to your object (controllers receive the Update calls and can alter the behavior of the Object they are attached to)
	Controller* AddController(Controller *controller);

	// Use this to find an existing controller on your object
	Controller* FindController(ObjectType type);

	// Updates this object's hierarchy / children
	// Don't forget to call the Object::Update funcion if you override Update in a derived class
	virtual void Update(float dt);

	// Renders this object's hierarchy / children
	// Don't forget to call the Object::Render funcion if you override Render in a derived class
	virtual void Render();

	// Can be caller after the Render step to draw any additional debug informatin
	// Don't forget to call the Object::DebugRender funcion if you override DebugRender in a derived class
	virtual void DebugRender();

	// Call this on the root of the hierarchy before the Render step to update the global values (positions, rotations, scale...) for the entire hierarchy
	virtual void UpdateGlobal();

	virtual ObjectType GetType() const { return ObjectType::Object; }
	virtual bool IsTypeOf(ObjectType type) const { return (type == ObjectType::Object) || UpdateList::IsTypeOf(type); }

	int GetID() const { return _ID; }
	// Use this to find an existing object by its ID
	static Object * FindByID(int id);

	virtual void Activate() { active = true; visible = true; }
	virtual void Deactivate() { active = false; visible = false; }

	virtual bool IsAlive() const { return active && !starting && !ending; }

	// You can override the Start/End OnStart/OnEnd to add Spwan / Kill, Enter / Exit animations
	virtual void Start() { Activate(); starting = true; ending = false; OnStart(); }
	virtual void End() { starting = false; ending = true; OnEnd(); }
	virtual void OnStart() { starting = false; }
	virtual void OnEnd() { ending = false; Deactivate(); }

	ObjectGlobalValues	global;

	Vector2D		size;
	long			color;
	Sprite*			pSprite;
	Vector2D		spriteTopLeftUV;
	Vector2D		spriteBottomRightUV;

	bool			active;
	bool			visible;
	bool			starting;
	bool			ending;

	Controller*		pControllers;

	void Init() override;

private:
	int					_ID;

	static int			_IDGenerator;
	static std::unordered_map<int, Object*> objectMap;
};

// -------------------------------------------------------------------------------------
// Class that defines the basic static game / world Object.
// It adds the functionality of Rendering in the World Space, needed for all objects part of the world.
// -------------------------------------------------------------------------------------

class WorldObject : public Object
{
public:
	WorldObject(Sprite *sprite = NULL) : Object(sprite) {}
	virtual void Render();

	virtual ObjectType GetType() const { return ObjectType::WorldObject; }
	virtual bool IsTypeOf(ObjectType type) const { return type == ObjectType::WorldObject ||  Object::IsTypeOf(type); }
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_OBJECT