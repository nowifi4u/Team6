#pragma once

#include <boost/thread.hpp>

#include "drawer/InterfaceDrawer.h"



inline void drawer_thread(InterfaceDrawer*& drawable, sf::RenderWindow*& callback, const sf::VideoMode& videomode, const sf::String& title, sf::Uint32 style = 7U, const sf::ContextSettings& context = sf::ContextSettings())
{
	LOG_2("drawer_thread: Creating RenderWindow...");
	sf::RenderWindow renderwindow(videomode, title, style, context);
	LOG_2("drawer_thread: Saving RenderWindow pointer...");
	callback = &renderwindow;

	LOG_2("drawer_thread: Render loop started");
	while (renderwindow.isOpen())
	{
		try
		{
			InterfaceDrawer* drawable_ = drawable;

			drawable_->draw(renderwindow);
		}
		catch (boost::thread_interrupted&)
		{
			LOG_2("drawer_thread: interrupted");
		}
	}
}


class DrawerController
{
	InterfaceDrawer* drawable_;
	sf::RenderWindow* drawer_window_;

	boost::thread* drawer_thread_;

public:

	sf::RenderWindow* get_window()
	{
		return drawer_window_;
	}

	InterfaceDrawer* get_drawer()
	{
		return drawable_;
	}

	template <class ConvertionTy>
	ConvertionTy* get_drawer()
	{
		return dynamic_cast<ConvertionTy*>(drawable_);
	}

	DrawerController(InterfaceDrawer* drawable, const sf::VideoMode& videomode, const sf::String& title, sf::Uint32 style = 7U, const sf::ContextSettings& context = sf::ContextSettings())
		: drawable_(drawable)
	{
		drawer_thread_ = new boost::thread(drawer_thread, boost::ref(drawable_), boost::ref(drawer_window_), boost::ref(videomode), boost::ref(title), style, boost::ref(context));
	}

	~DrawerController()
	{
		delete drawer_thread_;
	}

	void interrupt(InterfaceDrawer* drawable)
	{
		drawable_ = drawable;
		drawer_thread_->interrupt();
	}
};
