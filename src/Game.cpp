#include "headers/Game.hpp"
#include <stdlib.h>
#include <algorithm>
#include <list>
#include <iostream>

namespace engine {
  Game* Game::gGame = NULL;
  engine::Level* Game::gLevel = NULL;
  engine::GameObjectList Game::gObjects = engine::GameObjectList();
  Game::Game(const std::string title) {
    windowTitle = title;
    window.create(sf::VideoMode(800, 800), windowTitle);
    mUpdateRate = 1.0f / 20.0f;
    gGame = this;
  }

  Game::~Game() {

  }

  int Game::run(void) {
    this->GameLoop();
    return 0;
  }

  void Game::GameLoop(void) {
    bool lc = false;
    sf::Event event;

    sf::Clock anUpdateClock;

    anUpdateClock.restart();

    // When do we need to update next (in milliseconds)?
    sf::Int32 anUpdateNext = anUpdateClock.getElapsedTime().asMilliseconds();
    while (window.isOpen()) {
      window.clear();
      sf::Uint32 anUpdates = 0;

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        } else if (event.type == sf::Event::KeyPressed) {
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
          }
        }
      }
      sf::Int32 anUpdateTime = anUpdateClock.getElapsedTime().asMilliseconds();
      if (gLevel != NULL) {
        if (!lc){
          printf("Running all level things\n");
          lc = true;
        }
        while((anUpdateTime - anUpdateNext) >= mUpdateRate && anUpdates++ < 5) {
          for (engine::GameObject* gm : gObjects) {
            printf("Fixed Updating GM\n");
            gm->fixedUpdate();
            if (gm->sprite != NULL) {
              gm->sprite->setPosition(gm->getPosition());
            } else if (gm->shape != NULL) {
              gm->shape->setPosition(gm->getPosition());
            } else if (gm->text != NULL) {
              gm->text->setPosition(gm->getPosition());
            }
            // else if (gm->vertices != NULL) {
              // gm->vertices->setPosition(gm->getPosition());
            // }
          }
        }
        for (engine::GameObject* gm : gObjects)gm->update();

        for (engine::GameObject* gm : gObjects) {
          if (gm->sprite != NULL) {
            printf("Drawing sprite\n");
            window.draw(*gm->sprite);
          } else if (gm->shape != NULL) {
            window.draw(*gm->shape);
          } else if (gm->text != NULL) {
            window.draw(*gm->text);
          }
        }
      }
      // window.draw(shape);
      window.display();
    }
  }

  engine::Level* Game::GetLevel(void) {
    return gLevel;
  }

  Game* Game::GetGame(void) {
    return gGame;
  }

  GameObjectList Game::GetGameObjects(void) {
    return gObjects;
  }

  void Game::SetLevel(engine::Level* level) {
    gLevel = level;
    // gObjects = level->getGameObjects();
  }
}
