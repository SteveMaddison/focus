#include "focus/util.h"
#include "focus/printf.h"
#include "focus/stdarg.h"
#include "focus/spectrum/video.h"

#define SIGNED		1
#define UNSIGNED	2

static void printn( int number, int base, char flags );

static const char digits[] = "0123456789abcdef";

int printf( const char* format, ... )
{
	va_list args;
	char cvalue;
	int ivalue;
	char* pvalue;

	va_start( args, format );

	while( *format ) {
		if ( *format == '%' ) {
			switch( *++format ) {
				case '%':
					print_char( '%' );
					break;

				case 'b':
					ivalue = va_arg( args, int );
					printn( ivalue, 2, UNSIGNED );
					break;

				case 'c':
					/* char is passed as int! */
					cvalue = va_arg( args, int );
					print_char( cvalue );
					break;

				case 'd':
					ivalue = va_arg( args, int );
					printn( ivalue, 10, SIGNED );
					break;

				case 's':
					pvalue = va_arg( args, char* );
					while( *pvalue )
						print_char( *pvalue++ );
					break;

				case 'u':
					ivalue = va_arg( args, int );
					printn( ivalue, 10, UNSIGNED );
					break;

				case 'x':
					ivalue = va_arg( args, int );
					printn( ivalue, 16, UNSIGNED );
					break;
			}
		}
		else {
			print_char( *format );
		}
		format++;
	}

	va_end( args );
	return 0;

}

static void printn( int number, int base, char flags )
{
	unsigned int magnitude;
	char buffer[33];
	char *p = &buffer[32];

	if( number == 0 ) {
		print_char( '0' );
		return;
	}

	if( flags &= SIGNED && number < 0 ) {
		print_char( '-' );
		magnitude = (unsigned int) -number;
	}
	else {
		magnitude = (unsigned int) number;
	}

	while( magnitude ) {
		*p-- = digits[ magnitude % base ];
		magnitude /= base;
	}
	p++;

	while( p <= &buffer[32] ) {
		print_char( *p++ );
	}
}
