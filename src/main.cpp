//    ************************************************************
//    *                        Followers                         *
//    *                       August 2015                        *
//    ************************************************************
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>

#include "../include/Leader.hh"

const float gDisplayx = 1600;
const float gDisplayy = 1000;
int main() {
  //GAME SETUP
  sf::RenderWindow window(sf::VideoMode(gDisplayx,gDisplayy), "Follower");
  window.setFramerateLimit(60);

  // Time
  int clicker=0;
  sf::Clock clock;
  float t = 0.0;
  float dt = 1.0/60.0;

  //////////////////////////////////////////////////////
  //                   Parameters                     //
  //////////////////////////////////////////////////////
  int numberofleaders = 10;
  
  // Initializations
  Leader leader(window.getSize().x, window.getSize().y, numberofleaders);

  while( window.isOpen() ) {

    sf::Event event;
    while( window.pollEvent(event) ) {
      if( event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ) {
	window.close();
      }
    }

    window.clear();

    // UPDATING
    leader.update();
    leader.gravity();
    leader.spiral(clicker);
    //leader.follow();
    sf::Time elapsed = clock.restart();
    leader.tracersON(clicker);
    leader.dissolve(elapsed);
    
    // DRAWINGS
    window.draw(leader);
    window.display();  

    //t += dt;
    clicker++;
  }
  return 0;
}
