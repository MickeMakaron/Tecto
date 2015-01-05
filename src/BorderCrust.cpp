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

////////////////////////////////////////////////
// Tecto library
#include <BorderCrust.hpp>
////////////////////////////////////////////////


BorderCrust::BorderCrust(sf::Vector2i index, Crust& sourceCrust)
: mOriginalIndex(index)
, mIndex(mOriginalIndex)
, mRadiusVector(0, 0)
, mSourceCrust(sourceCrust)
{
}

void BorderCrust::update()
{
}

void BorderCrust::setIndex(int x, int y)
{
    mIndex.x = x;
    mIndex.y = y;
}

void BorderCrust::offsetIndex(sf::Vector2i offset)
{
    mIndex += offset;
}

void BorderCrust::offsetRadiusVector(const sf::Vector2f& offset)
{
    mRadiusVector += offset;
}

const sf::Vector2f& BorderCrust::getRadiusVector() const
{
    return mRadiusVector;
}

Crust& BorderCrust::getSourceCrust()
{
    return mSourceCrust;
}

sf::Vector2i BorderCrust::getOriginalIndex() const
{
    return mOriginalIndex;
}

const sf::Vector2i& BorderCrust::getIndex() const
{
    return mIndex;
}
