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
	function(){}
	function(QString newFunc, QColor color);
	~function(){}
	
	Analitza func;
	QString m_exp;
	QDoublePoint *points;
	
	int setFunction(QString newFunc, QColor);
	void update_points(QRect viewport, int resolucio);
	QColor color(){ return m_color; }
	void setColor(QColor newColor) { m_color=newColor; }
	
private:
	QColor m_color;
	QRect m_last_viewport;
	int m_last_resolution;
};

#endif
