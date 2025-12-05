#ifndef INTROSATLIB_DEF_H_
#define INTROSATLIB_DEF_H_

#include <stdint.h>

enum ISL_StatusTypeDef {
	ISL_OK      = 0x00U,
	ISL_ERROR	= 0x01U,
	ISL_BUSY	= 0x02U,
	ISL_TIMEOUT	= 0x03U
};

#if !defined(UNUSED)
#define UNUSED(X) (void) X      /* To avoid compiler warnings warnings */
#endif /* UNUSED */

#define RETURN_STATUS_IF_NOT_OK(func, status) if ((status = func) != ISL_StatusTypeDef::ISL_OK) { return status; }
#define RETURN_STATUS_IF_NOT_OK_SILENT(func) {ISL_StatusTypeDef __status__ = ISL_StatusTypeDef::ISL_OK; \
if ((__status__ = func) != ISL_StatusTypeDef::ISL_OK) { return __status__; }}

#endif /* INTROSATLIB_DEF_H_ */
