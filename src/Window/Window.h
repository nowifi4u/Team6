#ifndef WG_TASK_WINDOW_H
#define WG_TASK_WINDOW_H

#include <string>
#include <SFML/Graphics.hpp>

/*!
 *  /brief class that designed for window logic encapsulation
 *
 *  Class simplify usage of  sfml window
 */
class Window {
public:
    Window(const std::string& windowTitle, const sf::Vector2u& windowSize);
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
private:
    void create();
    void destroy();
    std::string windowTitle_;
    sf::Vector2u windowSize_;
    sf::RenderWindow window_;

    bool isDone_;
    bool isFullscreen_;
    bool style_;
};


#endif //WG_TASK_WINDOW_H
