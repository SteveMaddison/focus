#ifndef _DEVICES_H_
#define _DEVICES_H_

#define DEV_NAME_LEN	5

struct io_func_s {
	int 	(*open)(void*, int);
	int		(*close)(void*);
	int		(*read)(void*, char*, int);
	int		(*write)(void*, const char*, int);
	int 	(*sync)(void*, int);
};

struct device_s {
	int		id;
	char	name[ DEV_NAME_LEN ];
	struct io_func_s *functions;
	struct device_s *next;
};

extern struct device_s *device_start;
extern int device_init( void );
extern int device_register( struct device_s* dev );
extern struct device_s *device_find( const char *name );

#endif /* _DEVICES_H_ */
