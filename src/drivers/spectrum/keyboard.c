#include "focus/interrupt.h"
#include "focus/spectrum/keyboard.h"
#include "focus/spectrum/sound.h"
#include "focus/util.h"
#include "focus/fifo.h"

static void kb_intr( void );
static struct intr_task_s kb_intr_task;

static char last_key;
static int last_key_time;
static char caps_lock;
struct fifo_s* kb_buffer;

/* These are statics to avoid having to reallocate
 * every time kb_scan() is called.
 */
static unsigned char kb_port, value, code, bit, half_row, shift;

int kb_init( void )
{
	last_key = 0xff;
	last_key_time = 0;
	caps_lock = 0;
    kb_buffer = fifo_create( KB_BUFFER_SIZE );

    kb_intr_task.interval = 0;
    kb_intr_task.function = kb_intr;
    intr_task_register( &kb_intr_task );

	return 0;
}


/* Look-up table for key code -> ASCII (40 keys * 3 shift modes) */
const char symbols[] = {
	/* Regular */
	0x00, 'a', 'q' , '1' , '0' , 'p' , '\n', ' ' ,
	'z' , 's', 'w' , '2' , '9' , 'o' , 'l' , 0x00,
	'x' , 'd', 'e' , '3' , '8' , 'i' , 'k' , 'm' ,
	'c' , 'f', 'r' , '4' , '7' , 'u' , 'j' , 'n' ,
	'v' , 'g', 't' , '5' , '6' , 'y' , 'h' , 'b' ,
	/* Shift */
	0x00, 'A', 'Q' , '`'     , '\b'     , 'P' , '\n', 0x03,
	'Z' , 'S', 'W' , 0x00    , 0x00     , 'O' , 'L' , 0x00,
	'X' , 'D', 'E' , KB_INS  , KB_RIGHT , 'I' , 'K' , 'M' ,
	'C' , 'F', 'R' , KB_DEL  , KB_UP    , 'U' , 'J' , 'N' ,
	'V' , 'G', 'T' , KB_LEFT , KB_DOWN  , 'Y' , 'H' , 'B' ,
	/* Symbol Shift */
	0x00   , '~',  KB_HOME , '!' , '_' , '"'  , KB_PGUP , KB_PGDN,
	':'    , '|',  KB_TAB  , '@' , ')' , ';'  , '='     , 0x00   ,
	KB_ESC , '\\', KB_END  , '#' , '(' , 0x00 , '+'     , '.'    ,
	'?'    , '{',  '<'     , '$' , '\'', ']'  , '-'     , ','    ,
	'/'    , '}',  '>'     , '%' , '&' , '['  , '^'     , '*'
};


char kb_scan( void ) {
	kb_port = 0xfe; /* start at port 0xfefe */
	shift = 0;
	code = 0;

	for( half_row = 0 ; half_row < 8 ; half_row++ ) {
		/* read the port and invert the value */
		value = inb_fe( kb_port );
		value = ~value & 0x1f;

		if( value ) {
			if( half_row == 0 && value & 0x01 ) {
				/* shift pressed */
				shift |= KB_SHIFT;
				value &= 0xfe;
			}
			else if( half_row == 7 && value & 0x02 ) {
				/* symbol shift pressed */
				shift |= KB_SYMBOL_SHIFT;
				value &= 0xfd;
			}

			if( !code && value ) {
				/* Find the first bit set - any others mean more than one
				 * non-shift key is pressed, which we just ignore.
				 */
				for( bit = 0 ; bit < 5 ; bit++ ) {
					if( value & 0x01 )
						break;
					value >>= 1;
				}
				/* convert to an array index 0-39 decimal and exit loop */
				code = (bit << 3) | half_row;
			}
		}

		/* set port for next read */
		kb_port <<= 1;
		kb_port |= 0x01;
	}

	if( shift == (KB_SHIFT | KB_SYMBOL_SHIFT) ) {
		if(	code == KB_ENTER ) {
			reset();
			/* keyboard reset combo */
		}
		else {
			/* symbol shift takes precedence */
			shift = KB_SYMBOL_SHIFT;
		}
	}

	/* apply the shift offset */
	code += (shift * KB_MAX_KEY);
	if( caps_lock ) {
		if( symbols[ code ] >= 'a' && symbols[ code ] <= 'z' ) {
			code += KB_MAX_KEY;
		}
		else if( symbols[ code ] >= 'A' && symbols[ code ] <= 'Z' ) {
			code -= KB_MAX_KEY;
		}
	}
	if ( code == 51 ) {
		caps_lock = ~caps_lock;
	}

	/* auto-repeat */
	if( code == 0 ) {
		last_key = 0xff;
		last_key_time = 200;
	}
	else if( code == last_key ) {
		if( last_key_time++ > 222 ) {
			last_key_time = 200;
			return symbols[ code ];
		}
	}
	else {
		last_key = code;
		last_key_time = 0;
		return symbols[ code ];
	}

	return 0;
}

static void kb_intr( void )
{
    char c;
    if( c = kb_scan() ) {
    	if( fifo_write( kb_buffer, &c, 1 ) != 1 ) {
    		/* can't write - probably full */
    		beep();
    	}
    }
}
