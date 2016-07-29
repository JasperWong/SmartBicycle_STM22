#ifndef	BICYCLE_H
#define	BICYCLE_H
#include "stdint.h"
#include "ledpoint.h"

extern  uint8_t USART_RX_5;

typedef struct 
{
	enum state{
		STATE_FREEDOM_MODE,
		STATE_SPORT_MODE,
		STATE_SPORT_MODE_WARM1,
		STATE_SPORT_MODE_WARM2,
		STATE_SPORT_MODE_WARM3,
		STATE_SPORT_MODE_LEVEL,
		STATE_SPORT_MODE_CLIMB,
		STATE_LESSON_MODE,
		STATE_LESSON_MODE_WARM1,
		STATE_LESSON_MODE_WARM2,
		STATE_LESSON_MODE_WARM3,
		STATE_LESSON_MODE_LEVEL,
		STATE_LESSON_MODE_CLIMB,
        STATE_OUTDOOR_MODE,
	};
	uint8_t outer_status;
	uint8_t	inner_status;
}BICYCLE;

void freedom_mode(uint8_t rx);

#endif
