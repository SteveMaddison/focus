#ifndef _VFS_H_
#define _VFS_H_

#include "focus/devices.h"
#include "focus/volume.h"
#include "focus/fifo.h"

#define PATH_MAX_LEN		128

#define SYNC_ALL			0x01
#define SYNC_DATA			0x02


struct fhandle_s {
	struct io_func_s *functions;
	char			path[ PATH_MAX_LEN ];
	struct fifo_s	*fifo_in;
	struct fifo_s	*fifo_out;
};

struct fs_s {
	char		magic[4];
	int			serial_number;
	ssize_t		total_size;
	ssize_t		free_space;
	ssize_t		first_free_block;
	ssize_t		first_header;
	ssize_t		first_free_header;
	ssize_t		block_size;
	ssize_t		fat_entries;
	char		label[ MAX_LABEL_LEN ];
	struct io_func_s *functions;
};

extern int vfs_init( void );

extern int open( const char *path, int flags );
extern int close( int fd );
extern int read( int fd, void *buffer, int count );
extern int write( int fd, const void* buffer, int count );
extern int fsync( int fd );
extern int fdatasync( int fd );

#endif /* _VFS_H_ */

