#include "bicycle.h"
#include "bsp_usart1.h"
#include <stdlib.h>

COMMAND_PARSER parser;
extern BICYCLE mBicycle;
uint8_t guide_state;
int distance;
char str_distance[6];

void freedom_mode()
{
//    printf("free\r\n");
    if(parser.status==STATE_PARSE_FINISH_PENDING)
    {
        guide_state=parser.data_guide;
        distance=parser.data_distance;
        sprintf(str_distance,"%d" ,distance);
        clear_led();
        switch (guide_state)
        {
            case STATE_NONE:
                break;
            case STATE_OWN:
                break;
            case STATE_TURN_LEFT:
                turn_left_led();
                break;
            case STATE_TURN_RIGHT:
                turn_right_led();
                break;
            case STATE_TURN_LEFT_FRONT:
                turn_leftfront_led();
                break;
            case STATE_TURN_RIGHT_FRONT:
                turn_rightfront_led();
                break;
            case STATE_TURN_LEFT_BACK:
                turn_leftback_led();
                break;
            case STATE_TURN_RIGHT_BACK:
                turn_rightfront_led();

                break;
            case STATE_TURN_BACK:

                break;
            case STATE_GO_AHEAD:
                turn_goahead_left_led();
                turn_goahead_right_led();
                
 //               clear_led();
                break;

            default:

                break;
        }
        Usart_SendStr(USART1,"distance.val=");
 //       Usart_SendByte(USART1,1+0x30);
//        printf("%d",distance);
        printf(str_distance);
        Usart_SendByte(USART1,0xff);
        Usart_SendByte(USART1,0xff);
        Usart_SendByte(USART1,0xff);
        PARSER_Reset(&parser);
    }
    
}
void PARSER_Reset(COMMAND_PARSER *parser)
{
	parser->status = STATE_WAIT_G;
}

void PARSER_RunPaser(COMMAND_PARSER *parser,int32_t data)
{
	switch(parser->status)
	{
		case STATE_WAIT_G:
		{
//            if(data=='G')
//            {
//                mBicycle.outer_status=STATE_OUTDOOR_MODE;
//                Usart_SendStr(USART1,"page outdoor_mode");
//                Usart_SendByte(USART1,0xff);
//                Usart_SendByte(USART1,0xff);
//                Usart_SendByte(USART1,0xff);
//            }
//			printf("1");
			if(data=='g') 
			{	
				parser->data_guide=0;
				parser->data_distance=0;
				parser->status =STATE_WAIT_GUIDE_DATA;
			}
			else
			{
				parser->status=STATE_WAIT_GUIDE_DATA;
			}
			break;
		}
			
		case STATE_WAIT_GUIDE_DATA:
		{
			if(data==' ')
			{
				parser->status=STATE_WAIT_D;
				break;
			}
//			printf("3");
			parser->data_guide=parser->data_guide*10+(data-'0');
			break;
		}
		
		case STATE_WAIT_D:
		{
//			printf("4");
			if(data=='d')
			{
				parser->status=STATE_WAIT_DISTANCE_DATA;
			}
			break;
		}
				
		case STATE_WAIT_DISTANCE_DATA:
		{
			if(data==' ')
			{
				parser->status=STATE_WAIT_NEWLINE;
				break;
			}
			parser->data_distance=parser->data_distance*10+(data-'0');
//			printf("6");
			break;
		}

		
		case STATE_WAIT_NEWLINE:
		{
//			printf("7");
			if(data=='\n')
			{
//				printf("8");
				parser->status=STATE_PARSE_FINISH_PENDING;
			}
//			else 
//			{
//				parser->status=STATE_WAIT_X;
//			}
			break;
		}
		
		case STATE_PARSE_FINISH_PENDING:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void OnRecieve(int32_t data)
{

	PARSER_RunPaser(&parser,data);
	
}

