#ifndef SIO_EVENT_THREAD_HEADER
#define SIO_EVENT_THREAD_HEADER

#include <thread>
#include <atomic>
#include <functional>

#include <queue>
#include <mutex>
#include <condition_variable>

namespace sio {
	class event_thread;
	
	namespace this_thread {
		using namespace std::this_thread;
		
		void push(std::function<void()>);
		
		template<typename fnT, typename... argsT>
		void push(fnT fn, argsT... args) {
			static_cast<void(&)(std::function<void()>)>(push)(std::bind(fn, args...));
		}
		
		// Call this when you want a pre existing thread to
		// start an event loop. Effectively turning it into
		// an event_thread.
		void event_loop();
	}
	
	/**
	 * \brief The function that get's called within sio::event_thread
	 *        when it's created. This is required to be called for
	 *        the current thread to become an event_thread. Otherwise
	 *        you can create a sio::event_thread.
	 */
	//void event_threadf();
	
	/**
	 * @brief Event thread that wraps std::thread but add's three new
	 *        functions sync, async, and push. Any thread can be turned
	 *        into a sio::event_thread by calling the sio::event_threadf
	 *        function within the thread.
	 */
	class event_thread {
	private:
		std::mutex* _m;
		std::mutex* _qmtx;
		std::unique_lock<std::mutex>* _lk;
		std::condition_variable* _cv;
		std::queue<std::function<void()>>* _q;
		
		std::thread _thrd;
	public:
		event_thread();
		event_thread(std::function<void()> f);
		event_thread(event_thread&&);
		event_thread(event_thread&) = delete;
		event_thread(const event_thread&) = delete;
		
		~event_thread();
		
		event_thread& operator=(event_thread&&);
		
		bool joinable();
		std::thread::id get_id() const;
		std::thread::native_handle_type native_handle();
		void join();
		void detach();
		void swap(event_thread&);
		
		/**
		 * @brief Push callback onto event thread queue.
		 */
		void push(std::function<void()>);
		
//		template<typename retT, typename... argsT>
//		inline void push(std::function<retT(argsT...)> f, argsT... args) {
//			void(event_thread::*pushfn)(std::function<void()>);
//			pushfn = static_cast<void(event_thread::*)(std::function<void()>)>(&event_thread::push);
			
//			this->*pushfn([=, args...]{ f(args...); });
//		}
		
		static void push(std::thread::id&, std::function<void()>);
		
		/**
		 * @brief Calls pending callbacks in event_thread and blocking calling thread
		 *        until event_thread is finished.
		 */
		void sync();
		
		static void sync(std::thread::id&);
		
		/**
		 * @brief Calls pending callbacks in event_thread.
		 */
		void async();
		
		static void async(std::thread::id&);
		
		static unsigned hardware_concurrency();
	};
	
}

#endif//SIO_EVENT_THREAD_HEADER
