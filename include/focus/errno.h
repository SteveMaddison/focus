#ifndef _ERRNO_H_
#define _ERRNO_H_


/* memory */
#define ENOMEM			20	/* out of memory */

/* filesystems */
#define ENOENT			30	/* no such file / directory */
#define EACCES			31	/* permission denied */
#define	EEXIST			32	/* file exists */
#define	ENOTDIR			33	/* not a directory */
#define	EISDIR			34	/* is a directory */
#define ENAMETOOLONG	35	/* file name too long */
#define EBADPATH		36	/* invalid path/device name */

/* devices */
#define ENODEV			40	/* no such device */
#define EDEVREG			41	/* device already registered */
#define	EIO				42	/* i/o eror */
#define	ENOTBLK			43	/* block device required */
#define	EBUSY			44	/* device / resource busy */


extern int errno;

extern const char* strerr( int error_code );

#endif /* _ERRNO_H_ */
