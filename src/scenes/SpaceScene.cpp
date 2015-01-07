#include "SpaceScene.hpp"

#include "../drawable/Wavefront.hpp"

SpaceScene::SpaceScene() {

  uptr<Wavefront> box(new Wavefront("models/SimpleShip/Spaceship.obj"));
  //uptr<Wavefront> box(new Wavefront("/home/dragn/SportSpaceship/SportsSpaceship.obj"));
  //uptr<Wavefront> box(new Wavefront("/home/dragn/Documents/box.obj"));
  box->pos(V3(0.2, 0, 2));
  box->scale(V3(0.5f));

  camera.moveSpeed = 10;
  camera.pos(V3(0,0,-20));

  if (box->isOk()) _objects.push_back(std::move(box));
}
