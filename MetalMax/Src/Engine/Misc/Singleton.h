#ifndef _Singleton_H_
#define _Singleton_H_

#include <assert.h>

template <typename T> class Singleton
{
public:
	Singleton(void)
	{
		assert(!spInstance);
		spInstance=(T*)this;
	}

	virtual ~Singleton(void)
	{
		assert(spInstance);
		spInstance=0;
	}

	static T& GetInstance(void)
	{
		assert(spInstance);
		return *spInstance;
	}

	static bool IsInitialized(void)
	{
		return spInstance!=0;
	}

protected:
	static T *spInstance;

};
#define DECLARE_SINGLETON(T) template<> T *Singleton<T>::spInstance=0

#endif
