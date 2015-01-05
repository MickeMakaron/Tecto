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

#ifndef TECTO_CRUST_HPP
#define TECTO_CRUST_HPP



// This is essentially the crust that is not on the plate's border.
// Maybe possible to only have some of the outermost Crusts loaded into memory.
// If border is updated, then load nearby Crusts to memory either on the main thread or on another thread.
// This would only be necessary for big maps. Bigger maps take longer to tick and therefore the other thread would have more time to load to memory.
class Crust
{
    public:
                    Crust(unsigned int time);



        bool    isContinental() const;
        void    setContinental(bool flag);

        void    offsetHeight(int offset);
        void    setHeight(unsigned int);

        unsigned int        getHeight() const;
        unsigned int        getTimeCreated() const;

    private:
        unsigned int        mHeight;
        bool                mIsContinental;
        const unsigned int  mTimeCreated;
};

#endif // TECTO_CRUST_HPP
