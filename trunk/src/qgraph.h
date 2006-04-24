#ifndef QGRAPH_H
#define QGRAPH_H

#include <math.h>

#include <qdatetime.h>
#include <kdebug.h>

#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>
#include <qtooltip.h>
#include <qlabel.h>
#include <qpicture.h>
#include <qcursor.h>
#include <qpixmap.h>

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
	bool addFunction(const function&);
	bool setSelected(const QString&);
	bool setShown(const QString&, const bool& shown);
	bool editFunction(const unsigned int& num, const function& func);
	bool editFunction(const QString& tochange, const function& func);
	bool toImage(QString path);
	
	QSizePolicy sizePolicy() const;
	
	inline void setSquares(bool newSquare) {m_squares=newSquare; valid=false; }
	bool squares() const {return m_squares;}
	void unselect();
private:
	//painting
	QPixmap buffer; QPixmap front; bool valid;
	QPainter finestra;
	QLabel *micepos;
	QValueList<function> funclist;
	QDoublePoint mark;
	void dibuixa_eixos(QPainter*);
	void update_points();
	QPoint toWidget(double, double, bool* b=0);
	QDoublePoint fromWidget(QPoint p);
	void trunca(QPoint*, QPoint*, bool*);
	QDoublePoint calcImage(QDoublePoint dp);
	
	//events
	void paintEvent( QPaintEvent * );
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent * e );
	void wheelEvent(QWheelEvent *e);
	void resizeEvent(QResizeEvent *);
	int pushed;
	QPoint press; QPoint last;
	
	//representacio
	bool m_squares;
	int resolucio;
	double rang_x, rang_y;
	QPoint ant;
	QRect viewport;
	QPoint toViewport(const QPoint &mv);
	void pintafunc(QPaintDevice*);
	void update_scale() {
		rang_x=(double) this->width()/((double)viewport.width()-1.0);
		rang_y=(double) this->height()/((double)viewport.height()-1.0);
		valid=false;
		this->repaint(false);	}
public:
	void setViewPort(QRect);
	void setResolution(int res);
	void clear();
	inline static int round(double val){ return (val-floor(val)>=0.5) ? (int) floor(val)+1 : (int) floor(val); }
};

#endif
