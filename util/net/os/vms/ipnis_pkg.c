#include stdlib
#include string
#include descrip
#include ssdef
#include lnmdef
#include syidef

#include sys_types
#include ipnis_priv
#include iprpc


int ipnis_get_service (
  int num_connect_fails,
  const char *generic_service_name,
  char *service_name,
  char *node_name,
  size_t maxName,
  int *mode
) {

static int init = 1;
static char nodes[2][31+1];
static int modes[] = { IPRPC_K_MASTER, IPRPC_K_LOCAL };
int stat, ret_stat, i, item;
unsigned short len;
struct dsc$descriptor dsc;
char *ptr, buf[127+1];

  if ( init ) {
    strcpy( nodes[0], "134.167.20.34" );
    strcpy( nodes[1], "134.167.20.34" );
    init = 0;
  }

  if ( strstr( generic_service_name, "::" ) ) {

    Strncpy( buf, generic_service_name, 127 );
    ptr = strtok( buf, "::" );
    Strncpy( node_name, ptr, maxName );
    ptr = strtok( NULL, "::" );
    Strncpy( service_name, ptr, maxName );

  }
  else if ( strncmp( generic_service_name, "IOSCNR", strlen("IOSCNR") ) == 0 ) {

    i = 0;

    Strncpy( service_name, "5100", maxName );
    Strncpy( node_name, nodes[i], maxName );
    *mode = modes[i];

  }
  else if ( strncmp( generic_service_name, "REM_CONSOLE", strlen("REM_CONSOLE") ) == 0 ) {

    i = 0;

    Strncpy( service_name, "5101", maxName );
    Strncpy( node_name, nodes[i], maxName );
    *mode = modes[i];

  }
  else if ( strncmp( generic_service_name,
    "DBSCANSRV", strlen("DBSCANSRV") ) == 0 ) {

    i = 0;

    Strncpy( service_name, "5102", maxName );
    Strncpy( node_name, nodes[i], maxName );
    *mode = modes[i];

  }
  else if ( strncmp( generic_service_name,
    "TEST", strlen("TEST") ) == 0 ) {

    i = 0;

    Strncpy( service_name, "5200", maxName );
    Strncpy( node_name, nodes[i], maxName );
    *mode = modes[i];

  }
  else {

    ret_stat = IPNIS_UNKSVC;
    goto err_return;

  }

  return IPNIS_SUCCESS;

err_return:

  strcpy( service_name, "" );
  strcpy( node_name, "" );
  *mode = 0;

  return ret_stat;

}
