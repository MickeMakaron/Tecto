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

#ifndef TECTO_PLATE_HPP
#define TECTO_PLATE_HPP

////////////////////////////////////////////////
// Tecto library
#include <BorderCrust.hpp>
////////////////////////////////////////////////

////////////////////////////////////////////////
// C++ Standard Library
#include <list>
////////////////////////////////////////////////

////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include <SFML/Graphics/VertexArray.hpp>
////////////////////////////////////////////////

namespace sf
{
    class RenderWindow;
}


class Plate
{
    public:
                Plate(std::vector<std::vector<Crust>>& heightmap, sf::Vector2u worldSize, std::list<BorderCrust> border);

        void update(float years);
        void draw(sf::RenderWindow& window);


        const std::vector<sf::Vector2i>&                            getOldCrustIndices() const;
        const std::vector<BorderCrust*>&                            getNewCrustIndices() const;

        void                            setVelocity(float x, float y);
        sf::Vector2f                    getVelocity() const;

        void                            setRotationalVelocity(float degrees);

        unsigned int getBorderCrustCount() const;
    private:
        void                            drawBorder(sf::RenderWindow& window) const;
        void                            move(sf::Vector2f distance);
        void                            rotate(float degrees);
        void                            moveBorder(sf::Vector2f distance, float degrees);
        void updateBorder();
        void                            fitIndexToWorldmap(sf::Vector2i& index);
        void                            fitPositionToWorldmap(sf::Vector2f& pos);
void loopCoords(sf::Vector2f& coords);
void loopTranslation();
void initializeDrawMap();

        sf::Vector2i                    mWorldSize; // Defined as signed int vector to prevent type conversion in Plate::fitIndexToWorldmap.
        sf::Vector2f                    mWorldSizef;
        std::vector<std::vector<Crust>>& mHeightmap;
        sf::VertexArray                 mDrawMap;
        //std::deque<std::deque<Crust>>   mHeightmap;// Two-dimensional deque (for efficient insertion/deletion at both ends) containing all Pixels belonging to Plate. To retain intuitive element access, i.e. mHeightmap[x][y] instead of mHeightmap[y][x], it contains deques containing Crusts ordered in ascending Y-position.
        std::list<BorderCrust>          mBorder; // Store pointers to the outermost Crusts of Plate's mHeightmap.
        sf::Vector2f                    mVelocity;
        sf::Vector2f                    mTranslation; // How many indices the Plate has moved from its original position.
        float                           mRotation; // How many degrees the plate has rotated.
        float                           mRotationalVelocity; // How many degrees per tick the plate is rotating.
        sf::Vector2f                    mRotationalCenter;
        sf::Transform                   mTransform;

        std::vector<BorderCrust*>       mNewCrustIndices;
        std::vector<sf::Vector2i>       mOldCrustIndices;
};

#endif // TECTO_PLATE_HPP
