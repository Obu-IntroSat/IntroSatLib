#ifndef EXCEPTION_ARGUMENT_H_
#define EXCEPTION_ARGUMENT_H_

#include "../exception/base.h"

namespace is {
namespace exception {

struct argument: public base {
	argument(string_type file, numeric_type line): base("Error in argument", file, line) { }
protected:
	argument(string_type text, string_type file, numeric_type line): base(text, file, line) { }
};

} /* namespace exception */
} /* namespace is */

#endif /* EXCEPTION_ARGUMENT_H_ */
