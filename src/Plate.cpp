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
#include <Plate.hpp>
#include <Utility.hpp>
////////////////////////////////////////////////

////////////////////////////////////////////////
// C++ Standard Library
//////////////////////
// DEBUG
#include <iostream>
#include <cassert>
#include <functional>
//////////////////////
////////////////////////////////////////////////


////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include <SFML/Graphics/RenderWindow.hpp>
////////////////////////////////////////////////

Plate::Plate(std::vector<std::vector<Crust>>& heightmap, sf::Vector2u worldSize, std::list<BorderCrust> border)
: mHeightmap(heightmap)
, mBorder(border)
, mRotationalVelocity(0)
{
    mWorldSize.x = worldSize.x;
    mWorldSize.y = worldSize.y;

    mWorldSizef.x = mWorldSize.x;
    mWorldSizef.y = mWorldSize.y;


    sf::Vector2i origin = mBorder.begin()->getIndex();
    mRotationalCenter = sf::Vector2f(origin.x, origin.y);

    for(BorderCrust& crust : mBorder)
    {
        sf::Vector2i index = crust.getIndex();
        sf::Vector2f vRadius(index.x - mRotationalCenter.x, index.y - mRotationalCenter.y);
        crust.offsetRadiusVector(vRadius);
    }

    mOldCrustIndices.resize(mBorder.size());
    mNewCrustIndices.resize(mBorder.size());
    initializeDrawMap();
}


void Plate::update(float years)
{
    sf::Vector2f distance = mVelocity * years;
    float rotation = mRotationalVelocity * years;

    move(distance);
    rotate(rotation);

    moveBorder(distance, rotation);
    updateBorder();
}

void Plate::draw(sf::RenderWindow& window)
{
    sf::VertexArray rotationalCenter(sf::Quads, 4);
    rotationalCenter[0].position = mRotationalCenter - sf::Vector2f(2, 2);
    rotationalCenter[0].color = sf::Color::Red;

    rotationalCenter[1].position = mRotationalCenter + sf::Vector2f(2, -2);
    rotationalCenter[1].color = sf::Color::Red;

    rotationalCenter[2].position = mRotationalCenter + sf::Vector2f(2, 2);
    rotationalCenter[2].color = sf::Color::Red;

    rotationalCenter[3].position = mRotationalCenter + sf::Vector2f(-2, 2);
    rotationalCenter[3].color = sf::Color::Red;

    window.draw(rotationalCenter);
    drawBorder(window);
}

void Plate::fitIndexToWorldmap(sf::Vector2i& index)
{
    if(index.x < 0)
        index.x = (mWorldSize.x - 1) - (-index.x % mWorldSize.x);
    else if(index.x > mWorldSize.x - 1)
        index.x = index.x % mWorldSize.x;

    if(index.y < 0)
        index.y = (mWorldSize.y - 1) - (-index.y % mWorldSize.y);
    else if(index.y > mWorldSize.y - 1)
        index.y = index.y % mWorldSize.y;
}

/*
void Plate::initializeDrawMap()
{

    mDrawMap.clear();
    mDrawMap.resize(getCrustCount());

    sf::Vertex vertex;
    vertex.color = sf::Color(0, 0, 0);

    unsigned int mapIndex = 0;
    for(const std::vector<Crust>& iX : mHeightmap)
    {
        for(const Crust& iY : iX)
        {
            sf::Vector2f pos = sf::Vector2f(iY.getIndex().x, iY.getIndex().y);
            pos = mTransform.transformPoint(pos);
            pos += mTranslation;
            fitPositionToWorldmap(pos);

            vertex.position = pos;
            vertex.color.g = iY.getHeight() > 255 ? 255 : iY.getHeight();
            mDrawMap[mapIndex] = vertex;
            mapIndex++;
        }
    }
}
*/
void Plate::fitPositionToWorldmap(sf::Vector2f& pos)
{
    if(pos.x >= mWorldSizef.x)
        while(pos.x >= mWorldSizef.x)
            pos.x -= mWorldSizef.x;
    else if(pos.x < 0.f)
        while(pos.x < 0.f)
            pos.x += mWorldSizef.x;

    if(pos.y >= mWorldSizef.y)
        while(pos.y >= mWorldSizef.y)
            pos.y -= mWorldSizef.y;
    else if(pos.y < 0.f)
        while(pos.y < 0.f)
            pos.y += mWorldSizef.y;
}

void Plate::move(sf::Vector2f distance)
{
    mRotationalCenter += distance;
    mTranslation += distance;
    loopCoords(mRotationalCenter);
    loopTranslation(); // Shave mTranslation down when the plate has moved a whole "turn" around the world.
}

void Plate::loopCoords(sf::Vector2f& coords)
{
    while(coords.x >= mWorldSizef.x)
        coords.x -= mWorldSizef.x;
    while(coords.x < 0.f)
        coords.x += mWorldSizef.x;

    while(coords.y >= mWorldSizef.y)
        coords.y -= mWorldSizef.y;
    while(coords.y < 0.f)
        coords.y += mWorldSizef.y;
}

void Plate::loopTranslation()
{
    if(mTranslation.x > mWorldSize.x)
        while(mTranslation.x > mWorldSize.x)
            mTranslation.x -= mWorldSize.x;
    else if(mTranslation.x < -mWorldSize.x)
        while(mTranslation.x < -mWorldSize.x)
            mTranslation.x += mWorldSize.x;

    if(mTranslation.y > mWorldSize.y)
        while(mTranslation.y > mWorldSize.y)
            mTranslation.y -= mWorldSize.y;
    else if(mTranslation.y < -mWorldSize.y)
        while(mTranslation.y < -mWorldSize.y)
            mTranslation.y += mWorldSize.y;
}

void Plate::updateBorder()
{
    // Update border crusts.
    mNewCrustIndices[0] = nullptr;
    mOldCrustIndices.clear();

    for(BorderCrust& crust : mBorder)
        crust.update();

    std::list<BorderCrust>::iterator iNext, iPrevious;
    iNext = mBorder.begin();
    iNext++;
    iPrevious = mBorder.end();
    iPrevious--;
    unsigned int iNewCrust = 0;
    for(auto iCrust = mBorder.begin(); iCrust != mBorder.end(); iCrust++)
    {
        sf::Vector2i indexChange;
        sf::Vector2i index = iCrust->getIndex();
        sf::Vector2f indexf(index.x, index.y);
        sf::Vector2f positionf = mRotationalCenter + iCrust->getRadiusVector();
        if(positionf.x > indexf.x)
            indexChange.x = 1;
        else if(positionf.x < indexf.x)
            indexChange.x = -1;
        else
            indexChange.x = 0;

        if(positionf.y > indexf.y)
            indexChange.y = 1;
        else if(positionf.y < indexf.y)
            indexChange.y = -1;
        else
            indexChange.y = 0;



        if(indexChange.x != 0 && indexChange.y != 0)
        {
            index += indexChange;
            fitIndexToWorldmap(index);
            iCrust->setIndex(index.x, index.y);

            sf::Vector2f radiusVector = iCrust->getRadiusVector();//iCrust->getPosition();
            sf::Vector2f dNext = iNext->getRadiusVector() - radiusVector;
            sf::Vector2f dPrev = iPrevious->getRadiusVector() - radiusVector;
            //sf::Vector2f dNext = iNext->getPosition() - pos;
            //sf::Vector2f dPrev = iPrevious->getPosition() - pos;



            bool isFrontCrust = false;
            if(indexChange.x > 0)
            {
                if(dNext.y > 0 && dPrev.y < 0)
                    isFrontCrust = true;
            }
            else if(indexChange.x < 0)
            {
                if(dNext.y < 0 && dPrev.y > 0)
                    isFrontCrust = true;
            }

            if(indexChange.y > 0)
            {
                if(dNext.x < 0 && dPrev.x > 0)
                    isFrontCrust = true;
            }
            else if(indexChange.y > 0)
            {
                if(dNext.x > 0 && dPrev.x < 0)
                    isFrontCrust = true;
            }

            if(isFrontCrust)
            {
                mNewCrustIndices[iNewCrust] = (&(*iCrust));
                iNewCrust++;
            }
        }



        iNext++;
        iPrevious++;

        if(iNext == mBorder.end())
            iNext = mBorder.begin();

        if(iPrevious == mBorder.end())
            iPrevious = mBorder.begin();
    }

    mNewCrustIndices[iNewCrust] = nullptr;
}


void Plate::setVelocity(float x, float y)
{
    mVelocity = sf::Vector2f(x, y);
}

sf::Vector2f Plate::getVelocity() const
{
    return mVelocity;
}

const std::vector<sf::Vector2i>& Plate::getOldCrustIndices() const
{
    return mOldCrustIndices;
}

const std::vector<BorderCrust*>& Plate::getNewCrustIndices() const
{
    return mNewCrustIndices;
}

void Plate::setRotationalVelocity(float degrees)
{
    mRotationalVelocity = degrees;
}

void Plate::rotate(float degrees)
{
    mRotation += degrees;

    if(mRotation > 360.f)
        mRotation -= 360.f;

    mTransform.rotate(degrees, mRotationalCenter);
}

void Plate::moveBorder(sf::Vector2f distance, float degrees)
{
    // Now compute the rotation modifiers
    // for the current rotation change
    float radians = degreeToRadian(degrees);
    double s = std::sin(radians);
    double c = std::cos(radians);
    unsigned int iDrawMap = 0;
    for(BorderCrust& crust : mBorder)
    {
        // Compute how far the crust will move when
        // rotated around the rotational center.
        sf::Vector2f vRadius = crust.getRadiusVector();
        sf::Vector2f vRotation;
        vRotation.x = vRadius.x * c - vRadius.y * s - vRadius.x;
        vRotation.y = vRadius.x * s + vRadius.y * c - vRadius.y;


        crust.offsetRadiusVector(vRotation);
        //crust.move(distance + vRotation);

        mDrawMap[iDrawMap].position = mRotationalCenter + crust.getRadiusVector();
        iDrawMap++;
    }
}

void Plate::initializeDrawMap()
{
    mDrawMap = sf::VertexArray(sf::Points, mBorder.size());

    sf::Vertex vertex;
    vertex.color = sf::Color(255, 0, 0);
    unsigned int mapIndex = 0;
    for(const BorderCrust& crust : mBorder)
    {
        vertex.position = mRotationalCenter + crust.getRadiusVector();
        mDrawMap[mapIndex] = vertex;
        mapIndex++;
    }
}

// Draw the border red!
void Plate::drawBorder(sf::RenderWindow& window) const
{
    window.draw(mDrawMap);
}

unsigned int Plate::getBorderCrustCount() const
{
    return mBorder.size();
}
