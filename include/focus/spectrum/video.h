#ifndef _SPECTRUM_VIDEO_H_
#define _SPECTRUM_VIDEO_H_

struct cursor_s {
	int	xpos;
	int	ypos;
};

/* These are the "raw" colour codes. They can be used for
 * borders as-is. For screen attributes, bitwise OR the
 * ink with paper << 3.
 */
#define BLACK			0x00
#define BLUE			0x01
#define RED				0x02
#define MAGENTA			0x03
#define GREEN			0x04
#define CYAN			0x05
#define YELLOW			0x06
#define WHITE			0x07

#define ATTR_BRIGHT		0x40
#define ATTR_FLASH		0x80

/* default attributes */
#define ATTR_DEFAULT	WHITE
#define BORDER_DEFAULT	BLACK

/* screen dimensions in characters */
#define SCR_COLS		32
#define SCR_ROWS		24

/* screen dimensions in pixels */
#define SCR_WIDTH		256
#define SCR_HEIGHT		192

extern int video_init( void );
extern void cls( void );
extern void set_border( const char colour );
extern void set_attrib( const char value );
extern char get_attrib( void );
extern void plot( const char x, const char y );
extern void print_string( const char* string );
extern void print_char( const char letter );
extern void scroll( void );
extern void get_cursor( struct cursor_s *cur );
extern void set_cursor( struct cursor_s *new_cur );

#endif /* _SPECTRUM_VIDEO_H_ */
