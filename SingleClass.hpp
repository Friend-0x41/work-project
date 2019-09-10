#ifndef SINGLECLASS_HPP_
#define SINGLECLASS_HPP_

template<typename T>
class SingleClass
{
public:

	static T* getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new T;
		}
		return _instance;
	}
private:

	static T* _instance;
};

template<typename T>
T* SingleClass<T>::_instance = nullptr;
#endif