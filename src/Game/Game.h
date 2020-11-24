#ifndef WG_TASK_GAME_H
#define WG_TASK_GAME_H

#include <string>
#include "Window/Window.h"

constexpr char GAME_NAME[] = "WG_TASK";     //< game name
constexpr float frameTime = 1.f / 60.f;     //< 60 fps frame time

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
    Window& getWindow();

    void restartClock();


    bool isDone() const { return window_.isDone(); }

private:
    sf::Clock clock_;
    sf::Time elapsed_;
    Window window_;
};


#endif //WG_TASK_GAME_H
