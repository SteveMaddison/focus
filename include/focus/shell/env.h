#ifndef _SHELL_ENV_H_
#define _SHELL_ENV_H_

struct env_var_s {
	char *name;
	char *value;
	struct env_var_s *next;
};

extern struct env_var_s *env_start;

extern int env_init( void );
extern int env_set( char *name, char *value );
extern char *env_get( char *name );

#endif /* _SHELL_ENV_H_ */
