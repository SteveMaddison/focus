#include "focus/shell.h"
#include "focus/shell/commands.h"
#include "focus/util.h"
#include "focus/memory.h"

int cmd_mem( void ) {
	struct mem_stat_s *mstat;

	mstat = get_mem_stats();

	printf( "Used Free Total\n" );
	printf( "%u %u %u\n", mstat->total - mstat->free, mstat->free, mstat->total );

	return 0;
}
