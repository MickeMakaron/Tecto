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

#include <Lithosphere.hpp>
#include <Utility.hpp>
#include <Vector.hpp>

////////////////////////////////////////////////
// C++ Standard Library
//////////////////////
// DEBUG
#include <iostream>
//////////////////////
////////////////////////////////////////////////


////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include <SFML/Graphics/RenderWindow.hpp>
////////////////////////////////////////////////




Lithosphere::Lithosphere(unsigned int worldSizeX, unsigned int worldSizeY)
: mIndexOccupancyMap(worldSizeX, std::vector<int8_t>(worldSizeY, 1))
, mSize(worldSizeX, worldSizeY)
{
    Crust crust(0);
    crust.setContinental(true);

    mHeightmap = std::vector<std::vector<Crust>>(worldSizeX, std::vector<Crust>(worldSizeY, crust));

    /////////////////////////////////////////////////////////////////////
    // DEBUG
    // Just checkin' the memory size of them crusts.
    std::cout   << "Sizes of different classes..." << std::endl
                << "Crust: " << sizeof(Crust) << " bytes" << std::endl
                << "BorderCrust: " << sizeof(BorderCrust) << " bytes" << std::endl;
    /////////////////////////////////////////////////////////////////////

    sf::Vector2u worldSize(worldSizeX, worldSizeY);
    //initializePlumes(worldSize);
    //initializePlates(worldSize);


    int halfWorldSizeX = worldSizeX/2;
    int halfWorldSizeY = worldSizeY/2;

    std::list<BorderCrust> border;


    auto initBorder = [](std::list<BorderCrust>& border, int top, int right, int bot, int left, std::vector<std::vector<Crust>>& heightmap)
    {
        for(int x = left; x < right; x++)
            border.push_back(BorderCrust(sf::Vector2i(x, top), heightmap[x][top]));

        for(int y = top; y < bot; y++)
            border.push_back(BorderCrust(sf::Vector2i(right, y), heightmap[right][y]));

        for(int x = right; x > left; x--)
            border.push_back(BorderCrust(sf::Vector2i(x, bot), heightmap[x][bot]));

        for(int y = bot; y > top; y--)
            border.push_back(BorderCrust(sf::Vector2i(left, y), heightmap[left][y]));
    };
    // Top-left plate
    initBorder(border, 1, halfWorldSizeX, halfWorldSizeY, 1, mHeightmap);
    mPlates.push_back(std::unique_ptr<Plate>(new Plate(mHeightmap, worldSize, border)));
    border.clear();

    // Top-right plate
    initBorder(border, 1, worldSizeX - 1, halfWorldSizeY, halfWorldSizeX, mHeightmap);
    mPlates.push_back(std::unique_ptr<Plate>(new Plate(mHeightmap, worldSize, border)));
    border.clear();

    // Bottom-left plate
    initBorder(border, halfWorldSizeY, halfWorldSizeX, worldSizeY - 1, 1, mHeightmap);
    mPlates.push_back(std::unique_ptr<Plate>(new Plate(mHeightmap, worldSize, border)));
    border.clear();

    // Bottom-right plate
    initBorder(border, halfWorldSizeY, worldSizeX - 1, worldSizeY - 1, halfWorldSizeX, mHeightmap);
    mPlates.push_back(std::unique_ptr<Plate>(new Plate(mHeightmap, worldSize, border)));


  //  for(PlatePtr& pPlate : mPlates)
    {
        mPlates[3]->setVelocity(-.2f, -.2f);
        mPlates[3]->setRotationalVelocity(0.05f);
    }

    initializeDrawMap();


    std::cout   << "Memory report (kB)" << std::endl;

    for(int i = 0; i < mPlates.size(); i++)
        std::cout   << "Plate border " << i << ": " << sizeof(BorderCrust) * mPlates[i]->getBorderCrustCount() / 1000 << std::endl
                    << "Plate draw map " << i << ": " << sizeof(sf::Vertex) * mPlates[i]->getBorderCrustCount() / 1000 << std::endl;

    std::cout   << "Heightmap: " << sizeof(mHeightmap) * mSize.x * mSize.y / 1000 << std::endl
                << "Draw map: " << sizeof(sf::Vertex) * mSize.x * mSize.y / 1000 << std::endl
                << "Occupancy map: " << sizeof(mIndexOccupancyMap) * mSize.x * mSize.y / 1000 << std::endl;
}

void Lithosphere::initializePlumes(sf::Vector2u worldSize)
{
    // Big plume
    Plume plume(sf::Vector2i(0, 0), 20, 100);
    mPlumeTypes.push_back(plume);

    // Medium plume
    plume.mRadius /= 2;
    plume.mIntensity /= 2;
    mPlumeTypes.push_back(plume);

    // Small plume
    plume.mRadius /= 2;
    plume.mIntensity /= 2;
    mPlumeTypes.push_back(plume);


    // Initialize seed.
    std::srand(time(NULL));

    // Randomize number of plumes between 30 and 70. Earth has roughly 50.
    int nPlumes = std::rand() % 40 + 30;

    /*
     * Randomize the distribution of three different plume sizes. Earth's is roughly 19% big, 25% medium and 56% small.
     * Randomize the percentage in ints and then convert it to decimal form. I.e 19% -> 0.19.
     * Then multiply it with the number of plumes to get the final number.
     */

    // Percentage of big plumes: 15-25%
    int nBigPlumes = (std::rand() % 10 + 15) / 100.f * nPlumes;

    // Percentage of medium plumes: 20-30%
    int nMediumPlumes = (std::rand() % 10 + 20) / 100.f * nPlumes;

    // The rest of the plumes get to be small plumes.
    // Percentage of small plumes: 45-65%
    int nSmallPlumes = nPlumes - nBigPlumes - nMediumPlumes;

    std::cout << "nPlumes: " << nPlumes << std::endl
            << "nBigPlumes: " << nBigPlumes << std::endl
            << "nMediumPlumes: " << nMediumPlumes << std::endl
            << "nSmallPlumes: " << nSmallPlumes << std::endl
            << "Sum: " << nBigPlumes + nMediumPlumes + nSmallPlumes << std::endl;


    /*
     * Place the plumes on the heightmap.
     * They tend to be situated near the equator.
     */

    /* // Randomize minimum and maximum Y-values.
     int minRange = worldSize.y / 4;
     int halfMinRange = minRange / 2;
     int min = std::rand() % (minRange - halfMinRange) + (minRange + halfMinRange);

     int maxRange = 3 * worldSize.y / 4;
     int halfMaxRange = halfMinRange;
     int max = std::rand() % (maxRange - halfMaxRange) + (maxRange + halfMaxRange);
     */

    sf::Vector2i index;
    // Big plumes
    plume = mPlumeTypes[0];
    for(int i = 0; i < nBigPlumes; i++)
    {
        index.x = std::rand() % worldSize.x;
        index.y = std::rand() % worldSize.y;

        plume.mIndex = index;
        mPlumes.push_back(plume);
    }

    // Medium plumes
     plume = mPlumeTypes[1];
     for(int i = 0; i < nMediumPlumes; i++)
     {
         index.x = std::rand() % worldSize.x;
         index.y = std::rand() % worldSize.y;

         plume.mIndex = index;
         mPlumes.push_back(plume);
     }

     // Small plumes
     plume = mPlumeTypes[2];
     for(int i = 0; i < nSmallPlumes; i++)
     {
         index.x = std::rand() % worldSize.x;
         index.y = std::rand() % worldSize.y;

         plume.mIndex = index;
         mPlumes.push_back(plume);
     }
}

void Lithosphere::initializePlates(sf::Vector2u worldSize)
{
    std::vector<Vectori> border;


    std::srand(time(NULL));

    int nPlates = rand() % 6 + 5; // 5-10


    Vectoru wSize(worldSize.x, worldSize.y);

    for(int i = 0; i < nPlates; i++)
    {
        const Vectoru spawnAreaMax = wSize * 4/5;
        const Vectoru spawnAreaMin = wSize * 1/5;

        sf::Vector2u heightmapIndex;
        do
        {
            heightmapIndex.x = rand() % spawnAreaMax.x + spawnAreaMin.x;
            heightmapIndex.y = rand() % spawnAreaMax.y + spawnAreaMin.y;
        } while(true);


    }

/*
THIS
    // Initialize seed.
    std::srand(time(NULL));

    for(const Plume& plume : mPlumes)
        if(plume.mRadius == mPlumeTypes[0].mRadius)
        {
            border.push_back(plume.mIndex);
            break;
        }
    auto iCurrent = border.begin();


    std::list<Plume> plumes;
    for(Plume plume : mPlumes)
        plumes.push_back(plume);


    sf::Vector2f force(0, 0);
    float radiusModifier = 1.f;
    while(border.size() < worldSize.y)
    {
        while(force.x == 0 && force.y == 0)
        {
            for(auto iPlume = plumes.begin(); iPlume != plumes.end(); iPlume++)
            {
                sf::Vector2i dv = iPlume->mIndex - *iCurrent;
                float radius = iPlume->mRadius * radiusModifier;
                float dSqrd = dv.x * dv.x + dv.y * dv.y;


                if(dSqrd <= (radius * radius))
                {
                    if(dSqrd <= (radius * radius) / 10.f)
                    {
                        auto iTmp = iPlume;
                        iTmp++;
                        plumes.erase(iPlume);

                        if(iTmp == plumes.end())
                            break;

                        iPlume = iTmp;

                    }
                    else
                    {
                        if(dv.x != 0)
                            force.x += iPlume->mIntensity / (dv.x);
                        if(dv.y != 0)
                            force.y += iPlume->mIntensity / (dv.y);
                    }
                }
            }

            radiusModifier += 10.f;
        }

        radiusModifier = 0.f;

        sf::Vector2i direction(0, 0);

        if(force.x == 0)
            direction.x = 0;
        else
            direction.x = force.x > 0 ? 1 : -1;

        if(force.y == 0)
            direction.y = 0;
        else
            direction.y = force.y > 0 ? 1 : -1;
*/





       /*
       NOT THIS
       if(direction == sf::Vector2i(0, 0))
        {
            direction.x = std::rand() % 2 - 1;

            if(direction.x == 0)
                direction.y = 1;
            else
                direction.y = std::rand() % 2 - 1;
        }*/



/*
THIS
        std::cout << "Force: " << force.x << "x" << force.y << std::endl;
        force = sf::Vector2f(0, 0);


        if(iCurrent != border.begin())
            if(*iCurrent + direction == *(iCurrent - 1))
                direction -= direction * 2;

        sf::Vector2i index = *iCurrent + direction;
*/



       /*
       NOT THIS if(index.x < 0)
            index.x = (worldSize.x - 1) - (-index.x % worldSize.x);
        else if(index.x > worldSize.x - 1)
            index.x = index.x % worldSize.x;

        if(index.y < 0)
            index.y = (worldSize.y - 1) - (-index.y % worldSize.y);
        else if(index.y > worldSize.y - 1)
            index.y = index.y % worldSize.y;
*/

/*
 THIS
        border.push_back(index);
        iCurrent = border.begin() + border.size() - 1;
    }

    for(int i = 0; i < 10; i++)
        std::cout << i << ": " << border[i].x << "x" << border[i].y << std::endl;

    std::cout << "Force: " << force.x << "x" << force.y << std::endl;

    sf::VertexArray array(sf::Points, border.size());
    sf::Vertex v;
    v.color = sf::Color::White;

    for(int i = 0; i < border.size(); i++)
    {
        v.position.x = border[i].x;
        v.position.y = border[i].y;

        array[i] = v;
    }

    mBorders = array;

    */
/*
 NOT THIS
    std::vector<Plume> plumes;
    for(const Plume& plume : mPlumes)
        if(plume.mRadius == 20)
            plumes.push_back(plume);

    std::vector<Plume>::iterator iCurrentPlume, iClosestPlume;
    iCurrentPlume = iClosestPlume = plumes.begin();
    sf::Vector2i vCurrentPlume = iCurrentPlume->mIndex;
    sf::Vector2i d;
    float minDistanceSqrd;
    float distanceSqrd;
    float maxDistanceSqrd = worldSize.x * worldSize.x + worldSize.y * worldSize.y;
    while(plumes.size() > 1)
    {
        minDistanceSqrd = maxDistanceSqrd;

        border.push_back(iCurrentPlume->mIndex);
        plumes.erase(iCurrentPlume);

        for(auto it = plumes.begin(); it != plumes.end(); it++)
        {
            d = it->mIndex - vCurrentPlume;
            distanceSqrd = d.x * d.x + d.y * d.y;

            if(distanceSqrd <= minDistanceSqrd)
            {
                minDistanceSqrd = distanceSqrd;
                iClosestPlume = it;
            }
        }

        iCurrentPlume = iClosestPlume;
        iClosestPlume = plumes.end();
        vCurrentPlume = iCurrentPlume->mIndex;
    }

    border.push_back(plumes[0].mIndex);


    sf::VertexArray array(sf::LinesStrip, border.size());
    sf::Vertex v;
    v.color = sf::Color::White;

    for(int i = 0; i < border.size(); i++)
    {
        v.position.x = border[i].x;
        v.position.y = border[i].y;

        array[i] = v;
    }

    mBorders = array;
    */
}


void Lithosphere::update(float years)
{
    //for(PlatePtr& plate : mPlates)
    //    plate->update(years);

    mPlates[3]->update(years);
    //initializeDrawMap();
    handlePlateMovement();
}

void Lithosphere::draw(sf::RenderWindow& window) const
{


    window.draw(mDrawMap);
    //for(const PlatePtr& plate : mPlates)
    //    plate->draw(window);
    mPlates[3]->draw(window);
    drawPlumes(window);
    window.draw(mBorders);
    //mPlates[1]->draw(window);
}

void Lithosphere::handlePlateMovement()
{
    /*
    // Note that the order of the subvectors are the same as their respective parent plates in mPlates.
    std::vector<std::vector<BorderCrust*>> newIndices;
    std::vector<std::vector<sf::Vector2i>> oldIndices;

    // Get all indices that the plates' crusts have moved onto (newIndices) and from (oldIndices).
    for(PlatePtr& plate : mPlates)
    {
        newIndices.push_back(plate->getNewCrustIndices());
        oldIndices.push_back(plate->getOldCrustIndices());
    }
*/
    ///////////////////////////////////////
    // Update occupancy map.

    // Crusts that move onto an index not occupied by its plate.
    // I.e. the frontal crusts of the plate.
    // Incrementing the occupancy map at the index it has moved to.
    for(PlatePtr& plate : mPlates)
    {
        const std::vector<BorderCrust*>& newCrusts = plate->getNewCrustIndices();
        for(int i = 0; newCrusts[i] != nullptr; i++)
        {
            BorderCrust* pCrust = newCrusts[i];
            sf::Vector2i index = pCrust->getIndex();
            mIndexOccupancyMap[index.x][index.y]++;

            if(mIndexOccupancyMap[index.x][index.y] > 1)
                solveCollision(pCrust, i);
        }

        const std::vector<sf::Vector2i>& oldCrusts = plate->getOldCrustIndices();
        for(sf::Vector2i index : oldCrusts)
        {
            mIndexOccupancyMap[index.x][index.y]--;
            if(mIndexOccupancyMap[index.x][index.y] < 1)
                populateEmptyIndex(index);
        }

    }
/*
    for(auto plateIndices : newIndices)
        for(int i = 0; plateIndices[i] != nullptr; i++)
        {
            sf::Vector2i index = plateIndices[i]->getIndex();
            mIndexOccupancyMap[index.x][index.y]++;
        }


    // Back crusts.
    // Decrementing the occupancy map at the index it has moved from.
    for(auto plateIndices : oldIndices)
        for(sf::Vector2i index : plateIndices)
            mIndexOccupancyMap[index.x][index.y]--;
    ///////////////////////////////////////


    // If several crusts occupy the same index, investigate potential collision.
    for(int i = 0; i < newIndices.size(); i++)
        for(int j = 0; newIndices[i][j] != nullptr; j++)
        {
            sf::Vector2i index = newIndices[i][j]->getIndex();
            if(mIndexOccupancyMap[index.x][index.y] > 1)
                solveCollision(newIndices[i][j], i);
        }


    // If no crusts occupy index, populate it with fresh, delicious crust.
    for(int i = 0; i < oldIndices.size(); i++)
        for(sf::Vector2i index : oldIndices[i])
            if(mIndexOccupancyMap[index.x][index.y] < 1)
                populateEmptyIndex(index);*/
}



void Lithosphere::solveCollision(BorderCrust* pCrust, int plateIndex)
{
    //mPlates[plateIndex]->offsetHeight(pCrust->getSourceCrust().getIndex(), 1);
    sf::Vector2i index = pCrust->getOriginalIndex();
    Crust& sourceCrust = pCrust->getSourceCrust();
    sourceCrust.offsetHeight(100);

    int iDrawMap = index.x * mSize.y + index.y;
    mDrawMap[iDrawMap].color.g = sourceCrust.getHeight() > 255 ? 255 : sourceCrust.getHeight();
    mIndexOccupancyMap[index.x][index.y]--;

/*    int crustCount = mIndexOccupancyMap[index.x][index.y];
    std::vector<unsigned int> plates;

    int found = 0;
    for(int i = 0; i < mPlates.size(); i++)
    {

        int plateCrustCount = mPlates[i]->getCrustCount(index);

        if(plateCrustCount > 0)
        {
            crustCount += plateCrustCount;
            plates.push_back(i);
        }

        if(found == crustCount)
            break;
    }

    if(plates.size() > 1)
    {
        mPlates[plates[1]]->offsetHeight(index, 1);
        mIndexOccupancyMap[index.x][index.y] = 1;
    }*/
}

void Lithosphere::initializeDrawMap()
{
    mDrawMap.clear();
    mDrawMap.resize(mSize.x * mSize.y);

    sf::Vertex vertex;
    vertex.color = sf::Color(0, 0, 0);

    unsigned int mapIndex = 0;
    for(int x = 0; x < mSize.x; x++)
    {
        for(int y = 0; y < mSize.y; y++)
        {
            vertex.position.x = x;
            vertex.position.y = y;
            vertex.color.g = mHeightmap[x][y].getHeight() > 255 ? 255 : mHeightmap[x][y].getHeight();
            mDrawMap[mapIndex] = vertex;
            mapIndex++;
        }
    }
}


void Lithosphere::populateEmptyIndex(sf::Vector2i index)
{

}

void Lithosphere::drawPlumes(sf::RenderWindow& window) const
{
    sf::VertexArray plumes(sf::Quads, mPlumes.size() * 4);

    sf::Vertex v;
    v.color = sf::Color::Red;

    int halfRadius;
    int index = 0;
    for(const Plume& plume : mPlumes)
    {
        halfRadius = plume.mRadius / 2;

        v.position.x = plume.mIndex.x - halfRadius;
        v.position.y = plume.mIndex.y - halfRadius;
        plumes[index] = v;
        index++;

        v.position.x = plume.mIndex.x + halfRadius;
        v.position.y = plume.mIndex.y - halfRadius;
        plumes[index] = v;
        index++;

        v.position.x = plume.mIndex.x + halfRadius;
        v.position.y = plume.mIndex.y + halfRadius;
        plumes[index] = v;
        index++;

        v.position.x = plume.mIndex.x - halfRadius;
        v.position.y = plume.mIndex.y + halfRadius;

        plumes[index] = v;
        index++;
    }

    window.draw(plumes);
}

const std::vector<Lithosphere::PlatePtr>& Lithosphere::getPlates() const
{
    return mPlates;
}
