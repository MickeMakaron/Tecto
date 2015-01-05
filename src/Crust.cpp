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
#include <Crust.hpp>
////////////////////////////////////////////////

Crust::Crust(unsigned int time)
: mHeight(100)
, mIsContinental(false)
, mTimeCreated(time)
{
}

void Crust::offsetHeight(int offset)
{
    mHeight += offset;
}

void Crust::setHeight(unsigned int height)
{
    mHeight = height;
}


unsigned int Crust::getHeight() const
{
    return mHeight;
}

bool Crust::isContinental() const
{
    return mIsContinental;
}



void Crust::setContinental(bool flag)
{
    mIsContinental = flag;
}

unsigned int Crust::getTimeCreated() const
{
    return mTimeCreated;
}
