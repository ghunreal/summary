#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <stdio.h>

template <typename T>
class Singleton
{
public:
	static T* instance()
	{
		if (_spInstance == NULL)
			_spInstance = new T;
		return _spInstance;
	}
	
	static void destroy()
	{
		if (_spInstance != NULL)
		{
			delete _spInstance;
			_spInstance = NULL;
		}
	}
	
private:
	static T* _spInstance;
};

template<typename T> T* Singleton<T>::_spInstance = NULL;

//#define SINGLETON_INITILIAZE(T) template<> T* Singleton<T>::_spInstance = NULL

#endif