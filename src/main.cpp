//    ************************************************************
//    *                        Followers                         *
//    *                       August 2015                        *
//    ************************************************************
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <string>
#include <vector>

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
  int numberofleaders = 8;
  bool followers = false;
  bool tracersONorOFF = false;
  bool slowdown = false;
  
  // What type of pattern would you like to generate?

  // NOTES:
  // Flower/spiral/fibonacci patterns if number = 10 and Patterns[3]
  // Patterns[6] looks nice when number = 4, 8, 12, 16, 360
  // Patterns[7] looks nice when number = 4, 5, 8, 12,36
  // Patterns[8] - number = 360 gives awesome simulation but computationally slow
  std::vector<std::string> Patterns;
  Patterns.push_back("spiral");               // 0
  Patterns.push_back("random");               // 1
  Patterns.push_back("circle");               // 2
  Patterns.push_back("flower_spiral");        // 3
  Patterns.push_back("flower");               // 4
  Patterns.push_back("double circle");        // 5
  Patterns.push_back("puzzle piece center");  // 6 
  Patterns.push_back("bulls eye");            // 7
  Patterns.push_back("crazy folds");          // 8

  // Box Patterns under the influence of gravity with different initial conditions
  // note - numberofleaders is irrelevant here as it gets overridden
  // note - tracers should be OFF
 
  Patterns.push_back("boxC");                 // 9
  Patterns.push_back("boxC folds");           // 10
  Patterns.push_back("boxTL");                // 11
  Patterns.push_back("boxTL folds");          // 12


  // Initializations
  Leader leader(window.getSize().x, window.getSize().y, numberofleaders, Patterns[10], followers, tracersONorOFF, slowdown);

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
    if( leader.getFollowerBool() ) {
      leader.follow();
    }
    sf::Time elapsed = clock.restart();
    if( leader.getTracerBool() ) {
      leader.tracersON(clicker);
      leader.dissolve(elapsed);
    }    
    // DRAWINGS
    window.draw(leader);
    window.display();  

    //t += dt;
    clicker++;
  }
  return 0;
}
