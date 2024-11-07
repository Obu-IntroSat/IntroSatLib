#ifndef EXCEPTION_ARGUMENT_OUT_OF_RANGE_H_
#define EXCEPTION_ARGUMENT_OUT_OF_RANGE_H_

#include <src/exception/argument.hpp>

namespace is {
namespace exception {

struct argument_out_of_range: public argument {
	argument_out_of_range(string_type file, numeric_type line):
		argument("Error in argument out of range", file, line) { }
};

} /* namespace exception */
} /* namespace is */

#endif /* EXCEPTION_ARGUMENT_OUT_OF_RANGE_H_ */
