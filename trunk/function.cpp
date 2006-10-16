#include "function.h"
#include "qexp.h"

function::function()
	: func(NULL), points(NULL), m_selected(false),m_last_max_res(0)
{}

function::function(const QString& newFunc, const QColor& color=Qt::red, bool selec, bool mathml)
	: func(NULL), points(NULL), m_selected(selec), m_last_max_res(0)
{
	setFunction(newFunc, color, selec, mathml);
}


function::function(const function& f)
	: func(NULL), points(NULL), m_selected(false),m_last_max_res(0)
{
	setFunction(f.expression(), f.color(), f.selected(), Analitza::isMathML(f.expression()));
}

function function::operator=(const function& f)
{
	func=NULL; points=NULL; m_selected=f.selected(); m_last_max_res=0;
	setFunction(f.expression(), f.color(), f.selected(), Analitza::isMathML(f.expression()));
	return *this;
}

function::~function()
{
	if(func!=NULL)
		delete func;
	if(points!=NULL)
		delete [] points;
}

int function::setFunction(const QString& newFunc, const QColor& color=Qt::red, bool selec, bool mathml)
{
	int ret;
	m_last_max_res=0;
	m_last_resolution=0;
	m_exp = newFunc.trimmed();
	m_color = color;
	m_selected = selec;
	m_show=true;
	
	func = new Analitza;
	QString funct = m_exp;
	if(!mathml){
		QExp e(funct);
		e.parse();
		funct = e.mathML();
	}
	ret = func->setTextMML(funct);
	
	if(points!=NULL) {
		delete [] points;
		points=NULL;
	}
	return ret;
}

void function::update_points(QRect viewport, unsigned int max_res)
{
	Q_ASSERT(func!=NULL);
	if(!m_show)
		return;
	QStringList lambdas = func->bvarList();
	
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

void function::update_pointsY(QRect viewport, unsigned int max_res)
{
	if(viewport.left()==m_last_viewport.left() && viewport.right()==m_last_viewport.right() && max_res==m_last_max_res/* || max_res<=viewport.width()*/)
		return;
	
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
	
	func->m_vars->modify("x", new Cn(0.));
	Cn *vx = (Cn*) func->m_vars->value("x");
	
	for(x=l_lim; x<=r_lim; x+=inv_res) {
		vx->setValue(x);
		double y = func->calculate().value();
		points[i++]=QPointF(x, y);
	}
	
	m_last_viewport=viewport;
	m_last_resolution=resolucio;
	m_last_max_res = max_res;
}

void function::update_pointsX(QRect viewport, unsigned int max_res)
{
	if(viewport.top()==m_last_viewport.top() && viewport.bottom()==m_last_viewport.bottom() && max_res==m_last_max_res || max_res<=static_cast<unsigned int>(-viewport.height()))
		return;
	
	if(max_res!=m_last_max_res) {
		if(points!=NULL)
			delete [] points;
		points = new QPointF[max_res];
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
		func->m_vars->modify("y", new Cn(y));
		x = func->calculate().value();
		points[i++]=QPointF(x, y);
	}
	
	m_last_viewport=viewport;
	m_last_resolution=resolucio;
	m_last_max_res = max_res;
}

QPointF function::calc(const QPointF& p)
{
	QPointF dp=p;
	if(func->m_tree!=NULL && !m_exp.isEmpty()) {
		if(m_firstlambda=="y") {
			func->m_vars->modify("y", dp.y());
			dp.setX(func->calculate().value());
		} else {
			func->m_vars->modify(QString("x"), dp.x());
			dp.setY(func->calculate().value());
		}
	}
	return dp;
}