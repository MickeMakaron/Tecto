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

#ifndef TECTO_LITHOSPHERE_HPP
#define TECTO_LITHOSPHERE_HPP

////////////////////////////////////////////////
// Tecto library
#include <Plate.hpp>
////////////////////////////////////////////////


////////////////////////////////////////////////
// C++ Standard Library
#include <vector>
#include <cstdint>
#include <memory>
////////////////////////////////////////////////

class Lithosphere
{
    public:
        /*
         * A Plume represents a mantle plume, a theoretical diapir shooting out from the
         * core-mantle boundary into the lithosphere. They are thought to have taken part
         * in the formation of plate boundaries and propel plates away from it.
         *
         * In the context of Tecto their purpose is to be circular nodes generated
         * randomly across the heightmap. Its indices and other properties are absolute
         * and will never change during the simulation. All its member variables are
         * thus constant. During plate initialization the plumes will act as hubs
         * for the plates' boundaries.
         */
        struct Plume
        {
            Plume(sf::Vector2i index, unsigned int radius, unsigned int intensity)
            : mIndex(index)
            , mRadius(radius)
            , mIntensity(intensity)
            {};

            sf::Vector2i      mIndex;
             int      mRadius;
             int      mIntensity;
        };


        typedef std::unique_ptr<Plate> PlatePtr;

                Lithosphere(unsigned int worldSizeX, unsigned int worldSizeY);

        void    initializePlumes(sf::Vector2u worldSize);
        void    initializePlates(sf::Vector2u worldSize);
        void    initializeDrawMap();

        void    update(float years);
        void    draw(sf::RenderWindow& window) const;
        void    drawPlumes(sf::RenderWindow& window) const;


        void            solveCollision(BorderCrust* crust, int plateIndex);
        void            populateEmptyIndex(sf::Vector2i index);
        void            handlePlateMovement();
        sf::Vector2i    fitIndexToHeightmap(sf::Vector2i index) const;
        void            registerFrontAndBackCrusts(std::vector<std::vector<BorderCrust*>>& frontCrusts, std::vector<sf::Vector2i>& backCrusts);

        const std::vector<PlatePtr>& getPlates() const;

    private:
        std::vector<Plume>                  mPlumeTypes; // 0 = big, 1 = medium, 2 = small
        std::vector<std::unique_ptr<Plate>> mPlates;
        std::vector<std::vector<Crust>>     mHeightmap;
        sf::VertexArray                     mDrawMap;
        sf::VertexArray                     mBorders; // TEMPORARY
        std::vector<Plume>                  mPlumes;
        std::vector<std::vector<int8_t>>    mIndexOccupancyMap;
        sf::Vector2u                        mSize;

};

#endif // TECTO_LITHOSPHERE_HPP
