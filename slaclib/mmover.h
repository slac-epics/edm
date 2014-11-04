//  edm - extensible display manager
// file: mmover.h
// Software for the widget, multi-mover (mmover) is loosly based on edm slider.
// Multi is for multiple controls, and mover indicates that some quantity is
// being moved by this.  Specifically, I felt a need for this kind of control
// widget when designing a screen for a scope like device.  The mmover is used
// there to control the position of markers.  Likewise, I am planning to use
// this widget to control the vertical position of waveform traces on the
// screen representing a scope...
//------------------------------------------------------------------------------
#ifndef _mmover_h
#define _mmover_h 1

#include "ulBindings.h"
#include "act_grf.h"
#include "entry_form.h"
#include "utility.h"
#include "keypad.h"

#include "pv_factory.h"
#include "cvtFast.h"

#define MMV_MAJOR_VERSION 4
#define MMV_MINOR_VERSION 0
#define MMV_RELEASE 0
#define MMV_STATE_IDLE 1
#define MMV_STATE_MOVING 2
#define MMV_K_COLORMODE_STATIC 0
#define MMV_K_COLORMODE_ALARM 1
#define N_CTRL	2
#define ORIENT_SZ	16
#define ORIENT_HT	0
#define ORIENT_VL	1
#define ORIENT_HB	2
#define ORIENT_VR	3

#ifdef _mmover_cc
#include "mmover.str"

static void mmvEvHandl( Widget w,XtPointer client,XEvent *e,Boolean* flg);
static void updt1Ctrl( void* p,int ix);
static void mmvUpdateCtrl( XtPointer client,XtIntervalId *id);
static void oneCtrlPosition( void* p,double* fv,int ix);
static void decOneCtrl( void* p,int ix);
static void incOneCtrl( void* p,int ix);
static void mmvDecrement( XtPointer client,XtIntervalId *id);
static void mmvIncrement( XtPointer client,XtIntervalId *id);
static void mmvEditOk( Widget w,XtPointer client,XtPointer call);
static void mmvEditUpdt( Widget w,XtPointer client,XtPointer call);
static void mmvEditApply( Widget w,XtPointer client,XtPointer call);
static void mmvEditCancel( Widget w,XtPointer client,XtPointer call);
static void mmvEditCancelDel( Widget w,XtPointer client,XtPointer call);
static void mmvCtrlUpdate( ProcessVariable* pv,void* p,int ix);
static void mmvCtrl1Update( ProcessVariable *pv,void *userarg);
static void mmvCtrl2Update( ProcessVariable *pv,void *userarg);
static void mmvMonConState( ProcessVariable* pv,void* p,int ix);
static void mmvMonCtrl1ConState( ProcessVariable *pv,void *userarg);
static void mmvMonCtrl2ConState( ProcessVariable *pv,void *userarg);

#endif // _mmover_cc

class mmvClass:public activeGraphicClass{
private:
  friend void mmvEvHandl( Widget w,XtPointer client,XEvent* e,Boolean* flg);
  friend void updt1Ctrl( void* p,int ix);
  friend void oneCtrlPosition( void* p,double* fv,int ix);
  friend void decOneCtrl( void* p,int ix);
  friend void incOneCtrl( void* p,int ix);
  friend void mmvUpdateCtrl( XtPointer client,XtIntervalId *id);
  friend void mmvDecrement( XtPointer client,XtIntervalId *id);
  friend void mmvIncrement( XtPointer client,XtIntervalId *id);
  friend void mmvEditOk( Widget w,XtPointer client,XtPointer call);
  friend void mmvEditUpdt( Widget w,XtPointer client,XtPointer call);
  friend void mmvEditApply( Widget w,XtPointer client,XtPointer call);
  friend void mmvEditCancel( Widget w,XtPointer client,XtPointer call);
  friend void mmvEditCancelDel( Widget w,XtPointer client,XtPointer call);
  friend void mmvCtrlUpdate( ProcessVariable* pv,void* p,int ix);
  friend void mmvCtrl1Update( ProcessVariable* pv,void* userarg);
  friend void mmvCtrl2Update( ProcessVariable* pv,void* userarg);
  friend void mmvMonConState( ProcessVariable* pv,void* p,int ix);
  friend void mmvMonCtrl1ConState( ProcessVariable* pv,void* userarg);
  friend void mmvMonCtrl2ConState( ProcessVariable* pv,void* userarg);

  typedef struct{
    int colorMode;
    int color;
    colorButtonClass cb;
    char pvName[PV_Factory::MAX_PV_NAME+1];
  } ebufCtrl_t;

  typedef struct editBufTag{	// edit buffer
    int x,y,w,h;
    double increment,accelMultiplier;
    int bgColorMode;
    ebufCtrl_t ctrl[N_CTRL];
    int bgColor;
    int fgColor;
    colorButtonClass fgCb;
    colorButtonClass bgCb;
    int changeCbFlg;
    int activateCbFlg;
    int deactivateCbFlg;
    int limitsFromDb;
    efDouble efScaleMin;
    efDouble efScaleMax;
    efInt efPrecision;
    char orientStr[ORIENT_SZ];
  } editBufType, *editBufPtr;

  typedef struct{
    int		     x,y,w,h;
    double	     v,oldv,curv,onev;
    int		     state;
    int		     exists;
    int		     pvConnected;
    int		     needConInit;
    int		     needInfInit;
    int		     needRefresh;
    int		     oldStat,oldSev;
    pvColorClass     color;
    ProcessVariable* pvId;
    expStringClass   pvName;
  } ctrl_t;

  XtIntervalId updtCtrlTimer;
  int updtCtrlTimerValue;
  int updtCtrlTimerActive;
  XtIntervalId incrementTimer;
  int incrementTimerActive;
  int incrementTimerValue;
  int opComplete,minW,minH;

  ctrl_t ctrl[N_CTRL];
  int numCtrls;				// number of controls: 0, 1, or 2
  int ixDrag;				// index of control in drag
  editBufPtr eBuf;
  Widget frameWidget, mmvWidget;
  double minFv,maxFv,factor;
  double increment,accelMultiplier;
  int bgColorMode;
  pvColorClass bgColor,fgColor;
  int arcStart,arcStop;
  char orientStr[16];
  int  orientation;	// 0 - horizTop, 1 -  VertL, 2 - HorizBot, 3 - VertR
  int  horizontal;	// 1 - horizontal, 0 - vertical
  int  created;		// 0 - is being created, 1 - done created

  int bufInvalid,active,activeMode,init;
  int positive,needErase,needDraw;
  int initialConnection;

  VPFUNC changeCb,activateCb,deactivateCb;
  int changeCbFlg,activateCbFlg,deactivateCbFlg,anyCbFlg;
  int limitsFromDb;
  double scaleMin, scaleMax;
  efDouble efScaleMin, efScaleMax;
  int precision;
  efInt efPrecision;

public:
  mmvClass( void );
  mmvClass( const mmvClass *source);
  ~mmvClass( void){
    if(name) delete[] name;
    if(eBuf) delete eBuf;
  }
  char *objName( void){ return name;}
  void ifExists( int ix);
  void doIncrement( void);
  void doDecrement( void);
  int createInteractive( activeWindowClass *aw_obj,int x,int y,int w,int h);
  int save( FILE *f);
  int createFromFile( FILE *fptr,char *name,activeWindowClass *actWin);
  int genericEdit( void);
  int edit( void);
  int editCreate( void);
  int drawOne( int ix);
  int drawActiveOne( pvColorClass pvc,int ix);
  int draw( void);
  int erase( void);
  int drawActive( void);
  int eraseActive( void);
  void bufInvalidate( void);
  int activate( int pass,void *ptr);
  int deactivate( int pass);
  void updateDimensions ( void );
  int eraseActivePointers ( void );
  int drawActivePointers ( void );
  int expand1st( int numMacros,char *macros[],char *expansions[]);
  int expand2nd( int numMacros,char *macros[],char *expansions[]);
  void executeDeferred( void);
  int getProperty( char *prop,double *_value);
  void map( void);
  void unmap( void);
  void getPvs( int max,ProcessVariable *pvs[],int *n);
  char *crawlerGetFirstPv( void);
  char *crawlerGetNextPv( void);
};

#ifdef __cplusplus
extern "C" {
#endif

void *create_mmvClassPtr( void);
void *clone_mmvClassPtr( void*);

#ifdef __cplusplus
}
#endif

#endif
