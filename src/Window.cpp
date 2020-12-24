#include "Window.h"


void Window::create()
{
    auto style_ = isFullscreen_ ? sf::Style::Fullscreen : sf::Style::Default;
    window_.create({windowSize_.x, windowSize_.y, 32},
                   windowTitle_, style_);
}

Window::Window(const std::string &windowTitle, const sf::Vector2u &windowSize, const sf::Uint8 delta)
    :windowTitle_(windowTitle), windowSize_(windowSize), delta(delta)
{
    isFullscreen_ = false;
    isDone_ = false;
    create();
}

void Window::destroy()
{
    window_.close();
}

Window::~Window()
{
    destroy();
}
void Window::beginDraw()
{
    window_.clear(sf::Color::White);
}

void Window::endDraw()
{
    window_.display();
}

void Window::update()
{
   sf::Event event;
   while(window_.pollEvent(event))
   {
       if(event.type == sf::Event::Closed)
       {
           isDone_ = true;
       } else if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::F5)
       {
           toggleFullscreen();
       }
   }
}
void Window::toggleFullscreen()
{
    /// /bug might work incorrect with some tiling window managers, e.g. bspwm
    isFullscreen_ = !isFullscreen_;
    std::cout << "Switched!\n";
    destroy();
    create();
}

void Window::draw(sf::Drawable &drawable)
{
    window_.draw(drawable);
}

