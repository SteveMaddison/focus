#include "focus/shell.h"
#include "focus/shell/commands.h"
#include "focus/shell/env.h"
#include "focus/util.h"
#include "focus/memory.h"


int cmd_echo( int argc, char *argv[] ) {
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

int cmd_env( int argc, char *argv[] ) {
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

int cmd_mem( void ) {
	struct mem_stat_s *mstat;

	mstat = get_mem_stats();

	printf( "Used Free Total\n" );
	printf( "%u %u %u\n", mstat->total - mstat->free, mstat->free, mstat->total );

	return 0;
}
