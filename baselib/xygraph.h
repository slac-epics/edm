//  edm - extensible display manager

//  Copyright (C) 1999 John W. Sinclair

//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef __xygraph_h
#define __xygraph_h 1

#include <Xm/TextF.h>
#include <Xm/Text.h>
#include <math.h>
#include <time.h>
#include <vector>

#include "ulBindings.h"
#include "act_grf.h"
#include "entry_form.h"
#include "utility.h"
#include "keypad.h"
#include "calpad.h"
#include "msg_dialog.h"

#include "app_pkg.h"
#include "act_win.h"

#include "pv_factory.h"
#include "cvtFast.h"

#include "edmTime.h"

#define XYGC_K_MAX_TRACES 20

// scaling options
#define XYGC_K_FROM_PV 0
#define XYGC_K_USER_SPECIFIED 1
#define XYGC_K_AUTOSCALE 2

#define XYGC_K_SMOOTHING 0
#define XYGC_K_NO_SMOOTHING 1

#define XYGC_K_SYMBOL_TYPE_NONE 0
#define XYGC_K_SYMBOL_TYPE_CIRCLE 1
#define XYGC_K_SYMBOL_TYPE_SQUARE 2
#define XYGC_K_SYMBOL_TYPE_DIAMOND 3

#define XYGC_K_UPDATE_ON_X_AND_Y 0
#define XYGC_K_UPDATE_ON_X_OR_Y 1
#define XYGC_K_UPDATE_ON_X 2
#define XYGC_K_UPDATE_ON_Y 3
#define XYGC_K_UPDATE_ON_TRIG 4

#define XYGC_K_PLOT_STYLE_LINE 0
#define XYGC_K_PLOT_STYLE_POINT 1
#define XYGC_K_PLOT_STYLE_NEEDLE 2
#define XYGC_K_PLOT_STYLE_SINGLE_POINT 3

#define XYGC_K_TRACE_INVALID 0
#define XYGC_K_TRACE_XY 1
#define XYGC_K_TRACE_CHRONOLOGICAL 2

#define XYGC_K_STATE_UNKNOWN 0
#define XYGC_K_STATE_INITIALIZING 1
#define XYGC_K_STATE_GOT_1ST_POINT 2
#define XYGC_K_STATE_GOT_2ND_POINT 3
#define XYGC_K_STATE_PLOTTING 4
#define XYGC_K_STATE_PAUSED 5

#define XYGC_K_PLOT_MODE_PLOT_N_STOP 0
#define XYGC_K_PLOT_MODE_PLOT_LAST_N 1

#define XYGC_K_RESET_MODE_IF_NOT_ZERO 0
#define XYGC_K_RESET_MODE_IF_ZERO 1

#define XYGC_K_AXIS_STYLE_LINEAR 0
#define XYGC_K_AXIS_STYLE_LOG10 1
#define XYGC_K_AXIS_STYLE_TIME 2
#define XYGC_K_AXIS_STYLE_TIME_LOG10 3

#define XYGC_K_AXIS_TIME_FMT_SEC 0
#define XYGC_K_AXIS_TIME_FMT_MMDDYY_HHMMSS 1

#define XYGC_K_PVNAME 20
#define XYGC_K_LIT 21
#define XYGC_K_PVDESC 22

#define XYGC_K_FORMAT_FFLOAT 1
#define XYGC_K_FORMAT_GFLOAT 2
#define XYGC_K_FORMAT_EXPONENTIAL 3

#define XYGC_K_SCOPE_MODE 0
#define XYGC_K_PLOT_SORTED_X_MODE 1

#define XYGC_MAJOR_VERSION 4
#define XYGC_MINOR_VERSION 7
#define XYGC_RELEASE 0

#ifdef __xygraph_cc

#include "xygraph.str"

static char        *dragName[] = {
	xyGraphClass_str1,
	xyGraphClass_str2,
	xyGraphClass_str3,
	xyGraphClass_str4,
	xyGraphClass_str5,
	xyGraphClass_str6,
	xyGraphClass_str7,
	xyGraphClass_str8,
	xyGraphClass_str9,
	xyGraphClass_str10,
	xyGraphClass_str11,
	xyGraphClass_str12,
	xyGraphClass_str13,
	xyGraphClass_str14,
	xyGraphClass_str15,
	xyGraphClass_str16,
	xyGraphClass_str17,
	xyGraphClass_str18,
	xyGraphClass_str19,
	xyGraphClass_str20,
	xyGraphClass_str21,
	xyGraphClass_str22,
	xyGraphClass_str23,
	xyGraphClass_str24,
	xyGraphClass_str25,
	xyGraphClass_str26,
	xyGraphClass_str27,
	xyGraphClass_str28,
	xyGraphClass_str29,
	xyGraphClass_str30,
	xyGraphClass_str31,
	xyGraphClass_str32,
	xyGraphClass_str33,
	xyGraphClass_str36,
	xyGraphClass_str35,
	xyGraphClass_str36,
	xyGraphClass_str37,
	xyGraphClass_str38,
	xyGraphClass_str39,
	xyGraphClass_str40,
	xyGraphClass_str41,
	xyGraphClass_str42,
};

static void         updateTimerAction(
	XtPointer client,
	XtIntervalId * id );

static void         updateAutoScaleTimerAction(
	XtPointer client,
	XtIntervalId * id );

static void         adjp_edit_apply(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         adjp_edit_ok(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         adjp_edit_cancel(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         dump_edit_apply(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         dump_edit_ok(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         dump_edit_cancel(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         menu_cb(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         setKpXMinDoubleValue(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         setKpXMaxDoubleValue(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         cancelKpXMin(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         cancelKpXMax(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         setKpY1MinDoubleValue(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         setKpY2MinDoubleValue(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         setKpYMinDoubleValue(
	Widget w,
	XtPointer client,
	XtPointer call,
	int yIndex );

static void         setKpY1MaxDoubleValue(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         setKpY2MaxDoubleValue(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         setKpYMaxDoubleValue(
	Widget w,
	XtPointer client,
	XtPointer call,
	int yIndex );

static void         cancelKpY1Min(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         cancelKpY2Min(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         cancelKpYMin(
	Widget w,
	XtPointer client,
	XtPointer call,
	int yIndex );

static void         cancelKpY1Max(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         cancelKpY2Max(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         cancelKpYMax(
	Widget w,
	XtPointer client,
	XtPointer call,
	int yIndex );

static void         xMonitorConnection(
	ProcessVariable * pv,
	void *userarg );

static void         yMonitorConnection(
	ProcessVariable * pv,
	void *userarg );

static void         nMonitorConnection(
	ProcessVariable * pv,
	void *userarg );

static void         xValueUpdate(
	ProcessVariable * pv,
	void *userarg );

static void         yValueUpdate(
	ProcessVariable * pv,
	void *userarg );

static void         nValueUpdate(
	ProcessVariable * pv,
	void *userarg );

static void         yValueWithTimeUpdate(
	ProcessVariable * pv,
	void *userarg );

static void         traceCtlMonitorConnection(
	ProcessVariable * pv,
	void *userarg );

static void         traceCtlValueUpdate(
	ProcessVariable * pv,
	void *userarg );

static void         resetMonitorConnection(
	ProcessVariable * pv,
	void *userarg );

static void         resetValueUpdate(
	ProcessVariable * pv,
	void *userarg );

static void         trigMonitorConnection(
	ProcessVariable * pv,
	void *userarg );

static void         trigValueUpdate(
	ProcessVariable * pv,
	void *userarg );

static void         axygc_edit_ok_trace(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         axygc_edit_ok_axis(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         axygc_edit_ok(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         axygc_edit_update(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         axygc_edit_apply(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         axygc_edit_cancel(
	Widget w,
	XtPointer client,
	XtPointer call );

static void         axygc_edit_cancel_delete(
	Widget w,
	XtPointer client,
	XtPointer call );

#endif

// *INDENT-OFF*
#define	USE_PVDATA_CLASS	1
#ifdef	USE_PVDATA_CLASS
class pvData
{
 public:
	pvData(	ProcessVariable			*	pv,
			int							pvType,
			size_t						pvNumElem,
			int							pvIsSigned	);

	virtual ~pvData( );

	double		GetValue(			size_t	index = 0	);

	void		SetValue(			size_t index, double	newValue	);

	void		SetValueFromPv(		size_t index );

	void		SetValuesFromPv( );

	void		resize(				size_t	newSize )
	{
		m_values.resize( newSize );
	}

	const char * GetName( ) const
	{
		return( m_pv == NULL ? "NULL" : m_pv->get_name() );
	}

	const size_t		size( )
	{
		return m_values.size( );
	}

	const size_t		getNumElem( )
	{
		return m_pvNumElem;
	}

 private:
	ProcessVariable	*	m_pv;
	std::vector<double>	m_values;
	int					m_pvType;
	size_t				m_pvNumElem;
	int					m_pvIsSigned;
};
#endif	//	USE_PVDATA_CLASS
// *INDENT-OFF*


class xyGraphClass : public activeGraphicClass
{

	public:
	static const unsigned int NUM_Y_AXES = 2;

	private:

	// Why are we using a struct instead of a class here?
	typedef struct editBufTag
	{
		// edit buffer
		int bufX;
		int bufY;
		int bufW;
		int bufH;
		char bufGraphTitle[127+1];
		char bufXLabel[127+1];
		char bufYLabel[127+1];
		char bufY2Label[127+1];
		int bufPlotMode;
		int bufResetMode;
		// All these array's for trace parameters make the code really ugly
		// and hard to read and write.
		// Consider pushing all this to an xyTrace class
		int bufPlotStyle[XYGC_K_MAX_TRACES];
		int bufPlotSymbolType[XYGC_K_MAX_TRACES];
		int bufPlotUpdateMode[XYGC_K_MAX_TRACES];
		int bufPlotColor[XYGC_K_MAX_TRACES];
		char bufXPvName[XYGC_K_MAX_TRACES][PV_Factory::MAX_PV_NAME+1];
		char bufYPvName[XYGC_K_MAX_TRACES][PV_Factory::MAX_PV_NAME+1];
		char bufNPvName[XYGC_K_MAX_TRACES][PV_Factory::MAX_PV_NAME+1];
		int bufXSigned[XYGC_K_MAX_TRACES];
		int bufYSigned[XYGC_K_MAX_TRACES];
		int bufLineThk[XYGC_K_MAX_TRACES];
		int bufLineStyle[XYGC_K_MAX_TRACES];
		char bufTrigPvName[PV_Factory::MAX_PV_NAME+1];
		char bufResetPvName[PV_Factory::MAX_PV_NAME+1];
		char bufTraceCtlPvName[PV_Factory::MAX_PV_NAME+1];
		int bufOpMode[XYGC_K_MAX_TRACES];
		int bufY2Scale[XYGC_K_MAX_TRACES];
		int bufCount;
		int bufXAxis;
		int bufXAxisStyle;
		int bufXAxisSource;
		int bufXAxisTimeFormat;
		efDouble bufXMin;
		efDouble bufXMax;

		efInt bufXNumLabelIntervals;
		int bufXLabelGrid;
		efInt bufXNumMajorPerLabel;
		int bufXMajorGrid;
		efInt bufXNumMinorPerMajor;
		int bufXMinorGrid;
		efInt bufXAnnotationPrecision;
		int bufXAnnotationFormat;
		int bufXGridMode;
		int bufXAxisSmoothing;

		int bufY1Axis[NUM_Y_AXES];
		int bufY1AxisStyle[NUM_Y_AXES];
		int bufY1AxisSource[NUM_Y_AXES];
		efDouble bufY1Min[NUM_Y_AXES];
		efDouble bufY1Max[NUM_Y_AXES];

		efInt bufY1NumLabelIntervals[NUM_Y_AXES];
		int bufY1LabelGrid[NUM_Y_AXES];
		efInt bufY1NumMajorPerLabel[NUM_Y_AXES];
		int bufY1MajorGrid[NUM_Y_AXES];
		efInt bufY1NumMinorPerMajor[NUM_Y_AXES];
		int bufY1MinorGrid[NUM_Y_AXES];
		efInt bufY1AnnotationPrecision[NUM_Y_AXES];
		int bufY1AnnotationFormat[NUM_Y_AXES];
		int bufY1GridMode[NUM_Y_AXES];
		int bufY1AxisSmoothing[NUM_Y_AXES];

		int bufY2Axis;
		int bufY2AxisStyle;
		int bufY2AxisSource;
		efDouble bufY2Min;
		efDouble bufY2Max;

		efInt bufY2NumLabelIntervals;
		int bufY2LabelGrid;
		efInt bufY2NumMajorPerLabel;
		int bufY2MajorGrid;
		efInt bufY2NumMinorPerMajor;
		int bufY2MinorGrid;
		efInt bufY2AnnotationPrecision;
		int bufY2AnnotationFormat;

		int bufFgColor;
		int bufBgColor;
		int bufGridColor;
		int bufFormatType;
		int bufBorder;
		int bufPlotAreaBorder;
		int bufAutoScaleBothDirections;
		efInt bufAutoScaleTimerMs;
		efDouble bufAutoScaleThreshPct;
		int bufXFormatType;
		efInt bufXPrecision;
		int bufY1FormatType[NUM_Y_AXES];
		efInt bufY1Precision[NUM_Y_AXES];
		int bufY2FormatType;
		efInt bufY2Precision;

		int bufUpdateTimerValue;

	}	editBufType, *editBufPtr;

	entryListBase		*	scaleInwardEntry;
	entryListBase		*	scaleInwardTimerEntry;
	entryListBase		*	scaleInwardThreshEntry;

	entryListBase		*	resetPvEntry;
	entryListBase		*	resetModeEntry;

	typedef struct objPlusIndexTag
	{
		void *objPtr;
		int index;
	}	objPlusIndexType, *objPlusIndexPtr;

	friend void updateTimerAction (
	  XtPointer client,
	  XtIntervalId *id );

	friend void updateAutoScaleTimerAction (
	  XtPointer client,
	  XtIntervalId *id );

	friend void adjp_edit_apply (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void adjp_edit_ok (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void adjp_edit_cancel (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void dump_edit_apply (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void dump_edit_ok (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void dump_edit_cancel (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void menu_cb (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void setKpXMinDoubleValue (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void setKpXMaxDoubleValue (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void cancelKpXMin (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void cancelKpXMax (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void setKpY1MinDoubleValue (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void setKpY2MinDoubleValue (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void setKpYMinDoubleValue (
	  Widget w,
	  XtPointer client,
	  XtPointer call,
	  int yIndex );

	friend void setKpY1MaxDoubleValue (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void setKpY2MaxDoubleValue (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void setKpYMaxDoubleValue (
	  Widget w,
	  XtPointer client,
	  XtPointer call,
	  int yIndex );

	friend void cancelKpY1Min (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void cancelKpY2Min (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void cancelKpYMin (
	  Widget w,
	  XtPointer client,
	  XtPointer call,
	  int yIndex );

	friend void cancelKpY1Max (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void cancelKpY2Max (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void cancelKpYMax (
	  Widget w,
	  XtPointer client,
	  XtPointer call,
	  int yIndex );

	friend void xMonitorConnection (
	  ProcessVariable *pv,
	  void *userarg );

	friend void yMonitorConnection (
	  ProcessVariable *pv,
	  void *userarg );

	friend void nMonitorConnection (
	  ProcessVariable *pv,
	  void *userarg );

	friend void xValueUpdate (
	  ProcessVariable *pv,
	  void *userarg );

	friend void yValueUpdate (
	  ProcessVariable *pv,
	  void *userarg );

	friend void nValueUpdate (
	  ProcessVariable *pv,
	  void *userarg );

	friend void yValueWithTimeUpdate (
	  ProcessVariable *pv,
	  void *userarg );

	friend void traceCtlMonitorConnection (
	  ProcessVariable *pv,
	  void *userarg );

	friend void traceCtlValueUpdate (
	  ProcessVariable *pv,
	  void *userarg );

	friend void resetMonitorConnection (
	  ProcessVariable *pv,
	  void *userarg );

	friend void resetValueUpdate (
	  ProcessVariable *pv,
	  void *userarg );

	friend void trigMonitorConnection (
	  ProcessVariable *pv,
	  void *userarg );

	friend void trigValueUpdate (
	  ProcessVariable *pv,
	  void *userarg );

	friend void axygc_edit_ok_trace (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void axygc_edit_ok_axis (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void axygc_edit_ok (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void axygc_edit_update (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void axygc_edit_apply (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void axygc_edit_cancel (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	friend void axygc_edit_cancel_delete (
	  Widget w,
	  XtPointer client,
	  XtPointer call );

	static const int symHalfWidth = 2;
	static const int symHalfHeight = 2;

	int bufX, bufY, bufW, bufH;

	Pixmap pixmap;

	pvConnectionClass connection;

	keypadClass kp;

	double kpXMin, kpXMax;
	efDouble kpXMinEfDouble, kpXMaxEfDouble;
	int kpCancelMinX, kpCancelMaxX;
	int xMinX0, xMinX1, xMinY0, xMinY1;
	int xMaxX0, xMaxX1, xMaxY0, xMaxY1;

	double kpY1Min[NUM_Y_AXES], kpY1Max[NUM_Y_AXES];
	efDouble kpY1MinEfDouble[NUM_Y_AXES], kpY1MaxEfDouble[NUM_Y_AXES];
	int kpCancelMinY1[NUM_Y_AXES], kpCancelMaxY1[NUM_Y_AXES];
	int y1MinX0[NUM_Y_AXES], y1MinX1[NUM_Y_AXES], y1MinY0[NUM_Y_AXES], y1MinY1[NUM_Y_AXES];
	int y1MaxX0[NUM_Y_AXES], y1MaxX1[NUM_Y_AXES], y1MaxY0[NUM_Y_AXES], y1MaxY1[NUM_Y_AXES];

	expStringClass graphTitle, xLabel, yLabel, y2Label;

	int numTraces;
	int plotAreaX, plotAreaY, plotAreaW, plotAreaH;

	// All these array's for trace parameters make the code really ugly
	// and hard to read and write.
	// Consider pushing all this to an xyTrace class
	objPlusIndexType	xcArgRec[XYGC_K_MAX_TRACES];
	objPlusIndexType	xvArgRec[XYGC_K_MAX_TRACES];
	objPlusIndexType	ycArgRec[XYGC_K_MAX_TRACES];
	objPlusIndexType	yvArgRec[XYGC_K_MAX_TRACES];
	objPlusIndexType	ncArgRec[XYGC_K_MAX_TRACES];
	objPlusIndexType	nvArgRec[XYGC_K_MAX_TRACES];

	ProcessVariable		*	xPv[XYGC_K_MAX_TRACES];
	ProcessVariable		*	yPv[XYGC_K_MAX_TRACES];
	ProcessVariable		*	nPv[XYGC_K_MAX_TRACES];

	int		initialXConnection[XYGC_K_MAX_TRACES];
	int		initialYConnection[XYGC_K_MAX_TRACES];

	int plotColor[XYGC_K_MAX_TRACES];
	int lineThk[XYGC_K_MAX_TRACES];
	int lineStyle[XYGC_K_MAX_TRACES];
	int plotSymbolType[XYGC_K_MAX_TRACES];
	int plotUpdateMode[XYGC_K_MAX_TRACES];
	int opMode[XYGC_K_MAX_TRACES];
	int y2Scale[XYGC_K_MAX_TRACES];

	expStringClass xPvExpStr[XYGC_K_MAX_TRACES];
	expStringClass yPvExpStr[XYGC_K_MAX_TRACES];
	expStringClass nPvExpStr[XYGC_K_MAX_TRACES];

	int		xPvType[XYGC_K_MAX_TRACES];
	int		yPvType[XYGC_K_MAX_TRACES];

	int traceType[XYGC_K_MAX_TRACES]; // XYGC_K_TRACE_XY or XYGC_K_TRACE_CHRONOLOGICAL

	int plotState[XYGC_K_MAX_TRACES];

	unsigned int	traceSize[XYGC_K_MAX_TRACES];

	typedef struct plotInfoTag
	{
		int n;
		double firstDX;
		double lastDX;
		short firstX;
		short lastX;
		short firstY;
		short lastY;
		short minY;
		short maxY;
		short yZero;
	}	plotInfoType, *plotInfoPtr;

	plotInfoPtr		plotInfo[XYGC_K_MAX_TRACES];
	unsigned int	plotInfoSize[XYGC_K_MAX_TRACES];
	unsigned int	plotInfoHead[XYGC_K_MAX_TRACES];
	unsigned int	plotInfoTail[XYGC_K_MAX_TRACES];

	std::vector<pvData *>	xPvData;
	std::vector<pvData *>	yPvData;
	double			xPvCurValue[XYGC_K_MAX_TRACES];
	double			yPvCurValue[XYGC_K_MAX_TRACES];
	double			xPvMin[XYGC_K_MAX_TRACES];
	double			xPvMax[XYGC_K_MAX_TRACES];
	double			yPvMin[XYGC_K_MAX_TRACES];
	double			yPvMax[XYGC_K_MAX_TRACES];
	unsigned int	xPvCount[XYGC_K_MAX_TRACES];
	unsigned int	yPvCount[XYGC_K_MAX_TRACES];
	unsigned int	yPvDim[XYGC_K_MAX_TRACES];
	unsigned int	xPvDim[XYGC_K_MAX_TRACES];
	unsigned int	xPvSize[XYGC_K_MAX_TRACES];
	unsigned int	yPvSize[XYGC_K_MAX_TRACES];

	XPoint		*	plotBuf[XYGC_K_MAX_TRACES];
	unsigned int	plotBufSize[XYGC_K_MAX_TRACES];

	int				traceIsDrawn[XYGC_K_MAX_TRACES];

	double			dbXMin[XYGC_K_MAX_TRACES];
	double			dbXMax[XYGC_K_MAX_TRACES];
	int				dbXPrec[XYGC_K_MAX_TRACES];
	int				xArrayNeedInit[XYGC_K_MAX_TRACES];
	int				xArrayNeedUpdate[XYGC_K_MAX_TRACES];
	int				xArrayGotValue[XYGC_K_MAX_TRACES];
	int				xArrayGotValueCallback[XYGC_K_MAX_TRACES];

	double			dbYMin[XYGC_K_MAX_TRACES];
	double			dbYMax[XYGC_K_MAX_TRACES];
	int				dbYPrec[XYGC_K_MAX_TRACES];
	int				yArrayNeedInit[XYGC_K_MAX_TRACES];
	int				yArrayNeedUpdate[XYGC_K_MAX_TRACES];
	int				yArrayGotValue[XYGC_K_MAX_TRACES];
	int				yArrayGotValueCallback[XYGC_K_MAX_TRACES];

	unsigned int	arrayHead[XYGC_K_MAX_TRACES];
	unsigned int	arrayTail[XYGC_K_MAX_TRACES];
	unsigned int	arrayNumPoints[XYGC_K_MAX_TRACES];
	unsigned int	curNpts[XYGC_K_MAX_TRACES];

	unsigned int	effectiveCount[XYGC_K_MAX_TRACES];
	unsigned int	totalCount[XYGC_K_MAX_TRACES];

	ProcessVariable		*	traceCtlPv;
	ProcessVariable		*	resetPv;
	ProcessVariable		*	trigPv;
	int				initialTraceCtlConnection;
	int				initialResetConnection;
	int				initialTrigConnection;
	expStringClass	traceCtlPvExpStr;
	expStringClass	trigPvExpStr;
	expStringClass	resetPvExpStr;

	unsigned int	count;		// nPts from xygraph properties
	unsigned int	bufferScrollSize;
	int				plotStyle[XYGC_K_MAX_TRACES];
	int				plotMode;
	int				resetMode;
	int				firstTimeSample;
	int				curSec;
	int				curNsec;
	int				drawGridFlag;
	int				special[XYGC_K_MAX_TRACES];
	int				forceVector[XYGC_K_MAX_TRACES];
	int				traceCtl;

	int				xAxis;
	int				xAxisStyle;
	int				xAxisSource;
	int				xAxisTimeFormat;
	efDouble		xMin;
	efDouble		xMax;

	int				y1Axis[NUM_Y_AXES];
	int				y1AxisStyle[NUM_Y_AXES];
	int				y1AxisSource[NUM_Y_AXES];
	efDouble		y1Min[NUM_Y_AXES];
	efDouble		y1Max[NUM_Y_AXES];

	int				y2Axis;
	int				y2AxisStyle;
	int				y2AxisSource;
	efDouble		y2Min;
	efDouble		y2Max;

	double			curXMin;
	double			curXMax;
	double			adjCurXMin;
	double			adjCurXMax;
	double			curY1Min[NUM_Y_AXES];
	double			curY1Max[NUM_Y_AXES];
	double			adjCurY1Min[NUM_Y_AXES];
	double			adjCurY1Max[NUM_Y_AXES];
	double			curY2Min;
	double			curY2Max;
	int				curXPrec;
	int				curY1Prec[NUM_Y_AXES];
	int				curY2Prec;

	int				curXNumLabelTicks;
	int				curXMajorsPerLabel;
	int				curXMinorsPerMajor;
	int				curY1NumLabelTicks[NUM_Y_AXES];
	int				curY1MajorsPerLabel[NUM_Y_AXES];
	int				curY1MinorsPerMajor[NUM_Y_AXES];
	int				curY2NumLabelTicks;
	int				curY2MajorsPerLabel;
	int				curY2MinorsPerMajor;

	colorButtonClass	plotCb[XYGC_K_MAX_TRACES];
	colorButtonClass	fgCb;
	colorButtonClass	bgCb;
	colorButtonClass	gridCb;

	int				fgColor;
	int				bgColor;
	int				gridColor;

	int				xFormatType;
	efInt			xPrecision;
	char			xFormat[15+1];

	int				y1FormatType[NUM_Y_AXES];
	efInt			y1Precision[NUM_Y_AXES];
	char			y1Format[NUM_Y_AXES][15+1];

	int				y2FormatType;
	efInt			y2Precision;
	char			y2Format[15+1];

	fontMenuClass	fm;
	char			fontTag[63+1];

	int				border;
	int				plotAreaBorder;
	int				autoScaleBothDirections;
	efInt			autoScaleTimerMs;
	efDouble		autoScaleThreshPct;
	double			autoScaleThreshFrac;

	int				opComplete;
	int				active;
	int				activeMode;
	int				init;
	int				bufInvalid;
	XFontStruct	*	fs;
	int				fontAscent;
	int				fontDescent;
	int				fontHeight;

	efInt			xNumLabelIntervals;
	int				xLabelGrid;
	efInt			xNumMajorPerLabel;
	int				xMajorGrid;
	efInt			xNumMinorPerMajor;
	int				xMinorGrid;
	efInt			xAnnotationPrecision;
	int				xAnnotationFormat;
	int				xGridMode;
	int				xAxisSmoothing;

	efInt			y1NumLabelIntervals[NUM_Y_AXES];
	int				y1LabelGrid[NUM_Y_AXES];
	efInt			y1NumMajorPerLabel[NUM_Y_AXES];
	int				y1MajorGrid[NUM_Y_AXES];
	efInt			y1NumMinorPerMajor[NUM_Y_AXES];
	int				y1MinorGrid[NUM_Y_AXES];
	efInt			y1AnnotationPrecision[NUM_Y_AXES];
	int				y1AnnotationFormat[NUM_Y_AXES];
	int				y1GridMode[NUM_Y_AXES];
	int				y1AxisSmoothing[NUM_Y_AXES];

	efInt			y2NumLabelIntervals;
	int				y2LabelGrid;
	efInt			y2NumMajorPerLabel;
	int				y2MajorGrid;
	efInt			y2NumMinorPerMajor;
	int				y2MinorGrid;
	efInt			y2AnnotationPrecision;
	int				y2AnnotationFormat;

	int				xPvExists[XYGC_K_MAX_TRACES];
	int				yPvExists[XYGC_K_MAX_TRACES];
	int				traceCtlPvExists;
	int				trigPvExists;
	int				resetPvExists;

	double			xFactor[XYGC_K_MAX_TRACES], xOffset[XYGC_K_MAX_TRACES];
	double			y1Factor[NUM_Y_AXES][XYGC_K_MAX_TRACES],
	 y1Offset[NUM_Y_AXES][XYGC_K_MAX_TRACES];
	double			y2Factor[XYGC_K_MAX_TRACES], y2Offset[XYGC_K_MAX_TRACES];

	Widget			plotWidget;

	int				needNPvConnect[XYGC_K_MAX_TRACES];

	int				needConnect;
	int				needInit;
	int				needRefresh;
	int				needUpdate;
	int				needErase;
	int				needDraw;
	int				needResetConnect;
	int				needReset;
	int				needTrigConnect;
	int				needTrig;
	int				needTraceCtlConnect;
	int				needTraceUpdate;
	int				needXRescale;
	int				needY1Rescale[NUM_Y_AXES];
	int				needY2Rescale;
	int				needBufferScroll;
	int				needVectorUpdate;
	int				needRealUpdate;
	int				needBoxRescale;
	int				needNewLimits;
	int				needOriginalLimits;
	int				needAutoScaleUpdate;
	int				needNConnect;
	int				needNInit;
	int				needArraySizeChange;

	int				numBufferScrolls;

	int				updateTimerActive;
	int				updateTimerValue;
	XtIntervalId	updateTimer;

	int				updateAutoScaleTimerActive;
	int				updateAutoScaleTimerValue;
	XtIntervalId	updateAutoScaleTimer;

	double			xRescaleValue;
	double			y1RescaleValue[NUM_Y_AXES];
	double			y2RescaleValue;

	int				needThisbufScroll[XYGC_K_MAX_TRACES];

	entryFormClass	*	efTrace;
	entryFormClass	*	efAxis;
	entryFormClass		efDump;

	editBufPtr		eBuf;

	msgDialogClass	msgDialog;
	int				msgDialogPopedUp; 
	int				firstBoxRescale, doingBoxRescale;
	double			savedXMin;
	double			savedXMax;
	double			savedYMin[NUM_Y_AXES];
	double			savedYMax[NUM_Y_AXES];
	double			boxXMin;
	double			boxXMax;
	double			boxYMin[NUM_Y_AXES];
	double			boxYMax[NUM_Y_AXES];
	int				savedXMinNullState;
	int				savedXMaxNullState;
	int				savedYMinNullState[NUM_Y_AXES];
	int				savedYMaxNullState[NUM_Y_AXES];
	int				rescaleBoxX0;
	int				rescaleBoxY0;
	int				rescaleBoxX1;
	int				rescaleBoxY1;
	int				oldRescaleBoxW;
	int				oldRescaleBoxH;

	int				xSigned[XYGC_K_MAX_TRACES];
	int				ySigned[XYGC_K_MAX_TRACES];

	unsigned int	lowestYScaleIndex[NUM_Y_AXES];
	unsigned int	numYTraces[NUM_Y_AXES];

	time_t			timeOffset;

	int				widgetsCreated;
	Widget			popUpMenu;
	Widget			pullDownMenu;
	Widget			pbAutoScale;
	Widget			pbOrigScale;
	Widget			pbAdjustParams;
	Widget			pbDumpData;
	Widget			pbClearPlot;

	int				adjpFormX;
	int				adjpFormY;
	int				adjpFormW;
	int				adjpFormH;
	int				adjpFormMaxH;

	int				dumpFormX;
	int				dumpFormY;
	int				dumpFormW;
	int				dumpFormH;
	int				dumpFormMaxH;
	char			dumpFileName[255+1];

	int				popupMenuX;
	int				popupMenuY;

	public:

	xyGraphClass( void );

	xyGraphClass( const xyGraphClass *source );

	~xyGraphClass( void );

	int getDbXMinXMax (
	  double *min,
	  double *max	);

	void getDbYMinYMax (
	  double *min,
	  double *max,
	  int yi	);

	void getXMinMax (
	  double *min,
	  double *max	);

	void getYMinMax (
	  int yi,
	  double *min,
	  double *max	);

	char *objName ( void )
	{
	  return name;
	}

	int createInteractive (
	  activeWindowClass *aw_obj,
	  int x,
	  int y,
	  int w,
	  int h );

	int createFromFile (
	  FILE *f,
	  char *name,
	  activeWindowClass *_actWin );

	int old_createFromFile (
	  FILE *f,
	  char *name,
	  activeWindowClass *_actWin );

	int importFromXchFile (
	  FILE *f,
	  char *name,
	  activeWindowClass *_actWin );

	int save (
	  FILE *f );

	int old_save (
	  FILE *f );

	int genericEdit ( void );

	int edit ( void );

	int editCreate ( void );

	void regenBuffer ( void );

	void genChronoVector (
	  int trace,
	  int *rescale
	);

	void genXyVector (
	  int trace,
	  int *rescale
	);

	int fullRefresh ( void );

	int draw ( void );

	int erase ( void );

	void drawCircles (
	  int index,
	  XPoint *xp,
	  int n
	);

	void drawSquares (
	  int index,
	  XPoint *xp,
	  int n
	);

	void drawDiamonds (
	  int index,
	  XPoint *xp,
	  int n
	);

	int drawActiveOne (
	  int i // trace
	);

	int drawActive ( void );

	int eraseActive ( void );

	void bufInvalidate ( void );

	int expandTemplate (
	  int numMacros,
	  char *macros[],
	  char *expansions[] );

	int expand1st (
	  int numMacros,
	  char *macros[],
	  char *expansions[] );

	int expand2nd (
	  int numMacros,
	  char *macros[],
	  char *expansions[] );

	int containsMacros ( void );

	char *firstDragName ( void );

	char *nextDragName ( void );

	char *dragValue ( int i );

	int activate ( int pass, void *ptr );

	int deactivate ( int pass );

	void updateDimensions ( void );

	void btnDrag (
	  XMotionEvent *me,
	  int x,
	  int y,
	  int buttonState,
	  int buttonNumber );

	void btnUp (
	  XButtonEvent *be,
	  int x,
	  int y,
	  int buttonState,
	  int buttonNumber,
	  int *action );

	void btnDown (
	  XButtonEvent *be,
	  int x,
	  int y,
	  int buttonState,
	  int buttonNumber,
	  int *action );

	int getButtonActionRequest (
	  int *up,
	  int *down,
	  int *drag );

	void executeDeferred ( void );

	void initPlotInfo ( int trace	);

	void addPoint (
	  double oneX,
	  double scaledX,
	  double scaledY,
	  int	trace,
	  int	yAxisSelect
	);

	int fillPlotArray ( int trace, int isVector	);

	int fillScalarPlotArray ( int trace	);

	int fillVectorPlotArray ( int trace	);

	void drawBorder ( void );

	void drawXScale ( void );

	void drawY1Scale ( void );

	void drawY2Scale ( void );

	void drawGrid ( void );

	void drawTitle ( void );

	void drawXlabel ( void );

	void drawYlabel ( void );

	void drawY2label ( void );

	void getPvs ( int max, ProcessVariable *pvs[], int *n );

	char *getSearchString ( int i	);

	void replaceString ( int i, int max, char *string	);

	char *crawlerGetFirstPv ( void );

	char *crawlerGetNextPv ( void );

	void setXRescale( double rescale );
	void setYRescale( double rescale, unsigned int useY2Axis );

	bool traceIsDisabled( unsigned int i )
	{
		return( ( traceCtl & ( 1 << i ) ) != 0 );
	}
};

#ifdef __cplusplus
extern "C" {
#endif

void *create_xyGraphClassPtr ( void );
void *clone_xyGraphClassPtr ( void * );

#ifdef __cplusplus
}
#endif

#endif
