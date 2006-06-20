#ifndef FUNCTION_H
#define FUNCTION_H

#include "analitza.h"
#include "qdoublepoint.h"
#include <qcolor.h>

/**
@author Aleix Pol i Gonzalez
*/

class function {
public:
	function();
	function(QString newFunc, QColor color, bool selec=false);
	~function();
	
	Analitza func;
	QDoublePoint *points;
	
	int setFunction(QString newFunc, QColor, bool selec=false);
	void update_points(QRect viewport, unsigned int resolucio);
	
	QColor color(){ return m_color; }
	void setColor(QColor newColor) { m_color=newColor; }
	unsigned int npoints() { return m_last_resolution; }
	QString expression() const { return m_exp; }
	void setSelected(const bool &newSelec) { m_selected=newSelec; }
	bool selected() const { return m_selected; }
	void setShown(const bool &newShow) { m_show=newShow; }
	bool isShown() const { return m_show; }
	QDoublePoint calc(QDoublePoint dp);
private:
	bool m_show;
	bool m_selected;
	QString m_exp;
	QString m_firstlambda;
	
	QColor m_color;
	QRect m_last_viewport;
	unsigned int m_last_resolution;
	unsigned int m_last_max_res;
	QString err; //function errors
	void update_pointsY(QRect viewport, unsigned int resolucio); //for functions such as y=f(x)
	void update_pointsX(QRect viewport, unsigned int resolucio); //for functions such as x=f(y)
};

#endif
