/****************************************************************
****************************************************************
*
* Tecto - Realistic heightmap generator based on the theories of plate tectonics.
* Copyright (C) 2013-2015 Mikael Hernvall (mikael.hernvall@gmail.com)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
****************************************************************
****************************************************************/

#ifndef VECTOR_H
#define VECTOR_H


template <class T>
struct Vector
{
    Vector();
    Vector(T xParam, T yParam);


    template <typename OperationType>
    Vector operator + (const Vector<OperationType>& b);
    template <typename OperationType>
    Vector operator + (const OperationType& b);

    template <typename OperationType>
    Vector operator - (const Vector<OperationType>& b);
    template <typename OperationType>
    Vector operator - (const OperationType& b);

    template <typename OperationType>
    Vector operator * (const Vector<OperationType>& b);
    template <typename OperationType>
    Vector operator * (const OperationType& b);

    template <typename OperationType>
    Vector operator / (const Vector<OperationType>& b);
    template <typename OperationType>
    Vector operator / (const OperationType& b);

    template <typename OperationType>
    Vector operator % (const Vector<OperationType>& b);
    template <typename OperationType>
    Vector operator % (const OperationType& b);

    template <typename OperationType>
    void operator = (const Vector<OperationType>& b);
    template <typename OperationType>
    void operator = (const OperationType& b);


    template <typename OperationType>
    bool operator == (const Vector<OperationType>& b);
    template <typename OperationType>
    bool operator == (const OperationType& b);


    template <typename OperationType>
    bool operator != (const Vector<OperationType>& b);
    template <typename OperationType>
    bool operator != (const OperationType& b);


    template <typename OperationType>
    void operator += (const Vector<OperationType>& b);
    template <typename OperationType>
    void operator += (const OperationType& b);

    template <typename OperationType>
    void operator -= (const Vector<OperationType>& b);
    template <typename OperationType>
    void operator -= (const OperationType& b);

    template <typename OperationType>
    void operator *= (const Vector<OperationType>& b);
    template <typename OperationType>
    void operator *= (const OperationType& b);

    template <typename OperationType>
    void operator /= (const Vector<OperationType>& b);
    template <typename OperationType>
    void operator /= (const OperationType& b);

    template <typename OperationType>
    void operator %= (const Vector<OperationType>& b);
    template <typename OperationType>
    void operator %= (const OperationType& b);

    T x;
    T y;
};

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

template <class T> template <typename OperationType>
Vector<T> Vector<T>::operator + (const Vector<OperationType>& b)
{
    return Vector<T>(x + b.x, y + b.y);
}

template <class T> template <typename OperationType>
Vector<T> Vector<T>::operator + (const OperationType& b)
{
    return Vector<T>(x + b, y + b);
}


template <class T> template <typename OperationType>
Vector<T> Vector<T>::operator - (const Vector<OperationType>& b)
{
    return Vector<T>(x - b.x, y - b.y);
}

template <class T> template <typename OperationType>
Vector<T> Vector<T>::operator - (const OperationType& b)
{
    return Vector<T>(x - b, y - b);
}


template <class T> template <typename OperationType>
Vector<T> Vector<T>::operator * (const Vector<OperationType>& b)
{
    return Vector<T>(x * b.x, y * b.y);
}

template <class T> template <typename OperationType>
Vector<T> Vector<T>::operator * (const OperationType& b)
{
    return Vector<T>(x * b, y * b);
}


template <class T> template <typename OperationType>
Vector<T> Vector<T>::operator / (const Vector<OperationType>& b)
{
    return Vector<T>(x / b.x, y / b.y);
}

template <class T> template <typename OperationType>
Vector<T> Vector<T>::operator / (const OperationType& b)
{
    return Vector<T>(x / b, y / b);
}


template <class T> template <typename OperationType>
Vector<T> Vector<T>::operator % (const Vector<OperationType>& b)
{
    return Vector<T>(x % b.x, y % b.y);
}

template <class T> template <typename OperationType>
Vector<T> Vector<T>::operator % (const OperationType& b)
{
    return Vector<T>(x % b, y % b);
}


template <class T> template <typename OperationType>
void Vector<T>::operator = (const Vector<OperationType>& b)
{
    x = b.x;
    y = b.y;
}

template <class T> template <typename OperationType>
void Vector<T>::operator = (const OperationType& b)
{
    x = b;
    y = b;
}


template <class T> template <typename OperationType>
bool Vector<T>::operator == (const Vector<OperationType>& b)
{
    return (x == b.x && y == b.y);
}

template <class T> template <typename OperationType>
bool Vector<T>::operator == (const OperationType& b)
{
    return (x == b && y == b);
}


template <class T> template <typename OperationType>
bool Vector<T>::operator != (const Vector<OperationType>& b)
{
    return (x != b.x || y != b.y);
}

template <class T> template <typename OperationType>
bool Vector<T>::operator != (const OperationType& b)
{
    return (x != b || y != b);
}



template <class T> template <typename OperationType>
void Vector<T>::operator += (const Vector<OperationType>& b)
{
    x += b.x;
    y += b.y;
}

template <class T> template <typename OperationType>
void Vector<T>::operator += (const OperationType& b)
{
    x += b;
    y += b;
}



template <class T> template <typename OperationType>
void Vector<T>::operator -= (const Vector<OperationType>& b)
{
    x -= b.x;
    y -= b.y;
}

template <class T> template <typename OperationType>
void Vector<T>::operator -= (const OperationType& b)
{
    x -= b;
    y -= b;
}


template <class T> template <typename OperationType>
void Vector<T>::operator *= (const Vector<OperationType>& b)
{
    x *= b.x;
    y *= b.y;
}

template <class T> template <typename OperationType>
void Vector<T>::operator *= (const OperationType& b)
{
    x *= b;
    y *= b;
}


template <class T> template <typename OperationType>
void Vector<T>::operator /= (const Vector<OperationType>& b)
{
    x /= b.x;
    y /= b.y;
}

template <class T> template <typename OperationType>
void Vector<T>::operator /= (const OperationType& b)
{
    x /= b;
    y /= b;
}

template <class T> template <typename OperationType>
void Vector<T>::operator %= (const Vector<OperationType>& b)
{
    x %= b.x;
    y %= b.y;
}

template <class T> template <typename OperationType>
void Vector<T>::operator %= (const OperationType& b)
{
    x %= b;
    y %= b;
}



typedef Vector<unsigned int>    Vectoru;
typedef Vector<int>             Vectori;
typedef Vector<float>           Vectorf;
typedef Vector<char>            Vectorc;

#endif // VECTOR_H
