#ifndef FUNCTION_H
#define FUNCTION_H

#include <QColor>
#include "analitza.h"

/**
@author Aleix Pol i Gonzalez
*/

class function {
public:
	function();
	function(const function& f);
	function(const QString& newFunc, const QColor& color, bool selec=false, bool mathml=false);
	~function();
	
	Analitza *func;
	QPointF *points;
	
	int setFunction(const QString& newFunc, const QColor&, bool selec=false, bool mathml=false);
	void update_points(QRect viewport, unsigned int resolucio);
	
	QColor color() const { return m_color; }
	void setColor(const QColor& newColor) { m_color=newColor; }
	unsigned int npoints() const { return m_last_resolution; }
	QString expression() const { return m_exp; }
	void setSelected(bool newSelec) { m_selected=newSelec; }
	bool selected() const { return m_selected; }
	void setShown(bool newShow) { m_show=newShow; }
	bool isShown() const { return m_show && func->isCorrect(); }
	QPointF calc(const QPointF& dp);
	bool operator==(const function& f) const { return f.expression()==expression() && f.color()==color();}
	function operator=(const function& f);
	Analitza* analitza() const { return func; }
private:
	bool m_show;
	bool m_selected;
	QString m_exp;
	QString m_firstlambda;
	
	QColor m_color;
	QRect m_last_viewport;
	unsigned int m_last_resolution;
	unsigned int m_last_max_res;
	QStringList err; //function errors
	void update_pointsY(QRect viewport, unsigned int resolucio); //for functions such as y=f(x)
	void update_pointsX(QRect viewport, unsigned int resolucio); //for functions such as x=f(y)
};

#endif
