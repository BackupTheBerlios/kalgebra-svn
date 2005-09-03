#ifndef QGRAPH_H
#define QGRAPH_H

#include <math.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>
#include <qtooltip.h>
#include <qlabel.h>
#include <qpicture.h>
#include <qcursor.h>
#include "qdoublepoint.h"
#include "analitza.h"
#include "function.h"

/** @author aleix */

class QGraph : public QWidget
{
Q_OBJECT
public:
	QGraph(QWidget *parent = 0, const char *name = 0);
	~QGraph();
	int setFunc(QStringList Text, QValueList<QColor>);
// 	int setFuncMML(QString TextMML);
// 	Analitza func;
	bool toImage(QString path);
private:
	//pinta
	QValueList<function> funclist;
	void dibuixa_eixos(QPainter*);
	void update_points();
	QPoint toWidget(double, double, bool* b=0);
	QDoublePoint fromWidget(QPoint p);
	void trunca(QPoint*, QPoint*, bool*);
	QPainter finestra;
	QLabel *micepos;
	
	//events
	void paintEvent( QPaintEvent * );
	void mousePressEvent(QMouseEvent *e); QPoint press;
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent * e );
	void wheelEvent(QWheelEvent *e);
	
	//representacio
	int resolucio;
	double rang_x, rang_y;
	QRect viewport;
	QPoint toViewport(const QPoint &mv);
	void pintafunc(QPaintDevice*);
public:
	void setViewPort(QRect);
	void setResolution(int res);
	inline static int round(double val){
		return (val-floor(val)>=0.5) ? (int) floor(val)+1 : (int) floor(val);
	}
	
};

#endif
