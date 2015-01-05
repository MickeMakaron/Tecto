#include <Vector.hpp>



template <class T>
Vector<T>::Vector()
: x(0)
, y(0)
{

}

template <class T>
Vector<T>::Vector(T xParam, T yParam)
: x(xParam)
, y(yParam)
{

}

template <class T>
Vector<T> Vector<T>::operator + (const Vector& b)
{
    Vector<T> tmp;
    tmp.x = x + b.x;
    tmp.y = y + b.y;

    return tmp;
}
