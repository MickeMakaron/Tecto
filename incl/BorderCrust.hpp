#ifndef TECTO_BORDERCRUST_HPP
#define TECTO_BORDERCRUST_HPP

////////////////////////////////////////////////
// Tecto library
#include <Crust.hpp>
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

////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include <SFML/System/Vector2.hpp>
////////////////////////////////////////////////


class BorderCrust
{
    public:
                BorderCrust(sf::Vector2i index, Crust& mSourceCrust);

        void    update();

        void    offsetRadiusVector(const sf::Vector2f& offset);
        void    offsetIndex(sf::Vector2i offset);
        void    setIndex(int x, int y);

        sf::Vector2i getOriginalIndex() const;
        const sf::Vector2i& getIndex() const;
        const sf::Vector2f& getRadiusVector() const;
        Crust& getSourceCrust();


    protected:
        Crust&          mSourceCrust;
        const sf::Vector2i    mOriginalIndex;
        sf::Vector2i    mIndex;
        sf::Vector2f    mRadiusVector; // Difference-vector between mPos and the crust's rotational center. (see Plate::moveBorder to see it in use)
};

#endif // TECTO_BORDERCRUST_HPP
