#ifndef WG_TASK_GAME_H
#define WG_TASK_GAME_H

#include <string>
#include "Window.h"
#include "GameData.h"
#include "TextureManager.h"
#include "graph.h"
#include "json-parser.h"
#include "KKSCoordsCalculator.h"

constexpr char GAME_NAME[] = "WG_TASK";     //< game name
constexpr float frameTime = 1.f / 2.f;     //< 60 fps frame time

/**
 *  /brief Game class
 *
 *  This class represents entire content of game
 */
class Game {
public:
    Game();
    ~Game();

    /// Handle user input events
    void handleInput();
    /// Poll and process events
    void update();

    /// Renders entire game
    void render();

    /*loading resourses and preparing background, vertecies and edges for drawing
    (everething that should be done only once*/
    void renderCalculations();

    Window* getWindow();

    void restartClock();

    bool isDone() const { return window_.isDone(); }

    void setGameState(GameList::GameState gs) { state_ = gs; }
private:
    sf::Clock clock_;
    sf::Time elapsed_;
    Window window_;
    TextureManager textureManager_;

    //how to store???
    GraphIdx g;
    KKSCoordsCalculator calc;
    GameList::GameState state_;
};


#endif //WG_TASK_GAME_H
