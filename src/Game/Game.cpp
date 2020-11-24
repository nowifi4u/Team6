#include "Game.h"
Game::Game()
    :window_(GAME_NAME, sf::Vector2u(640, 480))
{
        // loading texture
}

Game::~Game()
{

}

void Game::update()
{
    if(elapsed_.asSeconds() >= frameTime)
    {
        window_.update();
        elapsed_ -= sf::seconds(frameTime);
    }
}

void Game::render()
{
        window_.beginDraw();
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);
        window_.draw(shape);
        window_.endDraw();

}

void Game::handleInput()
{

}

void Game::restartClock()
{
    elapsed_ += clock_.restart();
}