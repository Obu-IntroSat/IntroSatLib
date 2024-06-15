#ifndef BASE_INTANCEOF_H_
#define BASE_INTANCEOF_H_

#include <type_traits>

namespace IntroSatLib {

namespace Base {

template<typename Base, typename Derived>
struct InstanceOf : std::enable_if<std::is_base_of<Base, Derived>::value>
{ };


} /* namespace Base */
} /* namespace IntroSatLib */

#endif /* BASE_INTANCEOF_H_ */
