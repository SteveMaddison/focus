#ifndef _COMMANDS_H_
#define _COMMANDS_H_

struct command_s {
	char	*name;
	char	*desc;
	char	*help;
	int		(*function)(int, char**);
	struct command_s *next;
};

extern const struct command_s *command_start( void );

#endif /* _COMMANDS_H_ */
