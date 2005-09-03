#include "function.h"

function::function(QString newFunc, QColor color=Qt::red) {
	setFunction(newFunc, color);
}

int function::setFunction(QString newFunc="sin(x)", QColor color=Qt::red){
	int ret;
	m_exp = newFunc.stripWhiteSpace();
	if(m_exp[0]=='<')
		ret = func.setTextMML(m_exp);
	else
		ret = func.setText(m_exp);
	m_color = color;
	return ret;
}

void function::update_points(QRect viewport, int resolucio){
	double x=0.0, inv_res=1.0f/resolucio;
	
	int i=0;
	double l_lim=viewport.left()-1, r_lim=viewport.right()+1;
	
	if(resolucio!=m_last_resolution || viewport.width() != m_last_viewport.width()) // TODO: Could optimize for zooming
		points = new QDoublePoint[1+(1+viewport.width())*resolucio];
	else {
		if(viewport.right()<m_last_viewport.right() && resolucio==m_last_resolution) {
			r_lim= m_last_viewport.left()-1;
			for(i=viewport.width()*resolucio; i>=0;i--){ //FIXME
				points[i] = points[i+resolucio*(viewport.right()-m_last_viewport.right())];
			}
			qDebug("%d", viewport.width()*resolucio	);
			i=0;
		}
		
		if(viewport.left()>m_last_viewport.left() && resolucio==m_last_resolution) {
			l_lim= m_last_viewport.right();
			for(i=0;i<(viewport.width()-(viewport.left()-m_last_viewport.left()))*resolucio;i++){
				points[i]=points[i+(viewport.left()-m_last_viewport.left())*resolucio];
			}
		}
	}
	
	for(x=l_lim; x<=r_lim; x+=inv_res) {
		func.vars.modifica("x", x);
		double y = func.Calcula().text().toDouble();
		points[i++].setXY(x, y);
	}
// 	qDebug("[%f, %f]=%d", l_lim, r_lim, viewport.width()*resolucio);
	
	m_last_viewport=viewport;
	m_last_resolution=resolucio;
}
