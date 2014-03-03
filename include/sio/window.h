#ifndef SIO_WINDOW_HEADER
#define SIO_WINDOW_HEADER

#include "event_thread.h"
#include "window/scancode.h"

class _sioWindowImpl;

namespace sio {
	class window;
	
	/**
	 * @brief The window class holds all the functionality of basic
	 *        window functionality as well as events. Window's must be
	 *        created within a sio::event_thread. See sio/event_thread.h
	 *        for more details. 
	 */
	class window {
	public:
		class keydown_event;
		class keyup_event;
		class kbfocus_event;
		class kbblur_event;
		
		class mbdown_event;
		class mbup_event;
		class mfocus_event;
		class mblur_event;
		class mmove_event;
		class mwheel_event;
		
		class close_event;
		class resize_event;
		class redraw_event;
		
		class drop_event;
		
		#include "window/scancode.inl"
		
		window();
		virtual ~window();
		
		/**
		 * @brief Shows window when hidden. Does not restore.
		 */
		void show();
		
		/**
		 * @brief Hides window. Does not minimize.
		 */
		void hide();
		
		/**
		 * @brief Destroys window, freeing memory.
		 */
		void destroy();
		
		/**
		 * @brief Puts window in focus. 
		 */
		void focus();
		
		/**
		 * @brief Defocuses window. Window does not recieve keyboard input.
		 */
		void blur();
		
		/**
		 * @brief Minimizes window. 
		 */
		void minimize();
		
		/**
		 * @brief Maximizes window taking up most of the screen besides OS controls.
		 */
		void maximize();
		
		/**
		 * @brief Restores window. Shows window after a window has been minimized.
		 */
		void restore();
		
		void title(std::string);
		std::string title() const;
		
		int32_t width() const;
		int32_t height() const;
		
		void width(const int32_t w);
		void height(const int32_t h);
		
		int32_t pos_x() const;
		int32_t pos_y() const;
		
		void pos_x(const int32_t);
		void pos_y(const int32_t);
		
		void size(const int32_t w, const int32_t h);
		
		void pos(const int32_t x, const int32_t y);
		
		// ==================================
		// EVENTS
		
		virtual void on(keydown_event& e);
		virtual void on(keyup_event& e);
		virtual void on(kbfocus_event& e);
		virtual void on(kbblur_event& e);
		
		virtual void on(mbdown_event& e);
		virtual void on(mbup_event& e);
		virtual void on(mfocus_event& e);
		virtual void on(mblur_event& e);
		virtual void on(mmove_event& e);
		virtual void on(mwheel_event& e);
		
		virtual void on(close_event& e);
		virtual void on(resize_event& e);
		virtual void on(redraw_event& e);
		
		virtual void on(drop_event& e);
		
		// FOR INTERNAL USE ONLY
		_sioWindowImpl* const _sioWindow_impl;
	};
	
	class window::keydown_event {
	public:
		int32_t key;
		uint16_t mod;
		uint8_t repeat;
	};
	
	class window::keyup_event {
	public:
		int32_t key;
		uint16_t mod;
		uint8_t repeat;
	};
	
	class window::kbfocus_event {
	public:
		sio::window* window;
	};
	
	class window::kbblur_event {
	public:
		sio::window* window;
	};
	
	class window::mbdown_event {
	public:
		uint8_t mb;
		int32_t x;
		int32_t y;
	};
	
	class window::mbup_event {
	public:
		uint8_t mb;
		int32_t x;
		int32_t y;
	};
	
	class window::mfocus_event {
	public:
		
	};
	
	class window::mblur_event {
	public:
		
	};
	
	class window::mmove_event {
	public:
		int32_t x;
		int32_t y;
		int32_t xrel;
		int32_t yrel;
		
		uint32_t state;
	};
	
	class window::mwheel_event {
	public:
		float x;
		float y;
	};
	
	class window::close_event {
	public:
		
	};
	
	class window::resize_event {
	public:
		/**
		 * @brief The new width. window::width() would return same value.
		 */
		int32_t width;
		/**
		 * @brief The new height. window::height() would return same value.
		 */
		int32_t height;
	};
	
	class window::redraw_event {
	public:
		
	};
	
	class window::drop_event {
	public:
		std::string path_name;
	};
}

#endif//SIO_WINDOW_HEADER
