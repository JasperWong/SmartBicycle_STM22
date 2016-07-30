#ifndef	BICYCLE_H
#define	BICYCLE_H
#include "bsp_usart1.h"
#include "stdint.h"
#include "ledpoint.h"

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

typedef struct
{
	enum state_parser{
		STATE_WAIT_G, 																	
		STATE_WAIT_GUIDE_DATA,
		STATE_WAIT_D, 
		STATE_WAIT_DISTANCE_DATA,			
//		STATE_WAIT_CHECKSUM, 				
		STATE_WAIT_NEWLINE,						
		STATE_PARSE_FINISH_PENDING,
	};
	
	uint8_t status;
	uint8_t data_guide;
	int data_distance;
	
}COMMAND_PARSER;

enum state_guide{
    STATE_NONE ,
    STATE_OWN,
    STATE_TURN_LEFT,
    STATE_TURN_RIGHT,
    STATE_TURN_LEFT_FRONT,
    STATE_TURN_RIGHT_FRONT,
    STATE_TURN_LEFT_BACK,
    STATE_TURN_RIGHT_BACK,
    STATE_TURN_BACK,
    STATE_GO_AHEAD,
    STATE_DONE,
};
    

uint8_t PARSER_IsParseFinish(COMMAND_PARSER *parser);
void PARSER_Reset(COMMAND_PARSER *parser);
void PARSER_RunPaser(COMMAND_PARSER *parser,uint16_t data);
void freedom_mode();
void OnRecieve(int32_t data);
#endif
