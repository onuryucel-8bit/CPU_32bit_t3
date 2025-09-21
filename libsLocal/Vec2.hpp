#pragma once
#include <math.h>
namespace sml//standart math lib
{


	template <class T>
	class Vector2
	{
	public:
		Vector2();
		Vector2(T x_, T y_);
		~Vector2();

		T distance(Vector2 vec2);

		//operator overloadings
		Vector2<T> operator+(Vector2 vec2);
		Vector2<T> operator-(Vector2 vec2);

		T x, y;
	private:


	};

	template<class T>
	Vector2<T>::Vector2()
	{
	}

	template <typename T>
	Vector2<T>::Vector2(T x_, T y_)
	{
		x = x_;
		y = y_;
	}

	template <typename T>
	Vector2<T>::~Vector2()
	{
	}

	template<class T>
	Vector2<T> Vector2<T>::operator+(Vector2 vec2)
	{
		return Vector2<T>(vec2.x + x, vec2.y + y);
	}

	template<class T>
	Vector2<T> Vector2<T>::operator-(Vector2 vec2)
	{
		return Vector2<T>(vec2.x - x, vec2.y - y);
	}

	template<class T>
	T Vector2<T>::distance(Vector2 vec2)
	{
		//sqrt(x^2 - y^2);
		return sqrt((x - vec2.x) * (x - vec2.x) + (y - vec2.y) * (y - vec2.y));
	}

	using vec2f = Vector2<float>;
	using vec2u = Vector2<unsigned int>;
}