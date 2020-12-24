#ifndef WG_TASK_WINDOW_H
#define WG_TASK_WINDOW_H

#include <string>
#include <iostream>
#include <map>
#include "graph.h"
#include <SFML/Graphics.hpp>

/*!
 *  /brief class that designed for window logic encapsulation
 *
 *  Class simplify usage of  sfml window
 */
class Window {
public:
    Window(const std::string& windowTitle, const sf::Vector2u& windowSize, const  sf::Uint8 delta);
    ~Window();

    /// Clears window
    void beginDraw();

    /// Display drawed to buffer
    void endDraw();

    /// Poll and process events.
    void update();

    bool isDone() const { return isDone_; }
    bool isFullscreen() const { return isFullscreen_; }
    sf::Vector2u getWindowSize() const { return windowSize_; }

    /// Toggle fullscreen
    void toggleFullscreen();

    /// Draws drawable to buffer
    void draw(sf::Drawable& drawable);

    Window() = delete;

    sf::Uint8 getDelta() { return delta; }

    sf::Sprite& getBackGround() { return background; }

    std::map<GraphIdx::vertex_descriptor, sf::Sprite> nodes_g;
    std::map<GraphIdx::edge_descriptor, sf::Sprite> edges_g;

private:
    void create();
    void destroy();
    std::string windowTitle_;
    sf::Vector2u windowSize_;
    sf::RenderWindow window_;

    bool isDone_;
    bool isFullscreen_;

    sf::Uint8 delta;    
    sf::Sprite background;
};


#endif //WG_TASK_WINDOW_H
