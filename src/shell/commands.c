#include "focus/shell.h"
#include "focus/shell/commands.h"
#include "focus/shell/env.h"
#include "focus/util.h"
#include "focus/memory.h"
#ifdef ARCH_SPECTRUM
#include "focus/spectrum/console.h"
#endif


static int cmd_cls ( int argc, char *argv[] );
static int cmd_echo( int argc, char *argv[] );
static int cmd_env ( int argc, char *argv[] );
static int cmd_help( int argc, char *argv[] );
static int cmd_mem ( int argc, char *argv[] );

static const struct command_s cs_mem = {
	"mem",
	"Print memory info",
	"Usage: mem",
	cmd_mem,
	NULL
};
static const struct command_s cs_help = {
	"help",
	"Get help",
	"Usage: help [command]",
	cmd_help,
	&cs_mem
};
static const struct command_s cs_env = {
	"env",
	"Environment operations",
	"Usage: env [name [value]]",
	cmd_env,
	&cs_help,
};
static const struct command_s cs_echo = {
	"echo",
	"Print parameters",
	"Usage: echo [param [..param]]",
	cmd_echo,
	&cs_env
};
static const struct command_s cs_cls = {
	"cls",
	"Clear the screen",
	"Usage: cls",
	cmd_cls,
	&cs_echo
};

const struct command_s *command_start( void )
{
	return &cs_cls;
}


/* Command: cls
 */
static int cmd_cls( int argc, char *argv[] )
{
	UNUSED(argc);
	UNUSED(argv);

	cls();
	return 0;
}


/* Command: echo
 */
int cmd_echo( int argc, char *argv[] )
{
	int i;

	for( i = 1 ; i < argc ; i++ ) {
		printf( "%s", argv[ i ] );
		if( i <= argc ) {
			printf(" ");
		}
	}
	printf("\n");
	return 0;
}


/* Command: env
 */
static int cmd_env( int argc, char *argv[] )
{
	struct env_var_s *var = env_start;
	char *value;

	switch( argc )
	{
		case 1:
			while( var ) {
				printf("%s=%s\n", var->name, var->value );
				var = var->next;
			}
			break;
		case 2:
			value = env_get( argv[1] );
			if( value ) {
				printf( "%s\n", value );
			}
			break;
		case 3:
			return env_set( argv[1], argv[2] );
			break;
		default:
			printf("Usage: env [name [value]]\n");
			return -1;
			break;
	}

	return 0;
}


/* Command: help
 */
static int cmd_help( int argc, char *argv[] )
{
	struct command_s *command = command_start();

	switch( argc ) {
		case 1:
			printf( "%s\n", cs_help.help );
			printf( "Available commands:\n" );
			while( command ) {
				printf("%s : %s\n", command->name, command->desc );
				command = command->next;
			}
			break;

		case 2:
			while( command ) {
				if( !strcmp( argv[1], command->name ) ) {
					printf("%s: %s\n%s\n", command->name, command->desc, command->help );
					break;
				}
				command = command->next;
			}
			break;

		default:
			printf( "%s\n", cs_help.help );
			break;
	}

	return 0;
}


/* Command: mem
*/
static int cmd_mem( int argc, char *argv[] )
{
	struct mem_stat_s *mstat;
	UNUSED(argc);
	UNUSED(argv);

	mstat = get_mem_stats();

	printf( "Used Free Total\n" );
	printf( "%u %u %u\n", mstat->total - mstat->free, mstat->free, mstat->total );

	return 0;
}


