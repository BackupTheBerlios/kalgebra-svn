#ifndef QGRAPH_H
#define QGRAPH_H

#include <cmath>

#include <QResizeEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QWidget>
#include <QPainter>
#include <QEvent>
#include <QToolTip>
#include <QLabel>
#include <QCursor>
#include <QPixmap>

#include "analitza.h"
#include "function.h"

/** @author aleix */

class QGraph : public QWidget
{
Q_OBJECT
public:
	enum GraphMode {None=0, Pan, Selection};
	
	QGraph(QWidget *parent = 0);
	~QGraph();
	bool addFunction(const function&);
	bool setSelected(const QString&);
	bool setShown(const function&, bool shown);
	bool editFunction(int num, const function& func);
	bool editFunction(const QString& tochange, const function& func);
	function* editFunction(int num);
	bool toImage(QString path);
	
	QSizePolicy sizePolicy() const;
	
	void setSquares(bool newSquare) {m_squares=newSquare; valid=false; }
	bool squares() const {return m_squares;}
	void unselect();
	void setViewport(const QRectF&);
	void setResolution(int res);
	void clear();
	bool isFramed() const { return m_framed; }
	void setFramed(bool fr) { m_framed=fr; }
	bool isReadOnly() const { return m_readonly; }
	void setReadOnly(bool ro) { m_readonly=ro; setMouseTracking(!ro); }
private:
	static const QColor m_axeColor;
	static const QColor m_axe2Color;
	
	//painting
	QPixmap buffer;
	QPixmap front;
	bool valid;
	QPainter finestra;
	QLabel *micepos;
	QList<function> funclist;
	QPointF mark;
	void drawAxes(QPainter*);
	void drawPolarAxes(QPainter*);
	void drawCartesianAxes(QPainter*);
	void update_points();
	QPointF toWidget(const QPointF &);
	QPointF fromWidget(const QPoint& p);
	QPointF calcImage(QPointF dp);
	QPointF toViewport(const QPoint &mv);
	
	//events
	void paintEvent( QPaintEvent * );
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent * e );
	void wheelEvent(QWheelEvent *e);
	void resizeEvent(QResizeEvent *);
	GraphMode mode;
	QPoint press; QPoint last;
	
	//representacio
	bool m_squares;
	double resolucio;
	double rang_x, rang_y;
	QPointF ant;
	QRectF viewport;
	QRectF defViewport;
	void pintafunc(QPaintDevice*);
	void update_scale();
		
	void sendStatus(const QString& msg) { emit status(msg); }
	bool m_framed;
	bool m_readonly;
	QString m_posText;
	static QRect toBiggerRect(const QRectF&);
public slots:
	void forceRepaint() { valid=false; repaint(); }
	void resetViewport() { setViewport(defViewport); }
signals:
	void status(const QString &msg);
};

#endif
