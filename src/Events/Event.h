#ifndef EVENTS_EVENT_H_
#define EVENTS_EVENT_H_

#include <functional>
#include <list>
#include <algorithm>
#include <utility>
#include <atomic>
#include <mutex>
#include <future>

namespace IntroSatLib
{
	template <typename... Args> class event_handler
	{
	public:
		typedef std::function<void(Args...)> handler_func_type;
		typedef unsigned int handler_id_type;

		explicit event_handler(const handler_func_type& handlerFunc)
			: m_handlerFunc(handlerFunc)
		{
			m_handlerId = ++m_handlerIdCounter;
		}

		// copy constructor
		event_handler(const event_handler& src)
			: m_handlerFunc(src.m_handlerFunc), m_handlerId(src.m_handlerId)
		{
		}

		// move constructor
		event_handler(event_handler&& src)
			: m_handlerFunc(std::move(src.m_handlerFunc)), m_handlerId(src.m_handlerId)
		{
		}

		// copy assignment operator
		event_handler& operator=(const event_handler& src)
		{
			if (this != src)
			{
				m_handlerFunc = src.m_handlerFunc;
				m_handlerId = src.m_handlerId;
			}
			return *this;
		}

		// move assignment operator
		event_handler& operator=(event_handler&& src)
		{
			if (this != src)
			{
				std::swap(m_handlerFunc, src.m_handlerFunc);
				m_handlerId = src.m_handlerId;
			}
			return *this;
		}

		// function call operator
		void operator()(Args... params) const
		{
			if (m_handlerFunc)
			{
				m_handlerFunc(params...);
			}
		}

		bool operator==(const event_handler& other) const
		{
			return m_handlerId == other.m_handlerId;
		}

		operator bool() const
		{
			return m_handlerFunc;
		}

		handler_id_type id() const
		{
			return m_handlerId;
		}

	private:
		handler_func_type m_handlerFunc;
		handler_id_type m_handlerId;
		static uint32_t m_handlerIdCounter;
	};

	template <typename... Args> uint32_t event_handler<Args...>::m_handlerIdCounter(0);

	template <typename... Args> class event
	{
	public:
		typedef event_handler<Args...> handler_type;

		typename handler_type::handler_id_type add(const handler_type& handler)
		{
			m_handlers.push_back(handler);

			return handler.id();
		}

		inline typename handler_type::handler_id_type add(const typename handler_type::handler_func_type& handler)
		{
			return add(handler_type(handler));
		}

		bool remove(const handler_type& handler)
		{
			auto it = std::find(m_handlers.begin(), m_handlers.end(), handler);
			if (it != m_handlers.end())
			{
				m_handlers.erase(it);
				return true;
			}

			return false;
		}

		bool remove_id(const typename handler_type::handler_id_type& handlerId)
		{
			auto it = std::find_if(m_handlers.begin(), m_handlers.end(),
				[handlerId](const handler_type& handler) { return handler.id() == handlerId; });
			if (it != m_handlers.end())
			{
				m_handlers.erase(it);
				return true;
			}

			return false;
		}

		void call(Args... params) const
		{
			handler_collection_type handlersCopy = get_handlers_copy();

			call_impl(handlersCopy, params...);
		}

		inline void operator()(Args... params) const
		{
			call(params...);
		}

		inline typename handler_type::handler_id_type operator+=(const handler_type& handler)
		{
			return add(handler);
		}

		inline typename handler_type::handler_id_type operator+=(const typename handler_type::handler_func_type& handler)
		{
			return add(handler);
		}

		inline bool operator-=(const handler_type& handler)
		{
			return remove(handler);
		}

	protected:
		typedef std::list<handler_type> handler_collection_type;

		void call_impl(const handler_collection_type& handlers, Args... params) const
		{
			for (const auto& handler : handlers)
			{
				handler(params...);
			}
		}

		handler_collection_type get_handlers_copy() const
		{
			return m_handlers;
		}

	private:
		handler_collection_type m_handlers;
	};

}

#endif // EVENTS_EVENT_H_
