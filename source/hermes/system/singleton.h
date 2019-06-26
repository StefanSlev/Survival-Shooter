#ifndef HERMES_SYSTEM_SINGLETON
#define HERMES_SYSTEM_SINGLETON

#include "hermes/system/system_base.h"
#include "hermes/system/non_copyable.h"

BEGIN_HERMES_NAMESPACE

BEGIN_SYSTEM_NAMESPACE

template <class Object> class Singleton : public NonCopyable
{
public:
	static Object& GetInstance()
	{
		static Object instance;
		return instance;
	}

protected:
	Singleton() {}
	~Singleton() override {}
};

END_SYSTEM_NAMESPACE

END_HERMES_NAMESPACE

#define SINGLETON_ACCESS(Object) \
friend class hermes::system::Singleton<Object>;

#endif // HERMES_SYSTEM_SINGLETON