//  edm - extensible display manager
// file: mmover.cc
// for description, see the comment in mmover.h file.
//------------------------------------------------------------------------------
#define _mmover_cc 1

#include "mmover.h"
#include "app_pkg.h"
#include "act_win.h"
#include "thread.h"

static void oneCtrlPosition( void* p,double* fv,int ix){
//------------------------------------------------------------------------------
  mmvClass* mmv=(mmvClass*)p;
  if(mmv->positive){
    if(*fv<mmv->minFv) *fv=mmv->minFv;
    if(*fv>mmv->maxFv) *fv=mmv->maxFv;}
  else {
    if(*fv>mmv->minFv) *fv=mmv->minFv;
    if(*fv<mmv->maxFv) *fv=mmv->maxFv;}
  mmv->ctrl[ix].v=(*fv);
  if(mmv->horizontal){
    mmv->ctrl[ix].x=(int)((*fv-mmv->minFv)/mmv->factor+0.5)+mmv->h/2;}
  else{
    mmv->ctrl[ix].y=(int)((*fv-mmv->maxFv)/mmv->factor+0.5)+mmv->w/2;}
/*
printf( "oneCtrlPos: orien=%d,ix=%d,x=%d,y=%d,w=%d,h=%d\n",
mmv->orientation,ix,mmv->x,mmv->y,mmv->w,mmv->h);
printf( "oneCtrlPos: orien=%d,ix=%d,cx=%d,cy=%d,cw=%d,ch=%d\n",mmv->orientation,
ix,mmv->ctrl[ix].x,mmv->ctrl[ix].y,mmv->ctrl[ix].w,mmv->ctrl[ix].h);
printf( "oneCtrlPos: orien=%d,ix=%d,fact=%.2f,*fv=%.2f,ctrlx=%d,ctrly=%d\n",
mmv->orientation,ix,mmv->factor,*fv,mmv->ctrl[ix].x,mmv->ctrl[ix].y);
*/
}
static void updt1Ctrl( void* p, int ix){
//-----------------------------------------------------------------------------
  double fv; mmvClass* mmv=(mmvClass*)p;
  if(!mmv->ctrl[ix].exists) return;
  mmv->ctrl[ix].oldv = mmv->ctrl[ix].onev;
  mmv->eraseActivePointers();
  mmv->actWin->appCtx->proc->lock();
  mmv->ctrl[ix].v = mmv->ctrl[ix].onev = mmv->ctrl[ix].curv;
  mmv->actWin->appCtx->proc->unlock();
  fv=mmv->ctrl[ix].v;
  oneCtrlPosition( p,&fv,ix);
}
static void mmvUpdateCtrl( XtPointer client,XtIntervalId *id){
//------------------------------------------------------------------------------
  mmvClass* mmv=(mmvClass*)client;
  mmv->updtCtrlTimerActive=0;
  mmv->updtCtrlTimer=0;
  updt1Ctrl( mmv,0); updt1Ctrl( mmv,1);
  mmv->drawActivePointers();
  if(mmv->changeCb){ (*mmv->changeCb)( mmv);}
}
static void decOneCtrl( void* p,int ix){
//------------------------------------------------------------------------------
  double fvalue; int stat; mmvClass* mmv=(mmvClass*)p;
  if(mmv->incrementTimerValue<50){
    fvalue=mmv->ctrl[ix].v-mmv->increment*mmv->accelMultiplier;}
  else{ fvalue=mmv->ctrl[ix].v-mmv->increment;}
  oneCtrlPosition( p,&fvalue,ix);
  stat=mmv->drawActivePointers();
  mmv->actWin->appCtx->proc->lock();
  mmv->ctrl[ix].curv=mmv->ctrl[ix].v;
  mmv->actWin->appCtx->proc->unlock();
  if(mmv->ctrl[ix].exists){
    stat=mmv->ctrl[ix].pvId->put( fvalue);
    if(!stat) fprintf( stderr,mmvClStr1);
  }
}
static void mmvDecrement( XtPointer client,XtIntervalId *id){
//------------------------------------------------------------------------------
  mmvClass *mmv = (mmvClass *) client;

  if(!(mmv->incrementTimerActive)) return;
  if(mmv->incrementTimerValue > 50) mmv->incrementTimerValue -= 5;
  if(mmv->incrementTimerValue < 45) mmv->incrementTimerValue = 45;
  mmv->incrementTimer = appAddTimeOut( mmv->actWin->appCtx->appContext(),
	mmv->incrementTimerValue, mmvDecrement, client);
  mmv->eraseActivePointers();
  decOneCtrl( mmv,0);
  decOneCtrl( mmv,1);
  if(!mmv->ctrl[0].exists&&!mmv->ctrl[1].exists&&mmv->anyCbFlg){
    mmv->ctrl[0].needRefresh=1;
    mmv->ctrl[1].needRefresh=1;
    mmv->actWin->appCtx->proc->lock();
    mmv->actWin->addDefExeNode( mmv->aglPtr);
    mmv->actWin->appCtx->proc->unlock();
  }
  if(mmv->changeCb){ (*mmv->changeCb)( mmv);}
}
static void incOneCtrl( void* p,int ix){
//------------------------------------------------------------------------------
  double fvalue; int stat; mmvClass* mmv=(mmvClass*)p;
  if(mmv->incrementTimerValue<50){
    fvalue=mmv->ctrl[ix].v+mmv->increment*mmv->accelMultiplier;
  }
  else{ fvalue=mmv->ctrl[ix].v+mmv->increment;}
  oneCtrlPosition( p,&fvalue,ix);
  stat=mmv->drawActivePointers();
  mmv->actWin->appCtx->proc->lock();
  mmv->ctrl[ix].curv=mmv->ctrl[ix].v;
  mmv->actWin->appCtx->proc->unlock();
  if(mmv->ctrl[ix].exists) {
    stat=mmv->ctrl[ix].pvId->put( fvalue);
    if(!stat) fprintf( stderr,mmvClStr1);
  }
}
static void mmvIncrement( XtPointer client,XtIntervalId *id){
//------------------------------------------------------------------------------
  mmvClass *mmv = (mmvClass *) client;

  if(!(mmv->incrementTimerActive)) return;
  if(mmv->incrementTimerValue>50) mmv->incrementTimerValue-=5;
  if(mmv->incrementTimerValue<45) mmv->incrementTimerValue=45;
  mmv->incrementTimer = appAddTimeOut( mmv->actWin->appCtx->appContext(),
	mmv->incrementTimerValue, mmvIncrement, client);
  mmv->eraseActivePointers();
  incOneCtrl( mmv,0);
  incOneCtrl( mmv,1);
  if(!mmv->ctrl[0].exists&&!mmv->ctrl[1].exists&&mmv->anyCbFlg){
    mmv->ctrl[0].needRefresh=1;
    mmv->ctrl[1].needRefresh=1;
    mmv->actWin->appCtx->proc->lock();
    mmv->actWin->addDefExeNode( mmv->aglPtr);
    mmv->actWin->appCtx->proc->unlock();
  }
  if(mmv->changeCb){ (*mmv->changeCb)( mmv);}
}
static int findName( char* name,char* list){
//------------------------------------------------------------------------------
  int i=0,l; char st[32]; char* p1=list; char* p2=list;
  if(strlen(name)<=0) return(-1);
  while(p1){
    p2=strchr( p1,'|');
    if(!p2) l=strlen(p1); else l=p2-p1;
    if(l>=32) return(-1);
    strncpy( st,p1,l); st[l]=0;
    if(!strcmp( name,st)) break;
    if(p2) p1=p2+1; else p1=p2;
    i++;
  }
  if(!p1) i= -1;
  return(i);
}
static void mmvEditUpdt( Widget w,XtPointer client,XtPointer call){
//------------------------------------------------------------------------------
  mmvClass* mmv=(mmvClass*)client;
  mmv->actWin->setChanged();
  mmv->eraseSelectBoxCorners();
  mmv->erase();
  mmv->fgColor.setColorIndex( mmv->eBuf->fgColor,mmv->actWin->ci);
  mmv->bgColor.setColorIndex( mmv->eBuf->bgColor,mmv->actWin->ci);
  mmv->ctrl[0].color.setColorIndex( mmv->eBuf->ctrl[0].color,mmv->actWin->ci);
  mmv->ctrl[1].color.setColorIndex( mmv->eBuf->ctrl[1].color,mmv->actWin->ci);
  mmv->bgColorMode=mmv->eBuf->bgColorMode;
  if(mmv->bgColorMode==MMV_K_COLORMODE_ALARM) mmv->bgColor.setAlarmSensitive();
  else mmv->bgColor.setAlarmInsensitive();
  mmv->increment=mmv->eBuf->increment;
  mmv->accelMultiplier=mmv->eBuf->accelMultiplier;
  mmv->ctrl[0].pvName.setRaw( mmv->eBuf->ctrl[0].pvName);
  mmv->ctrl[1].pvName.setRaw( mmv->eBuf->ctrl[1].pvName);
  mmv->limitsFromDb=mmv->eBuf->limitsFromDb;
  mmv->efPrecision=mmv->eBuf->efPrecision;
  mmv->efScaleMin=mmv->eBuf->efScaleMin;
  mmv->efScaleMax=mmv->eBuf->efScaleMax;
  mmv->minFv=mmv->scaleMin=mmv->efScaleMin.value();
  mmv->maxFv=mmv->scaleMax=mmv->efScaleMax.value();
  if(mmv->efPrecision.isNull()) mmv->precision=1;
  else mmv->precision=mmv->efPrecision.value();
  strncpy( mmv->orientStr,mmv->eBuf->orientStr,ORIENT_SZ-1);
  mmv->orientStr[ORIENT_SZ-1]=0;
  mmv->orientation=findName( mmv->orientStr,mmvClStr3);
  mmv->horizontal=(mmv->orientation==ORIENT_HT||mmv->orientation==ORIENT_HB);
  strncpy( mmv->id,mmv->bufId,31);
  mmv->changeCbFlg=mmv->eBuf->changeCbFlg;
  mmv->activateCbFlg=mmv->eBuf->activateCbFlg;
  mmv->deactivateCbFlg=mmv->eBuf->deactivateCbFlg;
  mmv->anyCbFlg=mmv->changeCbFlg||mmv->activateCbFlg||mmv->deactivateCbFlg;
  mmv->ifExists( 0); mmv->ifExists( 1);
  mmv->x=mmv->eBuf->x; mmv->y=mmv->eBuf->y;
  mmv->w=mmv->eBuf->w; mmv->h=mmv->eBuf->h;
  if(mmv->horizontal){
    mmv->ctrl[0].h=mmv->ctrl[1].h=mmv->h;
    mmv->ctrl[0].w=mmv->ctrl[1].w=mmv->w-mmv->h;
    mmv->ctrl[0].x=mmv->w/4; mmv->ctrl[1].x=mmv->w*3/4;
    mmv->ctrl[0].y=mmv->ctrl[1].y=0;
  }
  else{
    mmv->ctrl[0].h=mmv->ctrl[1].h=mmv->h-mmv->w;
    mmv->ctrl[0].w=mmv->ctrl[1].w=mmv->w;
    mmv->ctrl[0].x=mmv->ctrl[1].x=0;
    mmv->ctrl[0].y=mmv->h/4; mmv->ctrl[1].y=3*mmv->h/4;
  }
  mmv->created=1;
  mmv->updateDimensions();
  if(mmv->w<mmv->minW) mmv->w=mmv->minW;
  if(mmv->h<mmv->minH) mmv->h=mmv->minH;
}
static void mmvEditApply( Widget w,XtPointer client,XtPointer call){
//------------------------------------------------------------------------------
  mmvClass* mmv=(mmvClass*)client;
  mmvEditUpdt ( w,client,call);
  mmv->refresh( mmv);
}
static void mmvEditOk( Widget w,XtPointer client,XtPointer call){
//------------------------------------------------------------------------------
  mmvClass* mmv=(mmvClass*)client;
  mmvEditUpdt ( w,client,call);
  mmv->ef.popdown();
  mmv->operationComplete();
}
static void mmvEditCancel( Widget w,XtPointer client,XtPointer call){
//------------------------------------------------------------------------------
  mmvClass* mmv=(mmvClass*)client;
  mmv->ef.popdown();
  mmv->operationCancel();
}
static void mmvEditCancelDel( Widget w,XtPointer client,XtPointer call){
//------------------------------------------------------------------------------
  mmvClass* mmv=(mmvClass*)client;
  mmv->ef.popdown();
  mmv->operationCancel();
  mmv->erase();
  mmv->deleteRequest=1;
  mmv->drawAll();
}
static void mmvMonConState( ProcessVariable* pv,void* p,int ix){
//------------------------------------------------------------------------------
  mmvClass* mmv=(mmvClass*)p;
  if(pv->is_valid()){ mmv->ctrl[ix].needConInit=1;}
  else{
    mmv->ctrl[ix].pvConnected=0;
    mmv->active=0;
    mmv->fgColor.setDisconnected();
    mmv->ctrl[ix].color.setDisconnected();
    mmv->bufInvalidate();
    mmv->needErase=1;
    mmv->needDraw=1;
  }
  mmv->actWin->appCtx->proc->lock();
  mmv->actWin->addDefExeNode( mmv->aglPtr);
  mmv->actWin->appCtx->proc->unlock();
}
static void mmvMonCtrl1ConState( ProcessVariable* pv,void* userarg){
//------------------------------------------------------------------------------
  mmvMonConState( pv,userarg,0);
}
static void mmvMonCtrl2ConState( ProcessVariable* pv,void* userarg){
//------------------------------------------------------------------------------
  mmvMonConState( pv,userarg,1);
}
static void mmvCtrlUpdate( ProcessVariable* pv,void* p,int ix){
//------------------------------------------------------------------------------
  mmvClass* mmv=(mmvClass*)p; int st,sev;

  st=pv->get_status();
  sev=pv->get_severity();
  if((st!=mmv->ctrl[ix].oldStat)||(sev!=mmv->ctrl[ix].oldSev)){
    mmv->ctrl[ix].oldStat=st;
    mmv->ctrl[ix].oldSev=sev;
    mmv->bgColor.setStatus( st,sev);
    mmv->ctrl[ix].color.setStatus( st,sev);
    mmv->bufInvalidate();
    mmv->needDraw=1;
    mmv->actWin->appCtx->proc->lock();
    mmv->actWin->addDefExeNode( mmv->aglPtr);
    mmv->actWin->appCtx->proc->unlock();
  }
  mmv->ctrl[ix].onev=pv->get_double(); // xtimer updates widget indicator
  mmv->ctrl[ix].curv=mmv->ctrl[ix].onev;
  if(!mmv->updtCtrlTimerActive){
    mmv->updtCtrlTimerActive=1;
    mmv->updtCtrlTimerValue=100;
    mmv->updtCtrlTimer=appAddTimeOut( mmv->actWin->appCtx->appContext(),
	mmv->updtCtrlTimerValue,mmvUpdateCtrl,(void*)mmv);
  }
}
static void mmvCtrl1Update( ProcessVariable *pv,void *userarg){
//------------------------------------------------------------------------------
  mmvCtrlUpdate( pv,userarg,0);
}
static void mmvCtrl2Update( ProcessVariable *pv,void *userarg){
//------------------------------------------------------------------------------
  mmvCtrlUpdate( pv,userarg,1);
}
mmvClass::mmvClass( void){
//------------------------------------------------------------------------------
  name = new char[strlen("mmvClass")+1];
  strcpy( name, "mmvClass");
  numCtrls=0; deleteRequest=0;
  selected=0; positive=1;
  orientStr[0]=0; strcpy( id, "");
  changeCbFlg=activateCbFlg=deactivateCbFlg=anyCbFlg=0;
  changeCb=NULL; activateCb=NULL; deactivateCb=NULL;
  scaleMin=0; scaleMax=10; minW=minH=0;
  limitsFromDb = 1;
  efScaleMin.setNull(1);
  efScaleMax.setNull(1);
  efPrecision.setNull(1);
  precision = 1;
  frameWidget = NULL;
  eBuf = NULL;
}
// copy constructor
mmvClass::mmvClass( const mmvClass *source){
//------------------------------------------------------------------------------
  int xOfs; activeGraphicClass *mmv = (activeGraphicClass *) this;

  mmv->clone( (activeGraphicClass *) source);
  name = new char[strlen("mmvClass")+1];
  strcpy( name, "mmvClass");
  deleteRequest=0;
//  numCtrls=source->numCtrls;
  bgColor.copy( source->bgColor);
  fgColor.copy( source->fgColor);
  bgColorMode = source->bgColorMode;
  ctrl[0].color.copy( source->ctrl[0].color);
  ctrl[1].color.copy( source->ctrl[1].color);
  ctrl[0].pvName.copy( source->ctrl[0].pvName);
  ctrl[1].pvName.copy( source->ctrl[1].pvName);
  created=1;
  updateDimensions();
  xOfs=(w-ctrl[0].w)/2; ctrl[0].x=xOfs;
  xOfs=(w-ctrl[1].w)/2; ctrl[1].x=xOfs;
  arcStart = 90*64 - 30*64;
  arcStop = 60*64;
  strncpy( orientStr,source->orientStr,ORIENT_SZ-1); orientStr[ORIENT_SZ-1]=0;
  orientation=source->orientation;
  horizontal=source->horizontal;
  increment = source->increment;
  accelMultiplier = source->accelMultiplier;
  positive = source->positive;
  strcpy( id, source->id);
  changeCbFlg = source->changeCbFlg;
  activateCbFlg = source->activateCbFlg;
  deactivateCbFlg = source->deactivateCbFlg;
  anyCbFlg=changeCbFlg||activateCbFlg||deactivateCbFlg;
  changeCb=NULL; activateCb=NULL; deactivateCb=NULL;
  limitsFromDb = source->limitsFromDb;
  scaleMin = source->scaleMin;
  scaleMax = source->scaleMax;
  precision = source->precision;
  efScaleMin = source->efScaleMin;
  efScaleMax = source->efScaleMax;
  efPrecision = source->efPrecision;
  frameWidget = NULL;
  eBuf = NULL;
  ifExists( 0); ifExists( 1);
}
void mmvClass::ifExists( int ix){
//------------------------------------------------------------------------------
  if(!ctrl[ix].pvName.getExpanded()||
	blankOrComment( ctrl[ix].pvName.getExpanded())) ctrl[ix].exists=0;
  else{ ctrl[ix].exists=1; ctrl[ix].color.setConnectSensitive(); numCtrls++;}
}
void mmvClass::doIncrement( void){
//------------------------------------------------------------------------------
  double fv; int stat;
/*printf( "doIncrement: numCtrls=%d\n",numCtrls);*/
  if(numCtrls!=1) return;
  eraseActivePointers();
  fv=ctrl[0].v+increment;
  oneCtrlPosition( this,&fv,0);
  stat=drawActivePointers();
  actWin->appCtx->proc->lock();
  ctrl[0].curv=ctrl[0].v;
  actWin->appCtx->proc->unlock();
  if(ctrl[0].exists){
    stat=ctrl[0].pvId->put( fv);
    if(!stat) fprintf( stderr, mmvClStr1);
  }
  else if(anyCbFlg){
    ctrl[0].needRefresh=1;
    ctrl[1].needRefresh=1;
    actWin->appCtx->proc->lock();
    actWin->addDefExeNode( aglPtr);
    actWin->appCtx->proc->unlock();
  }
  if(changeCb) (*changeCb)( this);
}
void mmvClass::doDecrement( void){
//------------------------------------------------------------------------------
  double fv; int stat;

  if(numCtrls!=1) return;
  eraseActivePointers();
  fv=ctrl[0].v-increment;
  oneCtrlPosition( this,&fv,0);
  stat=drawActivePointers();
  actWin->appCtx->proc->lock();
  ctrl[0].curv=ctrl[0].v;
  actWin->appCtx->proc->unlock();
  if(ctrl[0].exists){
    stat=ctrl[0].pvId->put( fv);
    if(!stat) fprintf( stderr,mmvClStr1);
  }
  else if(anyCbFlg){
    ctrl[0].needRefresh=1;
    ctrl[1].needRefresh=1;
    actWin->appCtx->proc->lock();
    actWin->addDefExeNode( aglPtr);
    actWin->appCtx->proc->unlock();
  }
  if(changeCb) (*changeCb)(this);
}
int mmvClass::createInteractive( activeWindowClass *aw_obj,
		int _x,int _y,int _w,int _h){
//------------------------------------------------------------------------------
  actWin=aw_obj; xOrigin=0; yOrigin=0; x=_x; y=_y; w=_w; h=_h;
  created=0;
  increment=0.0;
  accelMultiplier=1.0;
  strcpy( orientStr,"HorizTop");
  arcStart=90*64-30*64;
  arcStop=60*64;
  fgColor.setColorIndex( actWin->defaultTextFgColor, actWin->ci);
  bgColor.setColorIndex( actWin->defaultBgColor, actWin->ci);
  ctrl[0].color.setColorIndex( actWin->defaultFg1Color, actWin->ci);
  ctrl[1].color.setColorIndex( actWin->defaultFg1Color, actWin->ci);
  bgColorMode=0;
  this->draw();
  this->editCreate();
  return 1;
}
int mmvClass::save( FILE *f){
//------------------------------------------------------------------------------
  int stat, major, minor, release;
  tagClass tag;
  int zero = 0; double dzero = 0; char *emptyStr = "";

  major = MMV_MAJOR_VERSION;
  minor = MMV_MINOR_VERSION;
  release = MMV_RELEASE;
  tag.init();
  tag.loadW( "beginObjectProperties");
  tag.loadW( "major", &major);
  tag.loadW( "minor", &minor);
  tag.loadW( "release", &release);
  tag.loadW( "x", &x);
  tag.loadW( "y", &y);
  tag.loadW( "w", &w);
  tag.loadW( "h", &h);
  tag.loadW( "fgColor", actWin->ci, &fgColor);
  tag.loadW( "bgColor", actWin->ci, &bgColor);
  tag.loadW( "bgAlarm", &bgColorMode, &zero);
  tag.loadW( "ctrl1Color", actWin->ci, &ctrl[0].color);
  tag.loadW( "ctrl1Pv", &ctrl[0].pvName, emptyStr);
  tag.loadW( "ctrl2Color", actWin->ci, &ctrl[1].color);
  tag.loadW( "ctrl2Pv", &ctrl[1].pvName, emptyStr);
  tag.loadW( "orientStr",(char*)&orientStr,emptyStr);
  tag.loadW( "orientation",&orientation);
  tag.loadW( "increment", &increment, &dzero);
  tag.loadW( "incMultiplier", &accelMultiplier, &dzero);
  tag.loadBoolW( "limitsFromDb", &limitsFromDb, &zero);
  tag.loadW( "precision", &efPrecision);
  tag.loadW( "scaleMin", &efScaleMin);
  tag.loadW( "scaleMax", &efScaleMax);
  tag.loadW( unknownTags);
  tag.loadW( "endObjectProperties");
  tag.loadW( "");
  stat = tag.writeTags( f);
  return stat;
}
int mmvClass::createFromFile( FILE *f,char *name,activeWindowClass *_actWin){
//------------------------------------------------------------------------------
  int major,minor,release,stat; tagClass tag;
  int zero=0; double dzero=0; char *emptyStr="";

  actWin = _actWin;
  tag.init();
  tag.loadR( "beginObjectProperties");
  tag.loadR( unknownTags);
  tag.loadR( "major", &major);
  tag.loadR( "minor", &minor);
  tag.loadR( "release", &release);
  tag.loadR( "x", &x);
  tag.loadR( "y", &y);
  tag.loadR( "w", &w);
  tag.loadR( "h", &h);
  tag.loadR( "fgColor", actWin->ci, &fgColor);
  tag.loadR( "bgColor", actWin->ci, &bgColor);
  tag.loadR( "bgAlarm", &bgColorMode, &zero);
  tag.loadR( "ctrl1Color", actWin->ci, &ctrl[0].color);
  tag.loadR( "ctrl1Pv", &ctrl[0].pvName, emptyStr);
  tag.loadR( "ctrl2Color", actWin->ci, &ctrl[1].color);
  tag.loadR( "ctrl2Pv", &ctrl[1].pvName, emptyStr);
  tag.loadR( "orientStr",ORIENT_SZ-1,(char*)&orientStr);
  tag.loadR( "orientation",&orientation);
  tag.loadR( "increment", &increment, &dzero);
  tag.loadR( "incMultiplier", &accelMultiplier, &dzero);
  tag.loadR( "limitsFromDb", &limitsFromDb, &zero);
  tag.loadR( "precision", &efPrecision);
  tag.loadR( "scaleMin", &efScaleMin);
  tag.loadR( "scaleMax", &efScaleMax);
  tag.loadR( "endObjectProperties");

  stat=tag.readTags( f,"endObjectProperties");
  if(!(stat&1)){ actWin->appCtx->postMessage( tag.errMsg());}
  if(major>MMV_MAJOR_VERSION){ postIncompatable(); return 0;}
  if(major<4){ postIncompatable(); return 0;}

  this->initSelectBox(); // call after getting x,y,w,h

  strcpy( this->id, "");
  changeCbFlg = 0;
  activateCbFlg = 0;
  deactivateCbFlg = 0;
  anyCbFlg = 0;

  horizontal=(orientation==ORIENT_HT||orientation==ORIENT_HB);
  if(limitsFromDb||efPrecision.isNull()) precision=1;
  else precision=efPrecision.value();
  if((limitsFromDb||efScaleMin.isNull())&&(limitsFromDb||efScaleMax.isNull())){
    minFv=scaleMin=0; maxFv=scaleMax=10;}
  else{ minFv=scaleMin=efScaleMin.value(); maxFv=scaleMax=efScaleMax.value();}
  created=1;
  updateDimensions();
  if(horizontal){ ctrl[0].x=w/4; ctrl[1].x=3*w/4;}
  else{ ctrl[0].y=h/4; ctrl[1].y=3*h/4;}
  arcStart=90*64-30*64; arcStop=60*64;
  ctrl[0].curv=ctrl[0].onev=ctrl[0].v=0.0;
  ctrl[1].curv=ctrl[1].onev=ctrl[1].v=0.0;
  ifExists( 0); ifExists( 1);
  if(bgColorMode==MMV_K_COLORMODE_ALARM) bgColor.setAlarmSensitive();
  else bgColor.setAlarmInsensitive();
  return stat;
}
int mmvClass::genericEdit(void){
//------------------------------------------------------------------------------
  char title[32], *ptr;

  if(!eBuf){ eBuf=new editBufType;}
  ptr=actWin->obj.getNameFromClass( "mmvClass");
  if(ptr) strncpy( title,ptr,31);
  else strncpy( title,mmvClStr4,31);
  Strncat( title, mmvClStr5,31);
  strncpy( bufId,id,31);
  eBuf->x=x; eBuf->y=y; eBuf->w=w; eBuf->h=h;
  eBuf->fgColor=fgColor.pixelIndex();
  eBuf->bgColor=bgColor.pixelIndex();
  eBuf->ctrl[0].color=ctrl[0].color.pixelIndex();
  eBuf->ctrl[1].color=ctrl[1].color.pixelIndex();
  eBuf->bgColorMode=bgColorMode;
  eBuf->increment=increment;
  eBuf->accelMultiplier=accelMultiplier;
  eBuf->changeCbFlg=changeCbFlg;
  eBuf->activateCbFlg=activateCbFlg;
  eBuf->deactivateCbFlg=deactivateCbFlg;
  strncpy( eBuf->orientStr,orientStr,15); eBuf->orientStr[15]=0;
  if(ctrl[0].pvName.getRaw()) strncpy( eBuf->ctrl[0].pvName,
		ctrl[0].pvName.getRaw(),PV_Factory::MAX_PV_NAME);
  else strncpy( eBuf->ctrl[0].pvName,"",39);
  if(ctrl[1].pvName.getRaw()) strncpy( eBuf->ctrl[1].pvName,
		ctrl[1].pvName.getRaw(),PV_Factory::MAX_PV_NAME);
  else strncpy( eBuf->ctrl[1].pvName,"",39);
  eBuf->limitsFromDb=limitsFromDb;
  eBuf->efPrecision=efPrecision;
  eBuf->efScaleMin=efScaleMin;
  eBuf->efScaleMax=efScaleMax;
  ef.create( actWin->top,actWin->appCtx->ci.getColorMap(),
    &actWin->appCtx->entryFormX,&actWin->appCtx->entryFormY,
    &actWin->appCtx->entryFormW,&actWin->appCtx->entryFormH,
    &actWin->appCtx->largestH,title,NULL,NULL,NULL);

  ef.addTextField( mmvClStr7, 35,&eBuf->x);
  ef.addTextField( mmvClStr8, 35,&eBuf->y);
  ef.addTextField( mmvClStr9, 35,&eBuf->w);
  ef.addTextField( mmvClStr10,35,&eBuf->h);
  ef.addTextField( mmvClStr19,35,eBuf->ctrl[0].pvName,
	PV_Factory::MAX_PV_NAME);
  ef.addTextField( mmvClStr20,35,eBuf->ctrl[1].pvName,
	PV_Factory::MAX_PV_NAME);
  ef.addOption( mmvClStr2,mmvClStr3,eBuf->orientStr,ORIENT_SZ-1);
  ef.addTextField( mmvClStr15,35,&eBuf->increment);
  ef.addTextField( mmvClStr30,35,&eBuf->accelMultiplier);
  ef.addToggle( mmvClStr16, &eBuf->limitsFromDb);
  ef.addTextField( mmvClStr17,35,&eBuf->efScaleMin);
  ef.addTextField( mmvClStr18,35,&eBuf->efScaleMax);
  ef.addColorButton( mmvClStr21,actWin->ci,&eBuf->ctrl[0].cb,
	&eBuf->ctrl[0].color);
  ef.addColorButton( mmvClStr13,actWin->ci,&eBuf->ctrl[1].cb,
	&eBuf->ctrl[1].color);
  ef.addColorButton( mmvClStr12,actWin->ci,&eBuf->fgCb,&eBuf->fgColor);
  ef.addColorButton( mmvClStr32,actWin->ci,&eBuf->bgCb,&eBuf->bgColor);
  return 1;
}
int mmvClass::editCreate(void){
//------------------------------------------------------------------------------
  this->genericEdit();
  ef.finished( mmvEditOk, mmvEditApply, mmvEditCancelDel, this);
  actWin->currentEf = NULL;
  ef.popup();
  return 1;
}
int mmvClass::edit(void){
//------------------------------------------------------------------------------
  this->genericEdit();
  ef.finished( mmvEditOk, mmvEditApply, mmvEditCancel, this);
  actWin->currentEf = &ef;
  ef.popup();
  return 1;
}
int mmvClass::erase(void){
//------------------------------------------------------------------------------
  if(deleteRequest) return 1;
  XDrawRectangle( actWin->d, XtWindow(actWin->drawWidget),
	actWin->drawGc.eraseGC(), x, y, w, h);
  XFillRectangle( actWin->d, XtWindow(actWin->drawWidget),
	actWin->drawGc.eraseGC(), x, y, w, h);
  return 1;
}
int mmvClass::eraseActive(void){
//------------------------------------------------------------------------------
  if(!enabled||!activeMode||!init) return 1;
  XDrawRectangle( actWin->d, XtWindow(mmvWidget),
	actWin->executeGc.eraseGC(), 0, 0, w, h);
  XFillRectangle( actWin->d, XtWindow(mmvWidget),
	actWin->executeGc.eraseGC(), 0, 0, w, h);
  return 1;
}
int mmvClass::drawOne( int ix){
//------------------------------------------------------------------------------
  int ax,ay,aw,ah,ofs,arc1;
  if(!ctrl[ix].exists&&!anyCbFlg) return(1);
  actWin->drawGc.setFG( ctrl[ix].color.pixelColor());
  actWin->drawGc.setArcModePieSlice();
  switch(orientation){
    case ORIENT_HT:	ofs=ctrl[ix].w/4;
			ay=y+ctrl[ix].y;
			ax=x+ofs+ofs*ix-ctrl[ix].h;
			aw=ah=ctrl[ix].h*2; break;
    case ORIENT_VL:
			ofs=ctrl[ix].h/4;
			ay=y+ofs+ofs*ix+ctrl[ix].w;
			ax=x+ctrl[ix].x;
			aw=ah=ctrl[ix].w*2; break;
    case ORIENT_HB:
			ofs=ctrl[ix].w/4;
			aw=ah=ctrl[ix].h*2;
			ay=y+ctrl[ix].y-ctrl[ix].h;
			ax=x+ofs+ofs*ix-ctrl[ix].h; break;
    case ORIENT_VR:
			ofs=ctrl[ix].h/4;
			aw=ah=ctrl[ix].w*2;
			ay=y+ofs+ofs*ix+ctrl[ix].w;
			ax=x+ctrl[ix].x-ctrl[ix].w; break;
  }
  arc1=arcStart+90*64*orientation;
/*
printf( "drawOne: orient=%d,ofs=%d,cx=%d,cy=%d,cw=%d,ch=%d\n",
orientation,ofs,ctrl[ix].x,ctrl[ix].y,ctrl[ix].w,ctrl[ix].h);
printf( "drawOne: ax=%d,ay=%d,aw=%d,ah=%d,arc1=%d\n",ax,ay,aw,ah,arc1);
*/
  XFillArc( actWin->d,XtWindow(actWin->drawWidget),actWin->drawGc.normGC(),
	ax,ay,aw,ah,arc1,arcStop);
  return 1;
}
int mmvClass::drawActiveOne( pvColorClass pvc,int ix){
//------------------------------------------------------------------------------
  int ax,ay,aw,ah,arc1;
/*printf( "drawActiveOne: ix=%d,orientation=%d\n",ix,orientation);*/
  if(!ctrl[ix].exists&&!anyCbFlg) return(1);
  actWin->executeGc.setFG( pvc.getColor());
  actWin->executeGc.setArcModePieSlice();
  if(horizontal){
    ay=ctrl[ix].y; ax=ctrl[ix].x-ctrl[ix].h;
    aw=ah=ctrl[ix].h*2;
  }
  else{
    ax=ctrl[ix].x; ay=ctrl[ix].y-ctrl[ix].w;
    aw=ah=ctrl[ix].w*2;
  }
  switch(orientation){
    case ORIENT_HB:	ay-=ctrl[ix].h; break;
    case ORIENT_VR:	ax-=ctrl[ix].w; break;
  }
  arc1=arcStart+90*64*orientation;
    XFillArc( actWin->d,XtWindow(mmvWidget),actWin->executeGc.normGC(),
	ax,ay,aw,ah,arc1,arcStop);
  return 1;
}
int mmvClass::draw(void){
//------------------------------------------------------------------------------
  if(deleteRequest) return 1;
  actWin->drawGc.saveFg();
  actWin->drawGc.setFG( bgColor.pixelColor());
  XDrawRectangle( actWin->d, XtWindow(actWin->drawWidget),
	actWin->drawGc.normGC(),x,y,w,h);
  if(created){ drawOne( 0); drawOne( 1);}

  actWin->drawGc.restoreFg();
  return 1;
}
int mmvClass::eraseActivePointers(void){
//------------------------------------------------------------------------------
  if(!enabled||!activeMode||!init) return 1;
  actWin->executeGc.saveFg();
  if(ctrl[0].exists) drawActiveOne( bgColor,0);
  if(ctrl[1].exists) drawActiveOne( bgColor,1);
  actWin->executeGc.restoreFg();
  return 1;
}
int mmvClass::drawActivePointers(void){
//------------------------------------------------------------------------------
  if(!enabled||!activeMode||!init) return 1;
  if(ctrl[0].exists) drawActiveOne( ctrl[0].color,0);
  if(ctrl[1].exists) drawActiveOne( ctrl[1].color,1);
  return 1;
}
int mmvClass::drawActive(void){
//------------------------------------------------------------------------------
  if(!enabled||!activeMode||!init) return 1;
  actWin->executeGc.saveFg();
  actWin->executeGc.setFG( bgColor.getColor());
  actWin->executeGc.setArcModePieSlice();
  XFillRectangle( actWin->d,XtWindow(mmvWidget),actWin->executeGc.normGC(),
	0,0,w,h);
  actWin->executeGc.restoreFg();
  if(ctrl[0].exists) drawActiveOne( ctrl[0].color,0);
  if(ctrl[1].exists) drawActiveOne( ctrl[1].color,1);
  return 1;
}
void mmvClass::bufInvalidate(void){
//------------------------------------------------------------------------------
  bufInvalid = 1;
}
void mmvEvHandl( Widget w,XtPointer client,XEvent *e,Boolean* flg){
//------------------------------------------------------------------------------
  XMotionEvent* me; XButtonEvent* be; mmvClass* mmv; int stat; double fvalue;

  *flg=True;
  mmv=(mmvClass*)client;
  if(!mmv->active) return;
  if(e->type==EnterNotify){
    if(!mmv->ctrl[0].pvId->have_write_access())
      mmv->actWin->cursor.set(XtWindow(mmv->actWin->executeWidget),CURSOR_K_NO);
    else  mmv->actWin->cursor.set( XtWindow(mmv->actWin->executeWidget),
		CURSOR_K_DEFAULT);
  }
  if(e->type==LeaveNotify)
    mmv->actWin->cursor.set( XtWindow(mmv->actWin->executeWidget),
		CURSOR_K_DEFAULT);
  if(e->type==Expose){ mmv->bufInvalidate(); stat=mmv->drawActive();}
  if(!mmv->ctrl[0].pvId->have_write_access()) return;
  if(e->type==ButtonPress){
    be=(XButtonEvent*)e;
    switch(be->button){
      case Button1:
/*printf( "mmvEvHandl: numCtrls=%d,be->x=%d,be->y=%d,ctrlx=%d,ctrly=%d\n",
mmv->numCtrls,be->x,be->y,mmv->ctrl[0].x,mmv->ctrl[0].y);*/
	if(mmv->horizontal){
	  if((be->x>mmv->ctrl[0].x-mmv->h/2)&&
		(be->x<mmv->ctrl[0].x+mmv->h/2)&&
		(be->y<mmv->h)){	  /* dragging pointer */
	    mmv->ctrl[0].state=MMV_STATE_MOVING; mmv->ixDrag=0;
	  }
	  else if((be->x>mmv->ctrl[1].x-mmv->h/2)&&
		(be->x<mmv->ctrl[1].x+mmv->h/2)&&
		(be->y<mmv->h)){	  /* dragging pointer */
	    mmv->ctrl[1].state=MMV_STATE_MOVING; mmv->ixDrag=1;
	  }
	  else if(mmv->numCtrls==1){
	    if((be->x>mmv->ctrl[0].x+mmv->h/2)){	  /* auto inc */
	      mmv->doIncrement();
	      mmv->incrementTimerActive=1;
	      mmv->incrementTimerValue=101;
	      mmv->incrementTimer=appAddTimeOut(
		mmv->actWin->appCtx->appContext(),500,mmvIncrement,(void*)mmv);
	    }
	    else if((be->x<mmv->ctrl[0].x-mmv->h/2)){  /* auto dec */
	      mmv->doDecrement();
	      mmv->incrementTimerActive=1;
	      mmv->incrementTimerValue=101;
	      mmv->incrementTimer=appAddTimeOut(
		mmv->actWin->appCtx->appContext(),500,mmvDecrement,(void*)mmv);
	    }
	  }
	}
	else{			// vertical
	  if((be->y>mmv->ctrl[0].y-mmv->w/2)&&
		(be->y<mmv->ctrl[0].y+mmv->w/2)&&
		(be->x<mmv->w)){	  /* dragging pointer */
	    mmv->ctrl[0].state=MMV_STATE_MOVING; mmv->ixDrag=0;
	  }
	  else if((be->y>mmv->ctrl[1].y-mmv->w/2)&&
		(be->y<mmv->ctrl[1].y+mmv->w/2)&&
		(be->x<mmv->w)){	  /* dragging pointer */
	    mmv->ctrl[1].state=MMV_STATE_MOVING; mmv->ixDrag=1;
	  }
	  else if(mmv->numCtrls==1){
/*printf( "mmvEvHandl:1: be->y=%d,ctrly=%d,cAy=%d\n",be->y,mmv->ctrl[0].y,
mmv->w);*/
	    if((be->y<mmv->ctrl[0].y-mmv->w/2)){	  /* auto inc */
	      mmv->doIncrement();
	      mmv->incrementTimerActive=1;
	      mmv->incrementTimerValue=101;
	      mmv->incrementTimer=appAddTimeOut(
		mmv->actWin->appCtx->appContext(),500,mmvIncrement,(void*)mmv);
	    }
	    else if((be->y>mmv->ctrl[0].y+mmv->w/2)){  /* auto dec */
	      mmv->doDecrement();
	      mmv->incrementTimerActive=1;
	      mmv->incrementTimerValue=101;
	      mmv->incrementTimer=appAddTimeOut(
		mmv->actWin->appCtx->appContext(),500,mmvDecrement,(void*)mmv);
	    }
	  }
	}
	break;		// end of B1 press
      case Button2:
	mmv->ctrl[0].state=MMV_STATE_IDLE;
	mmv->incrementTimerActive=0;
	mmv->incrementTimerValue=101;
	if(!(be->state&(ControlMask|ShiftMask))) stat=mmv->startDrag( w,e);
	else if(!(be->state&ShiftMask)&&(be->state&ControlMask))
			stat=mmv->showPvInfo( be,be->x,be->y);
	break;

      case Button3:	break;
    }
  }
  if(e->type==ButtonRelease){	// Any B Release
    if(mmv->incrementTimerActive) XtRemoveTimeOut( mmv->incrementTimer);
    mmv->ctrl[0].state = MMV_STATE_IDLE;
    mmv->incrementTimerActive = 0;
    mmv->incrementTimerValue = 101;
    be=(XButtonEvent*)e;
    switch(be->button){
      case Button2:
	if((be->state&ShiftMask)&&!(be->state&ControlMask))
		stat = mmv->selectDragValue( be);
	else if((be->state&ShiftMask)&&(be->state&ControlMask))
		mmv->doActions( be,be->x,be->y);
	break;
      case Button3:
	if(mmv->numCtrls!=1) break;
	mmv->eraseActivePointers();
	if(mmv->horizontal) fvalue=mmv->factor*(be->x-mmv->h/2)+mmv->minFv;
	else fvalue=mmv->factor*(be->y-mmv->w/2)+mmv->maxFv;
	oneCtrlPosition( mmv,&fvalue,0);
	stat=mmv->drawActivePointers();
	mmv->actWin->appCtx->proc->lock();
	mmv->ctrl[0].curv=mmv->ctrl[0].v;
	mmv->actWin->appCtx->proc->unlock();
	if(mmv->ctrl[0].exists){
	  stat=mmv->ctrl[0].pvId->put( fvalue);
	  if(!stat) fprintf( stderr,mmvClStr1);
	}
	else if(mmv->anyCbFlg){
	  mmv->ctrl[mmv->ixDrag].needRefresh=1;
	  mmv->actWin->appCtx->proc->lock();
	  mmv->actWin->addDefExeNode( mmv->aglPtr);
	  mmv->actWin->appCtx->proc->unlock();
	}
	if(mmv->changeCb) (*mmv->changeCb)( mmv);
	break;
    }
  }
  else if(e->type==MotionNotify){
    me=(XMotionEvent*)e;
    if(me->state&Button1Mask){		// B1 Motion
/*printf( "mmvEvHndl: continue drag: ixDrag=%d,state=%d\n",
mmv->ixDrag,mmv->ctrl[mmv->ixDrag].state);*/
      if(mmv->ctrl[mmv->ixDrag].state==MMV_STATE_MOVING){
	mmv->eraseActivePointers();
	if(mmv->horizontal) fvalue=mmv->factor*(me->x-mmv->h/2)+mmv->minFv;
	else fvalue=mmv->factor*(me->y-mmv->w/2)+mmv->maxFv;
	oneCtrlPosition( mmv,&fvalue,mmv->ixDrag);
/*
printf( "mmvEvHndl: x=%d,y=%d,w=%d,h=%d\n",mmv->x,mmv->y,mmv->w,mmv->h);
printf( "mmvEvHndl: cx=%d,cy=%d,cw=%d,ch=%d\n",mmv->ctrl[mmv->ixDrag].x,
mmv->ctrl[mmv->ixDrag].y,mmv->ctrl[mmv->ixDrag].w,mmv->ctrl[mmv->ixDrag].h);
printf( "mmvEvHndl:minFv=%f,maxFv=%f\n",mmv->minFv,mmv->maxFv);
printf( "mmvEvHndl:mey=%d,factor=%f,fv=%f\n",me->y,mmv->factor,fvalue);
*/
	stat=mmv->drawActivePointers();
	mmv->actWin->appCtx->proc->lock();
	mmv->ctrl[mmv->ixDrag].curv=mmv->ctrl[mmv->ixDrag].v;
	mmv->actWin->appCtx->proc->unlock();
	if(mmv->ctrl[mmv->ixDrag].exists){
	  stat=mmv->ctrl[mmv->ixDrag].pvId->put( fvalue);
	  if(!stat) fprintf( stderr,mmvClStr1);
	}
	else if(mmv->anyCbFlg){
	  mmv->ctrl[mmv->ixDrag].needRefresh=1;
	  mmv->actWin->appCtx->proc->lock();
	  mmv->actWin->addDefExeNode( mmv->aglPtr);
	  mmv->actWin->appCtx->proc->unlock();
	}
	if(mmv->changeCb) (*mmv->changeCb)( mmv);
      }
    }
  }
}
int mmvClass::activate( int pass,void *ptr){
//------------------------------------------------------------------------------
  int opStat; char callbackName[63+1];
  switch(pass){
    case 1:	opComplete=0; break;
    case 2:
      if(!opComplete){
	opStat = 1;
	initEnable();
	mmvWidget=XtVaCreateManagedWidget( "",xmDrawingAreaWidgetClass,
		actWin->executeWidgetId(),XmNx,x,XmNy,y,XmNwidth,w,
		XmNheight,h,XmNbackground,bgColor.pixelColor(),NULL);
	if(!mmvWidget){ fprintf( stderr,mmvClStr24); return 0;}
	XtAddEventHandler( mmvWidget,ButtonPressMask|ButtonReleaseMask|
		PointerMotionMask|ExposureMask|EnterWindowMask|LeaveWindowMask,
		False,mmvEvHandl,(XtPointer)this);
	if(enabled){ if(mmvWidget) XtMapWidget( mmvWidget);}
	activeMode=1; init=0; active=0;
	aglPtr=ptr;
	ctrl[0].curv=ctrl[0].onev=0.0; ctrl[0].v=0.0;
	ctrl[1].curv=ctrl[1].onev=0.0; ctrl[1].v=0.0;
	ctrl[0].needConInit=ctrl[1].needConInit=0;
	ctrl[0].needRefresh=ctrl[1].needRefresh=0;
	ctrl[0].needInfInit=ctrl[1].needInfInit=0;
	ctrl[0].oldStat=ctrl[0].oldSev= -1;
	ctrl[1].oldStat=ctrl[1].oldSev= -1;
	ctrl[0].oldv=ctrl[1].oldv=0; updtCtrlTimerActive=0;
	ctrl[0].pvId=ctrl[1].pvId=NULL;
	ctrl[0].state=ctrl[1].state=MMV_STATE_IDLE;
	ctrl[0].pvConnected=ctrl[1].pvConnected=0;
	needErase=needDraw=0;
	initialConnection=1;
	updtCtrlTimer=0; incrementTimerActive=0;
	minFv=maxFv=0.0; factor=1.0;
	fgColor.setConnectSensitive();
	if(ctrl[0].exists){
	  ctrl[0].pvId=the_PV_Factory->create( ctrl[0].pvName.getExpanded());
	  if(ctrl[0].pvId){
	    ctrl[0].pvId->add_conn_state_callback(mmvMonCtrl1ConState,this);}
	  else{ fprintf( stderr,mmvClStr25); opStat=0;}
	}
	if(ctrl[1].exists){
	  ctrl[1].pvId=the_PV_Factory->create( ctrl[1].pvName.getExpanded());
	  if(ctrl[1].pvId){
	    ctrl[1].pvId->add_conn_state_callback(mmvMonCtrl2ConState,this);}
	  else{ fprintf( stderr,mmvClStr25); opStat=0;}
	}
	if(!ctrl[0].exists&&!ctrl[1].exists&&anyCbFlg){
	  init=1; ctrl[0].needInfInit=ctrl[1].needInfInit=1;
	  actWin->appCtx->proc->lock();
	  actWin->addDefExeNode( aglPtr);
	  actWin->appCtx->proc->unlock();
	}
	if(anyCbFlg){
	  if(changeCbFlg){
	    strncpy( callbackName,id,63);
	    Strncat( callbackName,mmvClStr26,63);
	    changeCb=actWin->appCtx->userLibObject.getFunc( callbackName);
	  }
	  if(activateCbFlg){
	    strncpy( callbackName,id,63);
	    Strncat( callbackName,mmvClStr27,63);
	    activateCb=actWin->appCtx->userLibObject.getFunc( callbackName);
	  }
	  if(deactivateCbFlg){
	    strncpy( callbackName,id,63);
	    Strncat( callbackName,mmvClStr28,63);
	    deactivateCb=actWin->appCtx->userLibObject.getFunc( callbackName);
	  }
	  if(activateCb) (*activateCb)( this);
	}
	if(opStat&1){ opComplete=1;}
	return opStat;
      }
      break;
    case 3:
    case 4:	break;
    case 5:	opComplete=0; break;
    case 6:	if(!opComplete){	opComplete=1;}
		break;
  }
  return 1;
}
int mmvClass::deactivate( int pass){
//------------------------------------------------------------------------------
  activeMode=0;
  if(ef.formIsPoppedUp()) ef.popdown();
  if(deactivateCb) (*deactivateCb)( this);

  switch(pass){
    case 1:
      if(updtCtrlTimerActive){
	updtCtrlTimerActive = 0;
	if(updtCtrlTimer){
	  XtRemoveTimeOut( updtCtrlTimer);
	  updtCtrlTimer=0;
	}
      }
      XtRemoveEventHandler( mmvWidget,ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask|ExposureMask|EnterWindowMask|LeaveWindowMask,
	False,mmvEvHandl,(XtPointer)this);
      if(ctrl[0].exists){
	if(ctrl[0].pvId){
	  ctrl[0].pvId->remove_conn_state_callback(
		mmvMonCtrl1ConState,this);
	  ctrl[0].pvId->remove_value_callback( mmvCtrl1Update,this);
	  ctrl[0].pvId->release();
	  ctrl[0].pvId=NULL;
	}
      }
      if(ctrl[1].exists){
	if(ctrl[1].pvId){
	  ctrl[1].pvId->remove_conn_state_callback(
		mmvMonCtrl2ConState,this);
	  ctrl[1].pvId->remove_value_callback( mmvCtrl2Update,this);
	  ctrl[1].pvId->release();
	  ctrl[1].pvId=NULL;
	}
      }
      break;
    case 2:	if(mmvWidget){
		  XtUnmapWidget( mmvWidget);
		  XtDestroyWidget( mmvWidget);
		}
		break;
  }
  return 1;
}
void mmvClass::updateDimensions( void){
//------------ adjust w & h to accomodate frame widget -------------------------
  if(horizontal){
    ctrl[0].w=ctrl[1].w=w-h; ctrl[0].h=ctrl[1].h=h;
    ctrl[0].y=ctrl[1].y=0;   minW=100; minH=12;
  }
  else{
    ctrl[0].h=ctrl[1].h=h-w; ctrl[0].w=ctrl[1].w=w;
    ctrl[0].x=ctrl[1].x=0;   minW=12; minH=100;
  }

  // dummy values at this point
  minFv=0.0; maxFv=10.0; positive=1;
  if(horizontal) factor=(maxFv-minFv)/(w-h);
  else factor=(minFv-maxFv)/(h-w);
  if(factor==0.0) factor=1.0;
/*
printf( "updtDim: w=%d,h=%d,minW=%d,minH=%d,factor=%f\n",
w,h,minW,minH,factor);
printf( "updtDim: minFv=%f,maxFv=%f,ctrl[0].w=%d\n",minFv,maxFv,ctrl[0].w);*/
}
int mmvClass::expand1st( int numMacros,char *macros[],char *expansions[]){
//------------------------------------------------------------------------------
  int retStat, stat;

  retStat = 1;
  stat = ctrl[0].pvName.expand1st( numMacros, macros, expansions);
  if(!( stat & 1)) retStat = stat;
  stat = ctrl[1].pvName.expand1st( numMacros, macros, expansions);
  if(!( stat & 1)) retStat = stat;
  return retStat;
}
int mmvClass::expand2nd( int numMacros,char *macros[],char *expansions[]){
//------------------------------------------------------------------------------
  int retStat, stat;

  retStat = 1;
  stat = ctrl[0].pvName.expand2nd( numMacros, macros, expansions);
  if(!( stat & 1)) retStat = stat;
  stat = ctrl[1].pvName.expand2nd( numMacros, macros, expansions);
  if(!( stat & 1)) retStat = stat;
  return retStat;
}
void mmvClass::executeDeferred( void){
//------------------------------------------------------------------------------
  int stat,nc1c,nc2c,nc1i,nc2i,nc1r,nc2r,ne,nd;
  double c1v,c2v,fv; int ix;

  if(actWin->isIconified) return;

  actWin->appCtx->proc->lock();
  nc1c=ctrl[0].needConInit; ctrl[0].needConInit=0;
  nc1i=ctrl[0].needInfInit; ctrl[0].needInfInit=0;
  nc1r=ctrl[0].needRefresh; ctrl[0].needRefresh=0;
  c1v=ctrl[0].curv;
  nc2c=ctrl[1].needConInit; ctrl[1].needConInit=0;
  nc2i=ctrl[1].needInfInit; ctrl[1].needInfInit=0;
  nc2r=ctrl[1].needRefresh; ctrl[1].needRefresh=0;
  c2v=ctrl[1].curv;
  ne = needErase; needErase = 0;
  nd = needDraw; needDraw = 0;
  actWin->remDefExeNode( aglPtr);
  actWin->appCtx->proc->unlock();

  if(!activeMode) return;
  if(nc1c){
    ctrl[0].pvConnected=1;
    if(limitsFromDb||efScaleMin.isNull()){
	scaleMin=ctrl[0].pvId->get_lower_disp_limit();}
    if(limitsFromDb||efScaleMax.isNull()){
	scaleMax=ctrl[0].pvId->get_upper_disp_limit();}
    minFv=scaleMin;
    maxFv=scaleMax;
    c1v=ctrl[0].curv=ctrl[0].pvId->get_double();
    nc1i=1;
  }
  if(nc2c){
    ctrl[1].pvConnected=1;
    if(limitsFromDb||efScaleMin.isNull()){
	scaleMin=ctrl[1].pvId->get_lower_disp_limit();}
    if(limitsFromDb||efScaleMax.isNull()){
	scaleMax=ctrl[1].pvId->get_upper_disp_limit();}
    minFv=scaleMin;
    maxFv=scaleMax;
    c2v=ctrl[1].curv=ctrl[1].pvId->get_double();
    nc2i=1;
  }
  if(nc1i||nc2i){
    if(maxFv>minFv) positive=1; else positive=0;
    if(nc1i) ix=0; else ix=1;
    if(horizontal){ factor=(maxFv-minFv)/ctrl[ix].w;}
    else{ factor=(minFv-maxFv)/ctrl[ix].h;}
    if(factor==0.0) factor=1.0;
    active=1; init=1;
    fgColor.setConnected();
    bufInvalidate();
    stat=eraseActive();
    stat=drawActive();
    bufInvalidate();
  }
  if(nc1i){
    ctrl[0].v=c1v;
    if(horizontal) ctrl[0].x=(int)((ctrl[0].v-minFv)/factor+0.5)+h/2;
    else ctrl[0].y=(int)((ctrl[0].v-maxFv)/factor+0.5)+w/2;
    ctrl[0].color.setConnected();
  }
  if(nc2i){
    ctrl[1].v=c2v;
    if(horizontal) ctrl[1].x=(int)((ctrl[1].v-minFv)/factor+0.5)+h/2;
    else ctrl[1].y=(int)((ctrl[1].v-maxFv)/factor+0.5)+w/2;
    ctrl[1].color.setConnected();
  }
  if(initialConnection&&(nc1i||nc2i)){
    initialConnection=0;
    if(nc1i&&ctrl[0].exists){
	ctrl[0].pvId->add_value_callback( mmvCtrl1Update,this);}
    if(nc2i&&ctrl[1].exists){
	ctrl[1].pvId->add_value_callback( mmvCtrl2Update,this);}
  }
  if(nc1r){
    fv=ctrl[0].v;
    oneCtrlPosition( this,&fv,0);
    ctrl[0].v=fv;
  }
  if(nc2r){
    fv=ctrl[1].v;
    oneCtrlPosition( this,&fv,0);
    ctrl[1].v=fv;
  }
  if(nc1r||nc2r){
    eraseActivePointers();
    stat=drawActivePointers();
    if(changeCb)(*changeCb)( this);
  }
  if(ne){ eraseActive();}
  if(nd){ drawActive();}
}
int mmvClass::getProperty( char *prop,double *_value){
//------------------------------------------------------------------------------
  int ix=0;
  if(strcmp( prop,mmvClStr29)==0){
    *_value = ctrl[ix].v; return 1;
  }
  return 0;
}
void mmvClass::map( void){
//------------------------------------------------------------------------------
  if(mmvWidget) XtMapWidget( mmvWidget);
}
void mmvClass::unmap( void){
//------------------------------------------------------------------------------
  int ix=0;
  if(mmvWidget) XtUnmapWidget( mmvWidget);
  ctrl[ix].state=MMV_STATE_IDLE;
  incrementTimerActive=0;
  incrementTimerValue=101;
}
void mmvClass::getPvs( int max,ProcessVariable *pvs[],int *n){
//------------------------------------------------------------------------------
  int ix=0;
  if(max<3){*n=0; return;}
  *n = 3;
  pvs[0] = ctrl[ix].pvId;
}
char *mmvClass::crawlerGetFirstPv( void){
//----------- crawler functions may return blank pv names ----------------------
  crawlerPvIndex=0; return ctrl[0].pvName.getExpanded();
}
char *mmvClass::crawlerGetNextPv( void){
//------------------------------------------------------------------------------
  int max=2;

  if(crawlerPvIndex>=max) return NULL;
  crawlerPvIndex++;
  if(crawlerPvIndex==1) return ctrl[1].pvName.getExpanded();
  return NULL;
}

#ifdef __cplusplus
extern "C" {
#endif

void *create_mmvClassPtr( void){
//------------------------------------------------------------------------------
  mmvClass *ptr;

  ptr = new mmvClass;
  return (void *) ptr;
}
void *clone_mmvClassPtr( void *_srcPtr){
//------------------------------------------------------------------------------
  mmvClass *ptr, *srcPtr;

  srcPtr = (mmvClass *) _srcPtr;
  ptr = new mmvClass( srcPtr);
  return (void *) ptr;
}

#ifdef __cplusplus
}
#endif
