#ifndef MEMORY_SHARED_PTR_H_
#define MEMORY_SHARED_PTR_H_

#include <cinttypes>
#include "../../third_party/etl/include/etl/memory.h"
#include "../../third_party/etl/include/etl/ipool.h"

namespace is {
namespace memory {

template<typename TObject>
struct shared_object
{
	std::uint32_t _count = 0;
	TObject _object;

public:
	template<typename... Args>
	shared_object(Args&&... args): _object(etl::forward<Args>(args)...) { };

	constexpr void
	acquire() noexcept
	{ _count++; }

	constexpr bool
	release() noexcept
	{ return --_count == 0; }
};

template<typename TObject>
class shared_ptr
{
public:

	typedef shared_object<TObject> type;

private:
	shared_object<TObject> *_object;
	etl::ipool *_pool;

private:
	constexpr void acquire()
	{
		if (_object == 0) { return; }
		_object->acquire();
	}

	constexpr void release()
	{
		if (_pool == 0) { return; }
		if (_object == 0) { return; }
		if (_object->release()) { _pool->destroy(_object); }
	}

public:
	constexpr TObject* get() const
	{
		if (_object == 0) { return 0; }
		return &(_object->_object);
	}

	constexpr TObject* operator->()
	{
		if (_object == 0) { return 0; }
		return &(_object->_object);
	}

	constexpr TObject& operator*()
	{
		if (_object == 0) { return 0; }
		return &(_object->_object);
	}

public:
	constexpr shared_ptr(): _object(0), _pool(0) { }

	constexpr shared_ptr(shared_object<TObject> *object, etl::ipool *pool): _object(object), _pool(pool)
	{ acquire(); }

	constexpr shared_ptr(const shared_ptr &ptr): _object(ptr._object), _pool(ptr._pool)
	{ acquire(); }

	shared_ptr& operator=(const shared_ptr &rhs)
	{
		if (&rhs == this) { return *this; }
		if (_object == rhs._object) { return *this; }
		release();
		_pool = rhs._pool;
		_object = rhs._object;
		acquire();
		return *this;
	}

	virtual ~shared_ptr() { release(); }
};

template<typename TObject, typename TShared = shared_ptr<TObject>, typename... Args>
TShared make_shared(etl::ipool *pool, Args&&... args)
{
	shared_object<TObject> *object = pool->create<shared_object<TObject>>(etl::forward<Args>(args)...);
	TShared result(object, pool);
	return result;
}

} /* namespace memory */
} /* namespace is */

#endif /* MEMORY_SHARED_PTR_H_ */
