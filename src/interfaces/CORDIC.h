#ifndef INTERFACES_CORDIC_H_
#define INTERFACES_CORDIC_H_

#include "stm32g4xx_hal.h"

#ifdef HAL_CORDIC_MODULE_ENABLED

#include "stm32g4xx_hal_cordic.h"

namespace is {

namespace interfaces {

class Cordic final
{
private:
	CORDIC_HandleTypeDef* _cordic;
	CORDIC_ConfigTypeDef sConfig;
	int32_t _args[2] = {0};
	int32_t _result[2] = {0};

public:

	Cordic(CORDIC_HandleTypeDef* cordic): _cordic(cordic)
	{
		sConfig.Function = CORDIC_FUNCTION_PHASE;
		sConfig.Precision = CORDIC_PRECISION_8CYCLES;
		sConfig.Scale = CORDIC_SCALE_0;
		sConfig.NbWrite = CORDIC_NBWRITE_2;
		sConfig.NbRead = CORDIC_NBREAD_2;
		sConfig.InSize = CORDIC_INSIZE_32BITS;
		sConfig.OutSize = CORDIC_OUTSIZE_32BITS;
	}

	void
	configure()
	{
		HAL_CORDIC_Configure(_cordic, &sConfig);
	}

	int32_t
	phase(int32_t x, int32_t y)
	{
		_args[0] = y;
		_args[1] = x;
		HAL_CORDIC_Calculate_DMA(_cordic, _args, _result, 1, CORDIC_DMA_DIR_IN_OUT);
		while(HAL_CORDIC_GetState(_cordic) != HAL_CORDIC_STATE_READY) { }
		return _result[0];
	}

};

} /* namespace interfaces */
} /* namespace is */

#endif /* HAL_CORDIC_MODULE_ENABLED */

#endif /* INTERFACES_CORDIC_H_ */
