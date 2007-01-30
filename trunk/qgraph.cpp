#include "qgraph.h"

#include <QPicture>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QList>
#include <QPixmap>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFrame>

QColor const QGraph::m_axeColor(100,100,255);
QColor const QGraph::m_axe2Color(235,235,255);

QGraph::QGraph(QWidget *parent) :
	QWidget(parent),
	mode(None), m_squares(true), resolucio(800), m_framed(false), m_readonly(false), m_posText("")
{
	this->setFocusPolicy(Qt::ClickFocus);
	this->setCursor(QCursor(Qt::CrossCursor));
	this->setMinimumHeight(20);
	this->setMinimumWidth(10);
	this->setMouseTracking(!m_readonly);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	valid=false;
	
	setViewport(QRect(QPoint(-12, 10), QPoint(12, -10)));
	defViewport = viewport;
	this->setAutoFillBackground(false);
}

QGraph::~QGraph() {
// 	funclist.clear();
}

QSizePolicy QGraph::sizePolicy() const
{
	return QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void QGraph::drawAxes(QPainter *f)
{
	finestra.setRenderHint(QPainter::Antialiasing, false);
	Axe a = Cartesian;
	if(!funclist.isEmpty()){
		for (QList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
			if((*it).selected() && (*it).isShown()) {
				a = (*it).axeType();
				break;
			}
		}
	}
	
	switch(a) {
		case Polar:
			drawPolarAxes(f);
			break;
		default:
			drawCartesianAxes(f);
	}
	
	//write coords
	f->drawText(QPointF(3.+this->width()/2., 13.               ), QString::number(viewport.top()));
	f->drawText(QPointF(3.+this->width()/2., this->height()-5. ), QString::number(viewport.bottom()));
	f->drawText(QPointF(8.                 , this->height()/2.-5.), QString::number(viewport.left()));
	f->drawText(QPointF(this->width()-30.  , this->height()/2.-5.),QString::number(viewport.right()));
	//EO write coords
}

void QGraph::drawPolarAxes(QPainter *w)
{
	QPen ceixos;
	ceixos.setColor(m_axeColor);
	w->setPen(ceixos);
	const QPointF centre = toWidget(QPointF(0.,0.));
	bool zero=false;
	zero= centre.x()>0. && centre.y()>0. && centre.x()<width() && centre.y()<height();
	//double thmin = floor(min(viewport.left(), viewport.bottom()));
	//double thmax = ceil(max(viewport.right(), viewport.top()));
	double thmin = zero ? 0. : min(floor(min(viewport.left(), viewport.bottom())), ceil(max(viewport.right(), viewport.top())));
	double thmax = ceil(max(
				sqrt(pow(viewport.topRight().x(), 2.) + pow(viewport.topRight().y(), 2.)),
				sqrt(pow(viewport.bottomLeft().x(), 2.)+ pow(viewport.bottomLeft().y(), 2.))
			    )
			);
		
	ceixos.setColor(m_axe2Color);
	ceixos.setStyle(Qt::SolidLine);
	w->setPen(ceixos);
	
	finestra.setRenderHint(QPainter::Antialiasing, true);
	for(double i=thmin; i<thmax; i++) { //i is +
		QPointF p(toWidget(QPointF(i,i)));
		QPointF p2(toWidget(QPointF(-i,-i)));
		w->drawEllipse(QRectF(p.x(),p.y(), p2.x()-p.x(),p2.y()-p.y()));
	}
	finestra.setRenderHint(QPainter::Antialiasing, false);
	
	ceixos.setColor(m_axeColor);
	ceixos.setStyle(Qt::SolidLine);
	w->setPen(ceixos);
	w->drawLine(QPointF(0., centre.y()), QPointF(this->width(), centre.y()));
	w->drawLine(QPointF(centre.x(), 0.), QPointF(centre.x(),this->height()));
}

void QGraph::drawCartesianAxes(QPainter *finestra)
{
	QPen ceixos;
	const QPointF centre = toWidget(QPointF(0.,0.));
	QPointF p;
	double x;
	
	ceixos.setColor(m_axe2Color);
	ceixos.setStyle(Qt::SolidLine);
	finestra->setPen(ceixos);

	for(x=ceil(viewport.left()); x<=viewport.right(); x+=1.0) {	// ralletes X
		p = toWidget(QPointF(x, 0.));
		if(m_squares)
			finestra->drawLine(QPointF(p.x(), this->height()), QPointF(p.x(), 0));
		else
			finestra->drawLine(p, p+QPointF(0.,-3.));
	}
	
	for(x=ceil(viewport.top()); x>=viewport.bottom(); x-=1.0) {		// ralletes y
		p = toWidget(QPointF(0., x));
		if(m_squares)
			finestra->drawLine(QPointF(0., p.y()), QPointF(width(), p.y()));
		else
			finestra->drawLine(p, p+QPointF(3.,0.));
	}
	
	ceixos.setColor(m_axeColor);
	ceixos.setStyle(Qt::SolidLine);
	finestra->setPen(ceixos);
	
	//dibuixa eixos viewport
	finestra->drawLine(QPointF(0., centre.y()), QPointF(this->width(), centre.y()));
	finestra->drawLine(QPointF(centre.x(), 0.), QPointF(centre.x(),this->height()));
	//EO dibuixa eixos viewport
}

void QGraph::pintafunc(QPaintDevice *qpd)
{
	QPointF ultim(0.,0.), act(0.,0.);
	QPen pfunc, ccursor;
	if(buffer.isNull() || buffer.width()!=width() || buffer.height()!=height())
		buffer = QPixmap(this->width(), this->height());
	buffer.fill(Qt::white);
	
	pfunc.setColor(QColor(0,150,0));
	pfunc.setWidth(2);
	
	if(qpd)
		finestra.begin(qpd);
	else if(!buffer.isNull()) {
		finestra.begin(&buffer);
		finestra.initFrom(this);
	} else
		qDebug("!!!!!!!!!!");
	
	drawAxes(&finestra);
	
	finestra.setRenderHint(QPainter::Antialiasing, true);
	
	QRectF panorama(QPoint(0,0), size());
	finestra.setPen(pfunc);
	
	if(funclist.count()>0){
		for (QList<function>::iterator it=funclist.begin(); it!=funclist.end(); ++it ){
			if((*it).isShown()) {
				pfunc.setColor((*it).color());
				pfunc.setWidth((*it).selected()+1);
				finestra.setPen(pfunc);
				int i = (*it).npoints(), j;
				
				ultim=toWidget((*it).points[0]);
				
				for(j=0; j<i;j++){
					act=toWidget((*it).points[j]);
					
					if(!isnan(act.y()) && !isnan(ultim.y()) && (panorama.contains(act) || panorama.contains(ultim)))
						finestra.drawLine(ultim, act);
					
					ultim=act;
				}
			}
		}
	}
	valid=true;
	finestra.end();
}

void QGraph::paintEvent( QPaintEvent * )
{
	if(!valid) 
		pintafunc(NULL);
	
	front = buffer;
	finestra.begin(&front);
	finestra.initFrom(this);
	QPen ccursor;
	QPointF ultim;
	
// 	finestra.setRenderHint(QPainter::Antialiasing, true);
	
	if(!m_readonly && mode==None) {
		ultim = toWidget(mark);
		
		ccursor.setColor(QColor(0xc0,0,0));
		ccursor.setStyle(Qt::SolidLine);
		
		finestra.setPen(ccursor);
		finestra.drawLine(QPointF(0.,ultim.y()), QPointF(this->width(), ultim.y()));
		finestra.drawLine(QPointF(ultim.x(),0.), QPointF(ultim.x(), this->height()));
		
		int w=finestra.fontMetrics().width(m_posText)+15, h=finestra.fontMetrics().height();
		
		if(ultim.x()+w > this->width())
			ultim.setX(this->width()-w);
		if(ultim.y()+h > this->height())
			ultim.setY(this->height()-h);
		if(ultim.y() < 0.)
			ultim.setY(0.);
		
		finestra.setPen(QPen(QColor(0,0,0)));
		finestra.drawText(QPointF(ultim.x()+15., ultim.y()+15.), m_posText);

	} else if(!m_readonly && mode==Selection) {
		ccursor.setColor(QColor(0xc0,0,0));
		ccursor.setStyle(Qt::SolidLine);
		finestra.setPen(ccursor);
		finestra.setBrush(QColor(0xff,0xff, 0,0x90));
		
		finestra.drawRect(QRect(press,last));
		//QPoint p=last, p1=press;
		/*QPoint p=toViewport(last)+viewport.topLeft(); //REAL mode
		QPoint p1=toViewport(press)+viewport.topLeft();
		p=toWidget((double)p.x(), (double)p.y(), NULL);
		p1=toWidget((double)p1.x(), (double)p1.y(), NULL);
		finestra.drawRect(QRect(p1,p));*/
	} //else micepos->hide();
// 	qDebug("<%d>", pushed);
	
	if(m_framed) {
		QPen bord(Qt::black);
		finestra.setPen(bord);
		QPoint p2=QPoint(this->width(), this->height());
		finestra.drawRect(QRect(QPoint(0,0), p2-QPoint(2,2)));
	}
	finestra.end();
	
	///////////////////////////////
	QPainter win(this);
	win.drawPixmap(QPoint(0,0), front);
}

void QGraph::wheelEvent(QWheelEvent *e){
	int d = e->delta()>0 ? -1 : 1;
	if(viewport.left()-d < 1 && viewport.top()+d > 1 && viewport.right()+d > 1 && viewport.bottom()-d < 1) {
		viewport.setLeft(viewport.left() - d);
		viewport.setTop(viewport.top() + d);
		viewport.setRight(viewport.right() + d);
		viewport.setBottom(viewport.bottom() - d);
		update_scale();
		update_points();
	}
	sendStatus(QString("(%1, %2)-(%3, %4)").arg(viewport.left()).arg(viewport.top()).arg(viewport.right()).arg(viewport.bottom()));
}

void QGraph::mousePressEvent(QMouseEvent *e){
// 	qDebug("%d", toViewport(e->pos()).x());
	if(!m_readonly && (e->button()==Qt::LeftButton || e->button()==Qt::MidButton)) {
		last = press = e->pos();
		ant = toViewport(e->pos());
		this->setCursor(QCursor(Qt::PointingHandCursor));
		if(e->button()==Qt::MidButton || (e->button()==Qt::LeftButton && e->modifiers()&Qt::ControlModifier))
			mode=Pan;
		else if(e->button()==Qt::LeftButton)
			mode=Selection;
		
	}
}

void QGraph::mouseReleaseEvent(QMouseEvent *e){
	this->setCursor(QCursor(Qt::CrossCursor));
	if(!m_readonly && mode==Selection) {
		QPointF pd = toViewport(press) - toViewport(e->pos());
		const double mindist = min(fabs(pd.x()), fabs(pd.y())), rate=7.;
		const double minrate = min(fabs(viewport.width()/rate), fabs(viewport.height()/rate));
		
		if(mindist < minrate) { //if selection is too small
			mode=None;
			repaint();
			return;
		}
		
		QPointF p=toViewport(e->pos())+viewport.topLeft();
		QPointF p1=toViewport(press)+viewport.topLeft();
		
		viewport.setTopLeft(p1);
		viewport.setBottomRight(p);
		
		if(viewport.bottom() > viewport.top()){
			double a=viewport.bottom();
			viewport.setBottom(viewport.top());
			viewport.setTop(a);
		}
		
		if(viewport.left() > viewport.right()){
			double a=viewport.left();
			viewport.setLeft(viewport.right());
			viewport.setRight(a);
		}
	
		update_scale();
		update_points();
		sendStatus(QString("(%1, %2)-(%3, %4)").arg(viewport.left()).arg(viewport.top()).arg(viewport.right()).arg(viewport.bottom()));
	}
	mode = None;
	this->repaint();
}

void QGraph::mouseMoveEvent(QMouseEvent *e)
{
	mark=calcImage(fromWidget(e->pos()));
	
	if(!m_readonly && mode==Pan && ant != toViewport(e->pos())){
		QPointF rel = toViewport((e->pos() - press - (toWidget(QPointF(.5,.5))-toWidget(QPointF(0.,0.)))).toPoint());
		viewport.setLeft(viewport.left() - rel.x()); viewport.setRight(viewport.right() - rel.x());
		viewport.setTop(viewport.top() - rel.y()); viewport.setBottom(viewport.bottom() - rel.y());
		
		update_points();
		press = e->pos();
		ant = toViewport(e->pos());
		valid=false;
		sendStatus(QString("(%1, %2)-(%3, %4)").arg(viewport.left()).arg(viewport.top()).arg(viewport.right()).arg(viewport.bottom()));
	} else if(e->buttons()&Qt::LeftButton) {
		last = e->pos();
	} else if(e->buttons()==0)
		sendStatus(QString("x=%1 y=%2").arg(mark.x(),3,'f',2).arg(mark.y(),3,'f',2));
	
	this->repaint();
}

void QGraph::keyPressEvent(QKeyEvent * e)
{
	const double step = 1.; //TODO: Make step relative to viewport
	switch(e->key()) {
		case Qt::Key_Right:
			viewport.setLeft(viewport.left() +step);
			viewport.setRight(viewport.right() +step);
			break;
		case Qt::Key_Left:
			viewport.setLeft(viewport.left() -step);
			viewport.setRight(viewport.right() -step);
			break;
		case Qt::Key_Down:
			viewport.setTop(viewport.top() -step);
			viewport.setBottom(viewport.bottom() -step);
			break;
		case Qt::Key_Up:
			viewport.setTop(viewport.top() +step);
			viewport.setBottom(viewport.bottom() +step);
			break;
		case Qt::Key_Minus:
// 			resolucio=(resolucio*viewport.width())/(viewport.width()+2.);
			viewport.setCoords(viewport.left() -1., viewport.top() +1., viewport.right() + 1., viewport.bottom() -1.);
			update_scale();
			break;
		case Qt::Key_Plus:
			if(viewport.height() < -3. && viewport.width() > 3.){
				//FIXME:Bad solution
// 				resolucio=(resolucio*viewport.width())/(viewport.width()-2.);
				viewport.setCoords(viewport.left() + 1., viewport.top() -1., viewport.right() -1., viewport.bottom() +1.);
				update_scale();
			} else return;
			break;
		default:
			return;
	}
	valid=false;
	update_points();
	this->repaint();
}

QPointF QGraph::calcImage(QPointF dp){
	m_posText="";
	if(!funclist.isEmpty()){
		for (QList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
			if((*it).selected() && (*it).isShown()) {
				QPair<QPointF, QString> o = (*it).calc(dp);
				dp=o.first;
				m_posText = o.second;
				break;
			}
		}
	}
	return dp;
}


void QGraph::unselect(){
	if(!funclist.isEmpty()){
		for (QList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
			(*it).setSelected(false);
		}
	}
}

void QGraph::update_points(){
	if(!funclist.isEmpty()){
// 		qDebug() << "res:" << resolucio;
		for (QList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it )
			(*it).update_points(toBiggerRect(viewport), static_cast<int>(floor(resolucio)));
		
		valid=false;
	}
}

bool QGraph::addFunction(const function& func)
{
	bool exist=false;
	
	for (QList<function>::iterator it = funclist.begin(); it != funclist.end() && !exist; ++it)
		exist = ((*it) == func);
	
	if(!exist) {
		funclist.append(func);
		sendStatus(i18n("%1 function added").arg(func.expression()));
	}
	
	update_points();
	return exist;
}

bool QGraph::editFunction(const QString& tochange, const function& func){
	bool exist=false;
	
	for (QList<function>::iterator it = funclist.begin(); !exist && it != funclist.end(); ++it ){
		if((*it).expression() == tochange){
			exist=true;
			(*it)=func;
		}
	}
	
	qDebug() << "editF:" << exist;
	
	update_points();
	this->repaint();
	return exist;
}

function* QGraph::editFunction(int num)
{
	return &funclist[num];
}

bool QGraph::editFunction(int num, const function& func)
{
	Q_ASSERT(num<funclist.count());
	funclist[num]=func;
	
	update_points();
	this->repaint();
	return true;
}

bool QGraph::setSelected(const QString& exp){
	for (QList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it )
		(*it).setSelected((*it).expression() == exp);
	
	update_points();
	this->repaint();
	return true;
}

bool QGraph::setShown(const function& f, bool shown)
{
	for (QList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
		if((*it) == f)
			(*it).setShown(shown);
	}
	
	valid=false;
	this->repaint();
	return true;
}

QPointF QGraph::toWidget(const QPointF& p)
{
	return QPointF((-viewport.left() + p.x()) * rang_x,  (-viewport.top() + p.y()) * rang_y);
}

QPointF QGraph::fromWidget(QPoint p)
{
	double part_negativa_x = -viewport.left();
	double part_negativa_y = -viewport.top();
	return QPointF(p.x()/rang_x-part_negativa_x, p.y()/rang_y-part_negativa_y);
}

QPointF QGraph::toViewport(const QPoint &mv)
{
	return QPointF(mv.x()/rang_x, mv.y()/rang_y);
}

void QGraph::setResolution(int res)
{
	resolucio = res;
	update_points();
}

void QGraph::setViewport(const QRectF &vp)
{
	viewport = vp;
	if(viewport.top()<viewport.bottom()) {
		double aux = viewport.bottom();
		viewport.setBottom(viewport.top());
		viewport.setTop(aux);
	}
	
	if(viewport.right()<viewport.left()) {
		double aux = viewport.left();
		viewport.setLeft(viewport.right());
		viewport.setRight(aux);
	}
	
	update_scale();
}

void QGraph::resizeEvent(QResizeEvent *)
{
	buffer=QPixmap(this->size());
	update_scale();
	repaint();
}

void QGraph::clear()
{
	funclist.clear();
	valid=false;
	repaint();
}

QRect QGraph::toBiggerRect(const QRectF& ent)
{
	QRect ret;
	ret.setTop(static_cast<int>(ceil(ent.top())));
	ret.setBottom(static_cast<int>(floor(ent.bottom())));
	ret.setLeft(static_cast<int>(floor(ent.left())));
	ret.setRight(static_cast<int>(ceil(ent.right())));
	
	return ret;
}

//////////////////////////////////////////////////////////////
bool QGraph::toImage(QString path)
{
	bool b=false;
	
	if(!path.isEmpty() && path.endsWith(".svg")) {
		QPicture pic;
		pintafunc(&pic);
		pic.save(path, "svg");
	} else if(!path.isEmpty() && path.endsWith(".png")) {
		this->repaint();
		b=buffer.save(path, "PNG");
	} else
		return false;
	
	qDebug() << "toImage:" << path << b << front.isNull();
	return true;
}
//////////////////////////////////////////////////////////////

void QGraph::update_scale()
{
	
	rang_x= this->width()/(viewport.width()-1.);
	rang_y= this->height()/(viewport.height()-1.);
	valid=false;
	this->repaint();
}
