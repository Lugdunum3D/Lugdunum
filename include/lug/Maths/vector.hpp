#pragma once

typedef Vector<float, 2> Vector2f;
typedef Vector<float, 3> Vector3f;
typedef Vector<float, 4> Vector4f;

template <typename T, int dimensions> class Vector
{
public:
	Vector();
	~Vector();
	T values[dimensions];
};

template <typename T> class Vector<T,2> {
	union 
	{
		T values[2];
		struct {
			float x, y;
		};
	};
};

template <typename T> class Vector<T, 3> {
	union
	{
		T values[3];
		struct {
			float x, y, z;
		};
	};
};

template <typename T> class Vector<T, 4> {
	union
	{
		T values[4];
		struct {
			float x, y, z, w;
		};
		struct {
			float r, g, b, a;
		};
	};
};


