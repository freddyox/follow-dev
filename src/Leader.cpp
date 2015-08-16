#include "../include/Leader.hh"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <iostream>

Leader::Leader(float x, float y, int N) {
  // Initialize some constants
  displayx = x;
  displayy = y;
  numberofleaders = N;
  conv = 3.141592 / 180.0; 
  centerMass = 100.0;
  center = sf::Vector2f(displayx/2.0, displayy/2.0);
  radius = 2.0;
  timer = 0;
  density = 3.3;

  // Initialize leader properties
  circles.setRadius(radius);
  circles.setFillColor(sf::Color::Cyan);
  sf::FloatRect origin = circles.getLocalBounds();
  circles.setOrigin( 0.5*origin.width, 0.5*origin.height );

  // Initialize
  std::vector<Particle> leadervec( numberofleaders );

  // Initialize rand();
  srand(time(NULL));
  
  for( int i=0; i<numberofleaders; i++ ){
    // Randomly place the leaders with random velocity vectors:
    float tempX = rand() % ( int (displayx) - 2*int(radius) ) + int(radius);
    float tempY = rand() % ( int (displayy) - 2*int(radius) ) + int(radius);
    circles.setPosition( tempX, tempY );
    float tempAngle = rand() % 360;
    
    float vx = (rand() % 90) / 10.0;
    float vy = (rand() % 90) / 10.0;
    sf::Vector2f velocity_knot( -vx*sin( conv*tempAngle ), vy*cos( conv*tempAngle) );
    float m = rand() % 10 + 3;
    ParticleAdd(circles, velocity_knot, tempAngle, m);
  }

  for( int i=0; i<numberofleaders; i++ ){
    // Randomly place the leaders with random velocity vectors:
    float tempX = rand() % ( int (displayx) - 2*int(radius) ) + int(radius);
    float tempY = rand() % ( int (displayy) - 2*int(radius) ) + int(radius);
    circles.setPosition( tempX, tempY );
    circles.setFillColor( sf::Color::Blue );
    float tempAngle = rand() % 360;
    
    float vx = (rand() % 90) / 10.0;
    float vy = (rand() % 90) / 10.0;
    sf::Vector2f velocity_knot( -vx*sin( conv*tempAngle ), vy*cos( conv*tempAngle) );
    float m = rand() % 10 + 3;
    FollowerAdd(circles, velocity_knot, tempAngle);
  }
}

void Leader::draw(sf::RenderTarget& target, sf::RenderStates) const {
  std::vector<Particle>::const_iterator cit;
  for( cit = leadervec.begin(); cit != leadervec.end(); cit++ ){
    target.draw( (*cit).leader );
  }

  std::vector<Follower>::const_iterator cit1;
  for( cit1 = followervec.begin(); cit1 != followervec.end(); cit1++ ){
    target.draw( (*cit1).follower );
  }
}

void Leader::update() {
  for( leaderit = leadervec.begin(); leaderit != leadervec.end(); leaderit++ ) {
    sf::Vector2f temp = (*leaderit).leader.getPosition();

    // Wall collisions
    if( temp.x + 1.1*radius >= displayx) {
      (*leaderit).velocity.x = -(*leaderit).velocity.x;
    }
    if( temp.x - 1.1*radius <= 0 ) {
      (*leaderit).velocity.x = -(*leaderit).velocity.x;
    }
    if( temp.y + 1.1*radius >= displayy ) {
      (*leaderit).velocity.y = -(*leaderit).velocity.y;
    }
    if( temp.y - 1.1*radius <= 0) {
      (*leaderit).velocity.y = -(*leaderit).velocity.y;
    } 
    // Now move the particle   
    (*leaderit).leader.move( (*leaderit).velocity.x, (*leaderit).velocity.y );
  }

  for( followerit = followervec.begin(); followerit != followervec.end(); followerit++ ) {
    (*followerit).follower.move( (*followerit).velocity.x, (*followerit).velocity.y );
  }
}

void Leader::gravity() {
  // Assume that there is a massive particle in the center
  for( leaderit = leadervec.begin(); leaderit != leadervec.end(); leaderit++ ) {
    sf::Vector2f r_vec = center - (*leaderit).leader.getPosition();
    float r = sqrt( pow(r_vec.x,2) + pow(r_vec.y,2) );
    sf::Vector2f r_hat = r_vec / r;
    float temp = (centerMass*(*leaderit).mass) / r*r;
    sf::Vector2f force = temp*r_hat;
    
    (*leaderit).velocity.x += (force.x) / ((*leaderit).mass*1000); 
    (*leaderit).velocity.y += (force.y) / ((*leaderit).mass*1000);
    
    // sf::Vector2f tempRed = (*leaderit).velocity;
    // float redValue = 0.5*(tempRed.x*tempRed.x + tempRed.y*tempRed.y);
    // float norm = 255*(1 - 1/redValue);
    // if( norm>=0 ) {
    //   (*leaderit).leader.setFillColor(sf::Color(int(norm),1-int(norm),0));
    // }
  }
}
void Leader::spiral(int clicker){
  for( leaderit = leadervec.begin(); leaderit != leadervec.end(); leaderit++ ) {
    (*leaderit).velocity.x *= .997; 
    (*leaderit).velocity.y *= .997; 
  }
}

void Leader::ParticleAdd(sf::CircleShape lead, sf::Vector2f vel, float ang, float m) {
  Particle newpart;
  newpart.leader = lead;
  newpart.velocity = vel;
  newpart.angle = ang;
  newpart.mass = m;
  leadervec.push_back( newpart );
}

void Leader::follow(){
  for( int i=0; i<leadervec.size(); i++ ){
    sf::Vector2f tempVel = leadervec[i].velocity;
    sf::Vector2f tempR = leadervec[i].leader.getPosition() - followervec[i].follower.getPosition();
    
    float Vmag = sqrt( tempVel.x*tempVel.x + tempVel.y*tempVel.y );
    float Rmag = sqrt( tempR.x*tempR.x + tempR.y*tempR.y );
    
    sf::Vector2f followerV = Vmag * (tempR / Rmag );
    followervec[i].velocity = followerV;    
  }
}

void Leader::FollowerAdd(sf::CircleShape follow, sf::Vector2f vel, float ang) {
  Follower newfoll;
  newfoll.follower = follow;
  newfoll.velocity = vel;
  newfoll.angle = ang;
  followervec.push_back( newfoll );
}
