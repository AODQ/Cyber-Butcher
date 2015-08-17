#include "player.h"

Player::Player() {
  SetPosition(0,0);
  SetSprite("LD33-Wallpaper1.png");
  SetSize(10.f,10.f);
  InitPhysics();
  SetDensity(30.f);
  GetBody()->SetGravityScale(0.0f);
}

void Player::Update() {
  static const float _Force = 2500;

  if ( theInput.IsKeyDown(GLFW_KEY_A) ) {
    ApplyForce(Vector2(-_Force, 0), Vector2(0,0));
  }
  if ( theInput.IsKeyDown(GLFW_KEY_W) ) {
    ApplyForce(Vector2(0 ,_Force), Vector2(0,0));
  }
  if ( theInput.IsKeyDown(GLFW_KEY_S) ) {
    ApplyForce(Vector2( 0, -_Force), Vector2(0,0));
  }
  if ( theInput.IsKeyDown(GLFW_KEY_D) ) {
    ApplyForce(Vector2(_Force, 0), Vector2(0,0));
  }
  if ( theInput.IsKeyDown(GLFW_KEY_Q) ) {
    ApplyTorque(15000.f);
  }
}