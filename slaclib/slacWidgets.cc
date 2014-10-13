#include "pv_factory.h"
#include "environment.str"
#include "edm.version"
#include "slacWidgets.str"
#include "freeze_button.h"
#include "mz_xygraph.h"
#include "slacBar.h"

typedef struct libRecTag {
  char *className;
  char *typeName;
  char *text;
} libRecType;

static int libRecIndex = 0;

static libRecType libRec[] = {
  { "activeFreezeButtonClass", global_str5, reg_str1 },
  { "mzxyGraphClass", global_str5, reg_str2 },
  { "activeSlacBarClass", global_str5, reg_str3 } 
};

#ifdef __cplusplus
extern "C" {
#endif

char *version ( void ) {

static char *v = VERSION;

  return v;

}

char *author ( void ) {

static char *a = "Sergei Chevtsov (chevtsov@slac.stanford.edu)";

  return a;

}

int firstRegRecord (
  char **className,
  char **typeName,
  char **text
) {

  *className = libRec[0].className;
  *typeName  = libRec[0].typeName;
  *text      = libRec[0].text;

  libRecIndex = 1;

  return 0;

}

int nextRegRecord (
  char **className,
  char **typeName,
  char **text
) {

int max = sizeof(libRec) / sizeof(libRec[0]);

  if ( libRecIndex >= max ) return -1; //no more 

  *className = libRec[libRecIndex].className;
  *typeName  = libRec[libRecIndex].typeName;
  *text      = libRec[libRecIndex].text;

  libRecIndex++;

  return 0;

}

void *create_activeFreezeButtonClassPtr ( void ) {

activeFreezeButtonClass *ptr;

  ptr = new activeFreezeButtonClass;
  return (void *) ptr;

}

void *clone_activeFreezeButtonClassPtr (
  void *_srcPtr )
{

activeFreezeButtonClass *ptr, *srcPtr;

  srcPtr = (activeFreezeButtonClass *) _srcPtr;

  ptr = new activeFreezeButtonClass( srcPtr );

  return (void *) ptr;

}

void *create_mzxyGraphClassPtr ( void ) {

mz_xyGraphClass *ptr;

  ptr = new mz_xyGraphClass;
  return (void *) ptr;

}

void *clone_mzxyGraphClassPtr (
  void *_srcPtr )
{

mz_xyGraphClass *ptr, *srcPtr;

  srcPtr = (mz_xyGraphClass *) _srcPtr;

  ptr = new mz_xyGraphClass( srcPtr );

  return (void *) ptr;

}

void *create_slacBarClassPtr ( void ) {

activeSlacBarClass *ptr;

  ptr = new activeSlacBarClass;
  return (void *) ptr;

}

void *clone_slacBarClassPtr (
  void *_srcPtr )
{

activeSlacBarClass *ptr, *srcPtr;

  srcPtr = (activeSlacBarClass *) _srcPtr;

  ptr = new activeSlacBarClass( srcPtr );

  return (void *) ptr;

}


#ifdef __cplusplus
}
#endif
