#include "function.h"
#include "qexp.h"

function::function()
	: points(NULL), m_show(true), m_selected(true), m_exp(QString::null),
		m_color(Qt::black), m_last_resolution(0), m_last_max_res(0)
{
// 	func=new Analitza;
}

function::function(const QString& newFunc, const QColor& color=Qt::red, bool selec, bool mathml)
	: points(NULL), m_show(true), m_selected(selec), m_exp(newFunc.trimmed()),
		m_color(color), m_last_resolution(0), m_last_max_res(0)
{
	qDebug() << "che lolo" << m_exp;
// 	func = new Analitza;
	QString funct=newFunc;
	if(!mathml){
		QExp e(newFunc);
		e.parse();
		funct = e.mathML();
	}
	func.setTextMML(funct);
	
	if(points!=NULL) {
		delete [] points;
		points=NULL;
	}
}


function::function(const function& f)
	: points(NULL), m_show(true), m_selected(f.selected()), m_exp(f.expression()),
		m_color(f.color()), m_last_resolution(0), m_last_max_res(0)
{
	qDebug() << "che lola";
	function(f.expression(), f.color(), f.selected(), Analitza::isMathML(f.expression()));
}

function function::operator=(const function& f)
{
	if(f!=*this) {
		m_last_resolution=0;
		m_last_max_res=0;
		if(points!=NULL) {
			delete [] points;
			points=NULL;
		}
		
// 		func = new Analitza;
		m_show=true;
		m_exp=f.expression();
		m_color=f.color();
		m_selected=f.selected();
		m_last_max_res=0;
		
		QString funct=f.expression();
		if(!Analitza::isMathML(f.expression())){
			QExp e(f.expression());
			e.parse();
			funct = e.mathML();
		}
		func.setTextMML(funct);
	}
	return *this;
}

function::~function()
{
// 	if(func!=NULL)
// 		delete func;
	if(points!=NULL)
		delete [] points;
}

void function::update_points(const QRect& viewport, unsigned int max_res)
{
// 	Q_ASSERT(func!=NULL);
	if(!m_show)
		return;
	QStringList lambdas = func.bvarList();
	
	if(lambdas.count() <= 1){ //2D Graph only support 1 lambda, must recheck when add parametric functions
		m_firstlambda = lambdas.count()==0 ? "x" : lambdas[0];
		
		if(m_firstlambda=="x")
			update_pointsY(viewport, max_res);
		else if(m_firstlambda=="y")
			update_pointsX(viewport, max_res);
		else
			err << i18n("Don't know how to represent f(%1)").arg(m_firstlambda);
	} else
		err << i18n("Too much lambda for a 2D Graph");
}

void function::update_pointsY(const QRect& viewport, unsigned int max_res){
	if(viewport.left()==m_last_viewport.left() && viewport.right()==m_last_viewport.right() && max_res==m_last_max_res/* || max_res<=viewport.width()*/)
		return;
	
	qDebug() << "update_pointsY:" << viewport << max_res;
	
	if(max_res!=m_last_max_res) {
		if(points==NULL)
			delete [] points;
		points = new QPointF[max_res];
	}
	double l_lim=viewport.left()-1., r_lim=viewport.right()+1., x=0.;
	
	unsigned int resolucio=0, ample=static_cast<unsigned int>(-l_lim+r_lim);
	while(resolucio<max_res)
		resolucio+=ample;
	resolucio -= ample;
		
	double inv_res= (double) (-l_lim+r_lim)/resolucio;
	register int i=0;
	
	if(viewport.width() == m_last_viewport.width()) { //FIXME: resolucio diferent
		int cacho = static_cast<int>(round(resolucio/(-l_lim+r_lim)));
		
		if(viewport.right()<m_last_viewport.right()) {
			r_lim= m_last_viewport.left();
			for(i=(viewport.width())*cacho; i>(m_last_viewport.right()-viewport.right());i--)
				points[i] = points[i+cacho*(viewport.right()-m_last_viewport.right())];
			i=0;
		}
		
		if(viewport.left()>m_last_viewport.left()) {
			l_lim= m_last_viewport.right();
			for(i=0;i<(viewport.width()-(viewport.left()-m_last_viewport.left()))*cacho;i++)
				points[i]=points[i+(viewport.left()-m_last_viewport.left())*cacho];
		}
	}
	
	func.m_vars->modify("x", new Cn(0.));
	Cn *vx = (Cn*) func.m_vars->value("x");
	
	for(x=l_lim; x<=r_lim; x+=inv_res) {
		vx->setValue(x);
		double y = func.calculate().value();
		points[i++]=QPointF(x, y);
	}
	
	m_last_viewport=viewport;
	m_last_resolution=resolucio;
	m_last_max_res = max_res;
}

void function::update_pointsX(const QRect& viewport, unsigned int max_res){
	if(viewport.top()==m_last_viewport.top() && viewport.bottom()==m_last_viewport.bottom() && max_res==m_last_max_res || max_res<=static_cast<unsigned int>(-viewport.height()))
		return;
	
	if(max_res!=m_last_max_res) {
		if(points!=NULL)
			delete [] points;
		points = new QPointF[max_res];
		Q_CHECK_PTR(points);
	}
	double t_lim=viewport.top()+1, b_lim=viewport.bottom()-1;
	
	unsigned int resolucio=0, ample=static_cast<unsigned int>(-b_lim+t_lim);
	
	while(resolucio<max_res)
		resolucio+= ample;
	resolucio -= ample;
	
	double inv_res= (double) ( -b_lim+t_lim)/resolucio;
	register unsigned int i=0;
	
	/*if(viewport.height() == m_last_viewport.height()) { //Should port y=f(x) optimizations here
		int cacho = round(resolucio/(-b_lim+t_lim));
// 		qDebug("<%d>", cacho);
		
		if(viewport.top()>m_last_viewport.top()) { //if its gone down
			b_lim= m_last_viewport.top()-1;
			for(i=0;i<(viewport.height()-(viewport.height()+m_last_viewport.top()))*cacho;i++){
// 				qDebug("%d>=%d", resolucio, i+(m_last_viewport.top()-viewport.top())*cacho);
				points[i]=points[i+(m_last_viewport.top()-viewport.top())*cacho];
			}
			i=0;
		}
	}*/
	
	double x, y=0.0;
	for(y=t_lim; y>=b_lim; y-=inv_res) {
		func.m_vars->modify("y", new Cn(y));
		x = func.calculate().value();
		points[i++]=QPointF(x, y);
	}
	
	m_last_viewport=viewport;
	m_last_resolution=resolucio;
	m_last_max_res = max_res;
}

QPointF function::calc(const QPointF& p) const
{
	QPointF dp=p;
	if(func.m_tree!=NULL && !m_exp.isEmpty()) {
		if(m_firstlambda=="y") {
			func.m_vars->modify("y", dp.y());
			dp.setX(func.calculate().value());
		} else {
			func.m_vars->modify(QString("x"), dp.x());
			double v =func.calculate().value();
			dp.setY(v);
		}
	}
	return dp;
}
