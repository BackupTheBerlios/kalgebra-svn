#ifndef Q3DGRAPH_H
#define Q3DGRAPH_H

#include <qgl.h>
#include <qtimer.h>
#include <qimage.h>
#include "analitza.h"

/**
@author aleix,,,
*/
class Q3DGraph : public QGLWidget
{
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
private:
	void keyPressEvent( QKeyEvent *e );
	void timeOut();
	void mousePressEvent(QMouseEvent *e); QPoint press;
	void mouseReleaseEvent(QMouseEvent *e); 
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
protected slots:
	void timeOutSlot();
public:
	void setMida(double max);
	void setStep(double res);
	void setZ(float coord_z);
	void setMethod(int m);
};

#endif
