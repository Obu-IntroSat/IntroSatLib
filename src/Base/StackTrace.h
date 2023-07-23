#ifndef BASE_STACKTRACE_H_
#define BASE_STACKTRACE_H_

#include <stack>
#include <deque>
#include "Status.h"

namespace IntroSatLib
{
namespace Base
{

template<typename T>
class StackTrace
{
private:
	std::stack<T> _currentStack;
	std::deque<T> _readyStack;
public:
	void Add(T trace)
	{
		_currentStack.push(trace);
	}
	void Remove(Status status)
	{
		if (status != Status::Ok)
		{
			_readyStack.push_front(_currentStack.top());
		}
		else
		{
			_readyStack.clear();
		}

		_currentStack.pop();

		if (_currentStack.empty())
		{

			_readyStack.clear();
		}
	}
};

}
}

#endif /* BASE_STACKTRACE_H_ */
