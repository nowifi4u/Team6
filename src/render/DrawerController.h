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
	sf::RenderWindow* drawer_window_ = nullptr;

	boost::thread* drawer_thread_ = nullptr;
	InterfaceDrawer* drawable_;

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

	void start(InterfaceDrawer* drawable, const sf::VideoMode& videomode, const sf::String& title = "", sf::Uint32 style = 7U, const sf::ContextSettings& context = sf::ContextSettings())
	{
		LOG_2("DrawerController::start");

		if (drawable != nullptr && drawer_thread_ == nullptr)
		{
			drawable_ = drawable;
			drawer_thread_ = new boost::thread(drawer_thread, boost::ref(drawable_), boost::ref(drawer_window_), boost::ref(videomode), boost::ref(title), style, boost::ref(context));
		}
	}

	void start(const sf::VideoMode& videomode, const sf::String& title = "", sf::Uint32 style = 7U, const sf::ContextSettings& context = sf::ContextSettings())
	{
		LOG_2("DrawerController::start");

		if (drawable_ != nullptr && drawer_thread_ == nullptr)
		{
			drawer_thread_ = new boost::thread(drawer_thread, boost::ref(drawable_), boost::ref(drawer_window_), boost::ref(videomode), boost::ref(title), style, boost::ref(context));
		}
	}

	void stop()
	{
		LOG_2("DrawerController::stop");

		if (drawer_thread_ != nullptr)
		{
			delete drawer_thread_;
			drawer_thread_ = nullptr;
			drawer_window_ = nullptr;
		}
	}

	void window_wait() const
	{
		LOG_2("DrawerController::window_wait");

		while (drawer_window_ == nullptr);

		LOG_2("DrawerController::window_wait done");
	}

	void window_join()
	{
		LOG_2("DrawerController::window_join");

		if (drawer_thread_ != nullptr)
		{
			drawer_thread_->join();
		}
	}

	DrawerController(InterfaceDrawer* drawable)
		: drawable_(drawable)
	{
	}

	DrawerController(InterfaceDrawer* drawable, const sf::VideoMode& videomode, const sf::String& title = "", sf::Uint32 style = 7U, const sf::ContextSettings& context = sf::ContextSettings())
		: DrawerController(drawable)
	{
		start(videomode, title, style, context);
	}

	~DrawerController()
	{
		stop();
	}

	void interrupt(InterfaceDrawer* drawable)
	{
		LOG_2("DrawerController::interrupt with set");

		if (drawer_thread_ != nullptr)
		{
			drawable_ = drawable;
			drawer_thread_->interrupt();
		}
	}

	void interrupt()
	{
		LOG_2("DrawerController::interrupt");

		if (drawer_thread_ != nullptr)
		{
			drawer_thread_->interrupt();
		}
	}
};
