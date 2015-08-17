#include "../include/Leader.hh"
#include <SFML/Window.hpp>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <iostream>

Leader::Leader(float x, float y, int N, std::string whichPattern, bool yestofollowers, bool tracersYorN) {
  // Some constants
  displayx = x;
  displayy = y;
  numberofleaders = N;
  conv = 3.141592 / 180.0; 
  centerMass = 500.0;
  center = sf::Vector2f(displayx/2.0, displayy/2.0);
  radius = 2.0;
  pattern = whichPattern;
  putinFollowers = yestofollowers;
  tracersONorOFF = tracersYorN;

  // Initialize leader properties
  circles.setRadius(radius);
  circles.setFillColor(sf::Color::Cyan);
  sf::FloatRect origin = circles.getLocalBounds();
  circles.setOrigin( 0.5*origin.width, 0.5*origin.height );
  
  // Tracers
  R_knot = 500;
  if( putinFollowers && tracersONorOFF ) {
    m_lifetime = sf::Time(sf::seconds(1));
  }
  else{
    m_lifetime = sf::Time(sf::seconds(10));
  }
  tracertemplate.setRadius(radius);
  tracertemplate.setFillColor(sf::Color::Cyan);
  origin = tracertemplate.getLocalBounds();
  tracertemplate.setOrigin( 0.5*origin.width, 0.5*origin.height );

  tracertemplateF.setRadius(radius);
  tracertemplateF.setFillColor(sf::Color::Blue);
  origin = tracertemplateF.getLocalBounds();
  tracertemplateF.setOrigin( 0.5*origin.width, 0.5*origin.height );

  srand(time(NULL));
  
  // This routine handles starting locations, velocities, and angles
  // for a variety of setups.
  for( int i=0; i<numberofleaders; i++ ){
    if(    pattern == "spiral"        || pattern == "circle" 
	|| pattern == "flower"        || pattern == "flower_spiral" 
	|| pattern == "double circle" || pattern == "puzzle piece center" 
	|| pattern == "bulls eye" ){
      float tempAngle = 360 / numberofleaders;
    
      sf::Vector2f tempPosition( -R_knot*sin( conv*tempAngle*i ), R_knot*cos( conv*tempAngle*i ) );
      sf::Vector2f place = center + tempPosition;
      circles.setPosition(place);    

      sf::Vector2f centertoleader = place - center;
      float mag = sqrt(pow(centertoleader.x,2)+pow(centertoleader.y,2));
      sf::Vector2f center_hat = centertoleader / mag;

      // Counter Clockwise
      sf::Vector2f vel_hat( center_hat.y, -center_hat.x );
      // Clockwise
      sf::Vector2f vel_hat1( -center_hat.y, center_hat.x );
      float vx,vy;

      if( pattern == "puzzle piece center" ) {
	centerMass = 10000;
      }      
      if( pattern == "flower" || pattern == "flower_spiral" || pattern == "bulls eye" ) { 
	if( pattern == "bulls eye" ) {
	  centerMass = 10000;
	}
	vx = sqrt(centerMass / R_knot );
	vy = sqrt(centerMass / R_knot );
      }
      
      else {
	// note: 16 and 16 basically give  a perfect circle
	vx = 10;
	vy = 10;
      }
      sf::Vector2f velocity_knot( vel_hat.x*vx, vel_hat.y*vy );
      float m = 10;
      ParticleAdd(circles, velocity_knot, tempAngle, m);
      
      if( pattern == "double circle" ) {
	float tempR = R_knot / 20;
	tempPosition = sf::Vector2f( -tempR*sin( conv*tempAngle*i ), tempR*cos( conv*tempAngle*i ) );
	place = center + tempPosition;
	circles.setPosition(place);  
	centertoleader = place - center;
	mag = sqrt(pow(centertoleader.x,2)+pow(centertoleader.y,2));
	center_hat = centertoleader / mag;
	vel_hat = sf::Vector2f( center_hat.y, -center_hat.x );
	
	vx = 10;
	vy = 10;
	
	velocity_knot = sf::Vector2f( vel_hat.x*vx, vel_hat.y*vy );
       	ParticleAdd(circles, velocity_knot, tempAngle, m);
      }
    }
    
    if( pattern == "random" ){
      // Randomly place the leaders with random velocity vectors:
      float tempX = rand() % ( int (displayx) - 2*int(radius) ) + int(radius);
      float tempY = rand() % ( int (displayy) - 2*int(radius) ) + int(radius);
      float tempAngle = rand() % 360;;  
      circles.setPosition( tempX, tempY );
      float vx = (rand() % 40) / 10.0;
      float vy = (rand() % 40) / 10.0;
      sf::Vector2f velocity_knot( -vx*sin( conv*tempAngle ), vy*cos( conv*tempAngle) );   
      float m = 10;
      ParticleAdd(circles, velocity_knot, tempAngle, m);
    }
  }
  if( putinFollowers ) {
    for( int i=0; i<numberofleaders; i++ ){
      // Randomly place the followers with random velocity vectors:
      float tempX = rand() % ( int (displayx) - 2*int(radius) ) + int(radius);
      float tempY = rand() % ( int (displayy) - 2*int(radius) ) + int(radius);
      circles.setPosition( tempX, tempY );
      circles.setFillColor( sf::Color::Blue );
      float tempAngle = rand() % 360;
    
      float vx = (rand() % 90) / 10.0;
      float vy = (rand() % 90) / 10.0;
      sf::Vector2f velocity_knot( -vx*sin( conv*tempAngle ), vy*cos( conv*tempAngle) );
      FollowerAdd(circles, velocity_knot, tempAngle);
    }
  }
}
////////////////////////////////////
//                                //
//            LEADERS             //
//                                //
////////////////////////////////////
void Leader::update() {
  for( leaderit = leadervec.begin(); leaderit != leadervec.end(); leaderit++ ) {
    sf::Vector2f temp = (*leaderit).leader.getPosition();

    // Wall collisions
    // if( temp.x + 1.1*radius >= displayx) {
    //   (*leaderit).velocity.x = -(*leaderit).velocity.x;
    // }
    // if( temp.x - 1.1*radius <= 0 ) {
    //   (*leaderit).velocity.x = -(*leaderit).velocity.x;
    // }
    // if( temp.y + 1.1*radius >= displayy ) {
    //   (*leaderit).velocity.y = -(*leaderit).velocity.y;
    // }
    // if( temp.y - 1.1*radius <= 0) {
    //   (*leaderit).velocity.y = -(*leaderit).velocity.y;
    // } 
    // Now move the Leaders   
    (*leaderit).leader.move( (*leaderit).velocity.x, (*leaderit).velocity.y );
  }

  // Move the followers
  if( putinFollowers ) {
    for( followerit = followervec.begin(); followerit != followervec.end(); followerit++ ) {
      (*followerit).follower.move( (*followerit).velocity.x, (*followerit).velocity.y );
    }
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
  }
}
void Leader::spiral(int clicker){
  if( pattern == "spiral" || pattern == "flower_spiral" ) {
    for( leaderit = leadervec.begin(); leaderit != leadervec.end(); leaderit++ ) {
      (*leaderit).velocity.x *= .997; 
      (*leaderit).velocity.y *= .997; 
    }
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
////////////////////////////////////
//                                //
//          FOLLOWERS             //
//                                //
////////////////////////////////////
void Leader::follow(){
  // Each follower chooses a leader based on the index below.  
  for( int i=0; i<leadervec.size(); i++ ){
    // Vector pointing from Follower to Leader - 
    // move the follower towards the leader
    sf::Vector2f tempVel = followervec[i].velocity;
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
////////////////////////////////////
//                                //
//            TRACERS             //
//                                //
////////////////////////////////////
void Leader::tracersON(int click) {
  for( leaderit = leadervec.begin(); leaderit != leadervec.end(); leaderit++ ) {
    sf::Vector2f tempPosition = (*leaderit).leader.getPosition();
    tracertemplate.setPosition( tempPosition );
    TracerAdd( tracertemplate, m_lifetime );
  }
  if( putinFollowers && tracersONorOFF ) {
    for( followerit = followervec.begin(); followerit != followervec.end(); followerit++ ) {
      sf::Vector2f tempPosition = (*followerit).follower.getPosition();
      tracertemplateF.setPosition( tempPosition );
      TracerAddF( tracertemplateF, m_lifetime);
    }
  }
}
void Leader::dissolve(sf::Time elapsed) {
  for( traceit = tracevec.begin(); traceit != tracevec.end(); traceit++ ){
    (*traceit).lifetime -= elapsed;
    sf::Vector2f tempR = center - (*traceit).tracer.getPosition();
    float R = pow(tempR.x,2)+pow(tempR.y,2);
    float Rratio = 1 - R / (R_knot*R_knot);
    float Rratio2 = R/(R_knot*R_knot);
    float ratio = (*traceit).lifetime.asSeconds() / m_lifetime.asSeconds();

    if( putinFollowers && tracersONorOFF ) {
      (*traceit).tracer.setFillColor(sf::Color( 0,255,255, static_cast<sf::Uint8>(ratio * 255) ));
    }
    else {
      if( Rratio <= 0.96 ) {
	(*traceit).tracer.setFillColor(sf::Color( static_cast<sf::Uint8>(Rratio * 255) , 20, static_cast<sf::Uint8>(Rratio2 * 255), static_cast<sf::Uint8>(ratio * 255) ));
      }
      else if( Rratio > 0.96 && Rratio <= 0.99 ){ 
	(*traceit).tracer.setFillColor(sf::Color(50, static_cast<sf::Uint8>(Rratio * 80), static_cast<sf::Uint8>(Rratio2 * 200), static_cast<sf::Uint8>(ratio * 255) ));
      }
      else if( Rratio >0.99 ) {
	(*traceit).tracer.setFillColor(sf::Color(50, static_cast<sf::Uint8>(Rratio * 200), static_cast<sf::Uint8>(Rratio2 * 244), static_cast<sf::Uint8>(ratio * 255) ));
      }
    }
    
    // This part here is computationally slow.
    if( (*traceit).lifetime <= sf::Time::Zero ){
      int random = rand() % leadervec.size();
      (*traceit).tracer.setPosition( leadervec[random].leader.getPosition() );
    }
  }
  if( putinFollowers && tracersONorOFF ) {
    for( traceitF = tracevecF.begin(); traceitF != tracevecF.end(); traceitF++ ){
      (*traceitF).lifetimeF -= elapsed;
      sf::Vector2f tempR = center - (*traceitF).tracerF.getPosition();
      float R = pow(tempR.x,2)+pow(tempR.y,2);
      float Rratio = 1 - R / (R_knot*R_knot);
      float Rratio2 = R/(R_knot*R_knot);
      float ratio = (*traceitF).lifetimeF.asSeconds() / m_lifetime.asSeconds();
      (*traceitF).tracerF.setFillColor(sf::Color(0,0,255, static_cast<sf::Uint8>(ratio * 255) ));

      if( (*traceitF).lifetimeF <= sf::Time::Zero ){
	int random = rand() % leadervec.size();
	(*traceitF).tracerF.setPosition( leadervec[random].leader.getPosition() );
 }
 
    }
  }
}
void Leader::TracerAdd(sf::CircleShape trace, sf::Time life){
  Tracers newtrace;
  newtrace.tracer = trace;
  newtrace.lifetime = life;
  tracevec.push_back( newtrace );
}
void Leader::TracerAddF(sf::CircleShape trace, sf::Time life){
  TracersF newtrace;
  newtrace.tracerF = trace;
  newtrace.lifetimeF = life;
  tracevecF.push_back( newtrace );
}
////////////////////////////////////
//                                //
//              DRAW              //
//                                //
////////////////////////////////////
void Leader::draw(sf::RenderTarget& target, sf::RenderStates) const {
  std::vector<Particle>::const_iterator cit;
  for( cit = leadervec.begin(); cit != leadervec.end(); cit++ ){
    target.draw( (*cit).leader );
  }
  if( putinFollowers ) {
    std::vector<Follower>::const_iterator cit1;
    for( cit1 = followervec.begin(); cit1 != followervec.end(); cit1++ ){
      target.draw( (*cit1).follower );
    }
  }
  if( tracersONorOFF ) {
    std::vector<Tracers>::const_iterator cit2;
    for( cit2 = tracevec.begin(); cit2 != tracevec.end(); cit2++ ) {
      target.draw( (*cit2).tracer );
    }
    std::vector<TracersF>::const_iterator cit3;
    for( cit3 = tracevecF.begin(); cit3 != tracevecF.end(); cit3++ ) {
      target.draw( (*cit3).tracerF );
    }
  }
}

/*
  NOTES:

  1) If the spiral effect is removed, then a bored out cylinder shape is created

  2) Increasing the central mass moves the particles faster, as it should, but the tracers
     begin to get a disconnected feel since they are moving too fast
     
  3) The initial velocity has been chosen to be perpendicular to a vector running from 
     central mass to leader mass => they move in a trajectory that is a precessing ellipse


 */
