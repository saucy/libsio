#include <iostream>
#include "event_thread.h"

namespace {
	
	struct EventThreadVars {
		inline EventThreadVars() : lk(m) {}
		std::mutex m;
		std::mutex qmtx;
		std::unique_lock<std::mutex> lk;
		std::condition_variable cv;
		std::queue<std::function<void()>> q;
	};
	
	thread_local EventThreadVars* vars = nullptr;
	std::map<std::hash<std::thread::id>::result_type, EventThreadVars*> varsMap;
	
	std::hash<std::thread::id> eventThreadIdHashFn;
	
	void eventThreadInactivePushFn(std::function<void()> f);
	void eventThreadActivePushFn(std::function<void()>);
	
	thread_local void(*eventThreadLocalPushFn)(std::function<void()>) = &eventThreadInactivePushFn;
	
	void eventThreadActivePushFn(std::function<void()> f) {
		vars->q.push(std::move(f));
	}
	
	void eventThreadInactivePushFn(std::function<void()> f) {
		vars = new EventThreadVars;
		eventThreadLocalPushFn = &eventThreadActivePushFn;
		eventThreadActivePushFn(std::move(f));
	}
	
	struct init {
		init() {}
		~init() {
			if(vars != nullptr) {
				delete vars;
			}
		}
	} init;
	
	void eventThreadLoop() {
		
		for(;;) {
			vars->cv.wait(vars->lk);
			vars->qmtx.lock();
			
			while(vars->q.size() > 0) {
				vars->q.front()();
				vars->q.pop();
			}
			
			vars->qmtx.unlock();
		}
	}
	
	void eventThreadInit(  std::function<void()> callback
	                     , std::mutex** m
		                   , std::mutex** qmtx
		                   , std::unique_lock<std::mutex>** lk
		                   , std::condition_variable** cv
		                   , std::queue<std::function<void()>>** q) {
		
		auto thrdHash = eventThreadIdHashFn(std::this_thread::get_id());
		
		if(vars == nullptr && varsMap.find(thrdHash) == varsMap.end()) {
			vars = new EventThreadVars;
		} else
		if(vars != nullptr && varsMap.find(thrdHash) == varsMap.end()) {
			varsMap.insert({thrdHash, vars});
		} else
		if(vars == nullptr && varsMap.find(thrdHash) != varsMap.end()) {
			vars = varsMap[thrdHash];
			vars->lk.lock();
		} else
		if(vars != nullptr && varsMap.find(thrdHash) != varsMap.end()) {
			EventThreadVars* thrdVars = varsMap[thrdHash];
			if(vars->q.size() > 0) {
				while(thrdVars->q.size() > 0) {
					vars->q.push(thrdVars->q.front());
					thrdVars->q.pop();
				}
			} else {
				vars->q.swap(thrdVars->q);
			}
			
			varsMap.insert({thrdHash, vars});
			delete thrdVars;
		}
		
		eventThreadLocalPushFn = &eventThreadActivePushFn;
		
		// if m is a nullptr, they all are.
		if(m != nullptr) {
			std::mutex* old_m = *m;
			std::unique_lock<std::mutex>* old_lk = *lk;
			std::condition_variable* old_cv = *cv;
		
			(*m) = &vars->m;
			(*qmtx) = &vars->qmtx;
			(*lk) = &vars->lk;
			(*cv) = &vars->cv;
			(*q) = &vars->q;
			
			varsMap.insert({
				eventThreadIdHashFn(std::this_thread::get_id()), vars
			});
			
			callback();
			
			old_cv->notify_one();
		} else {
			varsMap.insert({
				eventThreadIdHashFn(std::this_thread::get_id()), vars
			});
		
			callback();
		}
		
		eventThreadLoop();
	}
	
	std::thread createEventThread(  std::function<void()> callback
	                              , std::mutex** m
		                            , std::mutex** qmtx
		                            , std::unique_lock<std::mutex>** lk
		                            , std::condition_variable** cv
		                            , std::queue<std::function<void()>>** q) {
		
		std::mutex tmpM;
		std::unique_lock<std::mutex> tmpLK(tmpM);
		std::condition_variable tmpCV;
		
		(*m) = &tmpM;
		(*lk) = &tmpLK;
		(*cv) = &tmpCV;
		
		std::thread thrd([=]{
			eventThreadInit(callback, m, qmtx, lk, cv, q);
		});
		
		try {
			(*cv)->wait(**lk);
		} catch(std::exception& e) {
			
		}
		
		tmpLK.unlock();
		
		return thrd;
	}
	
	void blankFunc() { }
	
	void preInitEventThread(std::thread::id evtId) {
		auto thrdHash = eventThreadIdHashFn(evtId);
		EventThreadVars* thrdVars;
		
		thrdVars = new EventThreadVars;
		thrdVars->lk.unlock();
		
		varsMap.insert({thrdHash, thrdVars});
	}	
}

void sio::this_thread::push(std::function<void()> fn) {
	eventThreadLocalPushFn(fn);
}

void sio::this_thread::event_loop() {
	eventThreadInit([]{}, nullptr, nullptr, nullptr, nullptr, nullptr);
}

sio::event_thread::event_thread(std::function<void()> callback)
	: _thrd(createEventThread(callback, &_m, &_qmtx, &_lk, &_cv, &_q)) { }

sio::event_thread::event_thread()
	: _thrd(createEventThread(blankFunc, &_m, &_qmtx, &_lk, &_cv, &_q)) { }

//	: _m(new std::mutex)
//	, _qmtx(new std::mutex)
//	, _lk(new std::unique_lock<std::mutex>(_m))
//	, _q(new std::queue<std::function<void()>>)
//	, _thrd(eventThreadInit, []{}, &_m, &_qmtx, &_lk, &_cv, &_q) { }

sio::event_thread::~event_thread() {
	
}

sio::event_thread& sio::event_thread::operator=(sio::event_thread&& t) {
	if(joinable()) {
		std::terminate();
		swap(t);
		return *this;
	}
}

bool sio::event_thread::joinable() {
	return _thrd.joinable();
}

std::thread::id sio::event_thread::get_id() const {
	return _thrd.get_id();
}

std::thread::native_handle_type sio::event_thread::native_handle() {
	return _thrd.native_handle();
}

void sio::event_thread::join() {
	// TODO: Join event queue
	
	if(vars == nullptr) {
		
	}
	
	_thrd.join();
}

void sio::event_thread::detach() {
	_thrd.detach();
}

void sio::event_thread::swap(sio::event_thread& t) {
	_thrd.swap(t._thrd);
}

void sio::event_thread::push(std::function<void()> f) {
	_qmtx->lock();
	_q->push(f);
	_qmtx->unlock();
}

void sio::event_thread::async() {
	_cv->notify_one();
}

void sio::event_thread::sync() {
	_qmtx->lock();
	_cv->notify_one();
	_qmtx->unlock();
	
	while(_qmtx->try_lock()) {
		_qmtx->unlock();
	}
	
	_qmtx->lock();
	_qmtx->unlock();
}

void sio::event_thread::sync(std::thread::id& evtId) {
	auto thrdHash = eventThreadIdHashFn(evtId);
	EventThreadVars* thrdVars;
	
	if(varsMap.find(thrdHash) == varsMap.end()) {
		return;
	}
	
	thrdVars = varsMap[thrdHash];
	
	thrdVars->qmtx.lock();
	thrdVars->cv.notify_one();
	thrdVars->qmtx.unlock();
	
	while(thrdVars->qmtx.try_lock()) {
		thrdVars->qmtx.unlock();
	}
	
	thrdVars->qmtx.lock();
	thrdVars->qmtx.unlock();
}

void sio::event_thread::async(std::thread::id& evtId) {
	auto thrdHash = eventThreadIdHashFn(evtId);
	EventThreadVars* thrdVars;
	
	if(varsMap.find(thrdHash) == varsMap.end()) {
		return;
	}
	
	thrdVars = varsMap[thrdHash];
	thrdVars->cv.notify_one();
	
}

void sio::event_thread
::push(std::thread::id& evtId, std::function<void()> f) {
	auto thrdHash = eventThreadIdHashFn(evtId);
	EventThreadVars* thrdVars;
	
	if(varsMap.find(thrdHash) == varsMap.end()) {
		return;
	}
	
	thrdVars = varsMap[thrdHash];
	
	thrdVars->qmtx.lock();
	thrdVars->q.push(f);
	thrdVars->qmtx.unlock();
}

unsigned sio::event_thread::hardware_concurrency() {
	return std::thread::hardware_concurrency();
}
