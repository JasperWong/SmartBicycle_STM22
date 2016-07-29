#include "bicycle.h"

void freedom_mode(uint8_t rx)
{
    clear_led();
    switch (rx)
    {
		case '0':
			break;
		case '1':
			break;
		case '2':
			turn_left_led();
			break;
		case '3':
			turn_right_led();
			break;
		case '4':
			turn_leftfront_led();
			break;
		case '5':
			turn_rightfront_led();
			break;
		case '6':
			turn_leftback_led();
			break;
		case '7':
			turn_rightfront_led();

			break;
		case '8':


			break;
		case '9':
			turn_goahead_left_led();

			turn_goahead_right_led();

			break;

		default:

			break;

	}
}