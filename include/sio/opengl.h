#ifndef SIO_OPENGL_HEADER
#define SIO_OPENGL_HEADER

#include <map>

#include "window.h"

namespace sio {
namespace opengl {
	
	
	typedef void* context;
	
	// Create context
	context create_context(window&);
	// Create context and share objects with already existing one
	context create_context(window&, context);
	void delete_context(context);
	
	/**
	 * @brief Set the current context of this thread.
	 */
	void current_context(window&, context);
	
	/**
	 * @brief Get the current context of this thread.
	 * @return Current Context of this thread.
	 */
	context current_context();
	
	/**
	 * @brief Swaps Windows Buffer. If used in non-current contexts thread it
	 *        is implimentation defined. 
	 */
	void swap_buffers(window&);
	
	/**
	 * @brief Swap Buffers of current contexts owning window.
	 */
	void swap_buffers();
	
	template<typename Tp, class Alloc = std::allocator<Tp>>
	class context_local {
	public:
		typedef Tp value_type;
	private:

		std::map<context, Tp> _ctxMap;
		
	public:
		typedef Tp& reference;
		typedef const Tp& const_reference;
		
		reference operator*() {
			context ctx = current_context();
			return _ctxMap[ctx];
		}

		const_reference operator*() const {
			return _ctxMap[current_context()];
		}
		
		
	};
}}

#endif//SIO_OPENGL_HEADER
