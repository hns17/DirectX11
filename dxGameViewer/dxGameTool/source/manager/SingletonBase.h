#pragma once
//=============================================================
//	## singleTonBase ## (ΩÃ±€≈Ê ∆–≈œ)
//=============================================================
template<class T>
class SingletonBase
{
protected:
	//ΩÃ±€≈Ê ¿ŒΩ∫≈œΩ∫ º±æ
	static unique_ptr<T> _instance;

	SingletonBase() {}
	~SingletonBase() {}
public:
	static T& GetSingleton(void);
};

//ΩÃ±€≈Ê √ ±‚»≠
template<class T>
unique_ptr<T> SingletonBase<T>::_instance;

//ΩÃ±€≈Ê ∞°¡Æø¿±‚
template<class T>
inline T& SingletonBase<T>::GetSingleton(void)
{
	//ΩÃ±€≈Ê¿Ã æ¯¿∏∏È ªı∑Œ ª˝º∫«œ±‚ »ƒ ΩÃ±€≈Ê ∏Æ≈œ
	if (!_instance) _instance = make_unique<T>();
	return *(_instance.get());
}

