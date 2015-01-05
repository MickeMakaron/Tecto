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
#include <Lithosphere.hpp>
////////////////////////////////////////////////

////////////////////////////////////////////////
// C++ Standard Library
#include <utility>

//////////////////////
// DEBUG
#include <iostream>
//////////////////////
////////////////////////////////////////////////

////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
////////////////////////////////////////////////


int main()
{
    unsigned int sizeX, sizeY;
    sizeX = sizeY = 5000;

    sf::RenderWindow window(sf::VideoMode(sizeX, sizeY), "VODKA", sf::Style::Default);
    const float YEARS_PER_TICK = 10.f;

    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(false);
    window.setKeyRepeatEnabled(false);

    //Lithosphere lithosphere(window.getSize().x, window.getSize().y);
    Lithosphere lithosphere(sizeX, sizeY);
    sf::Clock clock;
    unsigned int ticks = 0;
    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        lithosphere.update(YEARS_PER_TICK);
        ticks++;


       // if(ticks > 3000)
        {
            window.clear();
            lithosphere.draw(window);
            window.display();

            ticks = 0;

            std::cout << clock.restart().asMilliseconds() << std::endl;
        }

    }
}











