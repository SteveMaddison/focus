#ifndef _VOLUME_H_
#define _VOLUME_H_

#ifndef _DEVICES_H_
#include "focus/devices.h"
#endif
#ifndef _DIRECTORY_H_
#include "focus/directory.h"
#endif
#ifndef _VOLUME_H_
#include "focus/volume.h"
#endif


struct volume_s {
	struct device_s *dev;
	char number;
	struct dir_entry_s *root;
	struct fs_s *fs;
	struct volume_s *next;
};

extern struct volume_s *volume_start;
extern int volume_init( void );
extern int volume_register( struct volume_s *volume );
extern struct volume_s *volume_find( const char *name, char number );

#endif /* _VOLUME_H_ */
