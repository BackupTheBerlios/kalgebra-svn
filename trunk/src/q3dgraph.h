#ifndef Q3DGRAPH_H
#define Q3DGRAPH_H

#include <kurl.h>

#include <qgl.h>
#include <qtimer.h>
#include <qimage.h>
#include <qdatetime.h>
#include "analitza.h"

#define G_POINTS	0
#define G_LINES		1
#define G_SOLID		2

#define KEYRIGHT	1<<0
#define KEYLEFT		1<<1
#define KEYUP		1<<2
#define KEYDOWN		1<<3
#define KEYAVPAG	1<<4
#define KEYREPAG	1<<5
#define KEYS		1<<6
#define KEYW		1<<7
#define KEYQ		1<<8
#define KEYE		1<<9
#define LCLICK		1<<10
#define RCLICK		1<<11
#define MCLICK		1<<12


/**
@author aleix,,,
*/
class Q3DGraph : public QGLWidget {
Q_OBJECT
public:
	Q3DGraph(QWidget *parent = 0, const char *name = 0);
	~Q3DGraph();

	virtual void initializeGL() ;
	virtual void resizeGL( int width, int height ) ;
	virtual void paintGL() ;
	int setFunc(QString Text);
	int setFuncMML(QString TextMML);
	void dibuixa_eixos();
	void setTraslucency(bool tr);
	QPixmap toPixmap();
	bool save(const KURL& url);
private:
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
	void timeOut();
	void mousePressEvent(QMouseEvent *e); QPoint press;
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	float graus[3];
	float z;
	Analitza func3d;
	bool tefunc;
	double **punts;
	void crea();
	double default_step;
	double default_size;
	double zoom;
	int method;
	bool trans;
	void mem();
	unsigned short keyspressed;
protected slots:
	void timeOutSlot();
public:
	void setMida(double max);
	void setStep(double res);
	void setZ(float coord_z);
	void setMethod(int m);
};

#endif
