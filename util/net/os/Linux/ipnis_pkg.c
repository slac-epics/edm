#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sys_types.h"
#include "ipnis_priv.h"
#include "iprpc.h"


int ipnis_get_service (
  int num_connect_fails,
  const char *generic_service_name,
  char *service_name,
  char *node_name,
  size_t maxName,
  int *mode
) {

static char *g_nodes[2] = { "134.167.20.89", "134.167.21.95" };
static int g_modes[] = { RPC_K_MASTER, RPC_K_LOCAL };
int i;

  if ( strncmp( generic_service_name, "IOSCNR", strlen("IOSCNR") ) == 0 ) {

    i = 0;

    Strncpy( service_name, "5200", maxName );
    Strncpy( node_name, "134.167.21.95", maxName );
    *mode = g_modes[i];

  }
  else if ( strncmp( generic_service_name, "TEST", strlen("TEST") ) == 0 ) {

    i = 0;

    Strncpy( service_name, "5200", maxName );
    Strncpy( node_name, "134.167.20.34", maxName );
    *mode = g_modes[i];

  }
  else {
    for ( i=0; i<=maxName; i++ ) {
      if ( generic_service_name[i] == 0 )
	  	break;
      service_name[i] = toupper(generic_service_name[i]);
    }
    service_name[i] = 0;

    i = 0;
    Strncpy( node_name, g_nodes[i], maxName );
    *mode = g_modes[i];

  }

  return IPNIS_SUCCESS;

}
