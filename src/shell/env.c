#include "focus/shell/env.h"
#include "focus/memory.h"
#include "focus/util.h"

struct env_var_s *env_start;

static struct env_var_s *env_find( char *name );
static int env_create( char *name, char *value );
static void env_remove( struct env_var_s *var );


int env_init( void )
{
	env_start = NULL;
	env_create( "PATH", "." );
	env_create( "PROMPT", "> " );
	return 0;
}

int env_set( char *name, char *value )
{
	struct env_var_s *var = env_find( name );
	if( var ) {
		if( strlen( var->value ) >= strlen( value ) ) {
			strcpy( var->value, value );
			return 0;
		}
		else {
			env_remove( var );
		}
	}

	return env_create( name, value );
}

char* env_get( char *name )
{
	struct env_var_s *var = env_find( name );

	if( var ) {
		return var->value;
	}
	else {
		return NULL;
	}
}

struct env_var_s *env_find( char *name )
{
	struct env_var_s *var = env_start;

	while( var ) {
		if( !strcmp( name, var->name ) )
			break;
		var = var->next;
	}
	return var;
}

static int env_create( char *name, char *value )
{
	size_t size;
	struct env_var_s *var;

	/* allocate memory block for struct, name, value and null chars */
	size = sizeof(struct env_var_s) + strlen(name) + strlen(value) + 2;
	var = kmalloc( size, 0 );

	if( var ) {
		var->name = (char*)var + sizeof(struct env_var_s);
		strcpy( var->name, name );
		var->value = (char*)var->name + strlen(var->name) + 1;
		var->value++;
		var->value = '\0';
		strcpy( var->value, value );
		var->next = env_start;
		env_start = var;
		return 0;
	}
	return -1;
}

static void env_remove( struct env_var_s *var )
{
	struct env_var_s *prev = env_start;

	if( var == env_start ) {
		env_start = var->next;
	}
	else {
		while( prev ) {
			if( prev->next == var )
				break;
			prev = prev->next;
		}
		prev->next = var->next;
	}
	kfree( var );
}
