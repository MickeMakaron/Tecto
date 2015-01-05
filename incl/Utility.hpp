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

#ifndef TECTO_UTILITY_HPP
#define TECTO_UTILITY_HPP

// Pi for float
#define PI 3.1415926f

/*
template <typename container1, typename container2>
void append(container1& c1, const container2& c2)
{
    c1.insert(c1.end(), c2.begin(), c2.end());
}


template <typename vector1, typename vector2>
void add(vector1& v1, const vector2& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
}

template <typename vector1, typename vector2, typename vector3>
void add(vector1& v1, const vector2& v2, const vector3& v3)
{
    v1.x = v2.x + v3.x;
    v1.y = v2.y + v3.y;
}


template <typename vector1, typename vector2, typename vector3>
void subtract(vector1& v1, const vector2& v2, const vector3& v3)
{
    v1.x = v2.x - v3.x;
    v1.y = v2.y - v3.y;
}
*/

float degreeToRadian(const float& degree);

#endif // TECTO_UTILITY_HPP




