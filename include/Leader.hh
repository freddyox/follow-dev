#ifndef LEADER_HH
#define LEADER_HH

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>

class Leader : public sf::Drawable, public sf::Transformable {

private:
  float displayx, displayy;
  float radius, vel, conv;
  float G, centerMass, density;
  sf::Vector2f center;
  int numberofleaders,timer;
  bool putinFollowers, tracersONorOFF;
  std::string pattern;

  sf::CircleShape circles;

  // Leaders
  struct Particle {
    sf::CircleShape leader;
    sf::Vector2f velocity;
    float angle;
    float mass;
  };

  std::vector<Particle> leadervec;
  std::vector<Particle>::iterator leaderit;

  // Followers
  struct Follower {
    sf::CircleShape follower;
    sf::Vector2f velocity;
    float angle;
  };
  std::vector<Follower> followervec;
  std::vector<Follower>::iterator followerit;

  // Tracers
  struct Tracers{
    sf::CircleShape tracer;
    sf::Time lifetime;
  };
  sf::CircleShape tracertemplate;
  std::vector<Tracers> tracevec;
  std::vector<Tracers>::iterator traceit;
  sf::Time m_lifetime;
  float R_knot;

  struct TracersF{
    sf::CircleShape tracerF;
    sf::Time lifetimeF;
  };
  sf::CircleShape tracertemplateF;
  std::vector<TracersF> tracevecF;
  std::vector<TracersF>::iterator traceitF;

public:
  Leader(float, float, int, std::string, bool, bool);
  ~Leader() {};
  void draw(sf::RenderTarget&, sf::RenderStates) const;
  void ParticleAdd(sf::CircleShape, sf::Vector2f, float, float);
  void gravity();
  void update();
  void spiral(int);

  void FollowerAdd(sf::CircleShape, sf::Vector2f, float);
  void follow();

  void tracersON(int);
  void TracerAdd(sf::CircleShape, sf::Time);
  void TracerAddF(sf::CircleShape, sf::Time);
  void dissolve(sf::Time);

  bool getFollowerBool() { return putinFollowers; }
  bool getTracerBool() { return tracersONorOFF; }
};
#endif
