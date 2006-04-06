#include "qgraph.h"

QGraph::QGraph(QWidget *parent, const char *name) : QWidget(parent, name, Qt::WStyle_NormalBorder) {
	m_squares=true;
	
	resolucio=32;
	pushed=0;
	
	viewport = QRect::QRect(QPoint::QPoint(-12, 10), QPoint::QPoint(12, -10));
	
	this->setFocusPolicy(QWidget::WheelFocus);
	micepos = new QLabel("", this);
	micepos->setFrameShape(QFrame::Box);
	micepos->setPaletteBackgroundColor(QColor(255,230,255));
	micepos->setAlignment(AlignAuto | AlignVCenter | AlignHCenter);
	micepos->hide();
	this->setCursor(QCursor(Qt::CrossCursor));
	setMouseTracking(true);
	valid=false;
}

QGraph::~QGraph() {
	finestra.flush();
}

QSizePolicy QGraph::sizePolicy() const {
	return QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void QGraph::dibuixa_eixos(QPainter *finestra){
	QPen ceixos;
	QPoint centre = toWidget(0.,0.);
	double x;
// 	qDebug("(%d, %d)-(%d, %d)", viewport.left(), viewport.top(), viewport.right(), viewport.bottom());
	
	if(m_squares) {
// 		ceixos.setStyle(Qt::DotLine);
		ceixos.setStyle(Qt::SolidLine);
		ceixos.setColor(QColor(225,225,255));
		finestra->setPen(ceixos);
	} else {
		ceixos.setColor(QColor(100,100,255));
		ceixos.setStyle(Qt::SolidLine);
		finestra->setPen(ceixos);
	}
	
	for(x=viewport.left(); x<(viewport.right()+1); x+=1.0) {// ralletes X
		if(m_squares)
			finestra->drawLine(QPoint(toWidget(x,viewport.top()).x(), this->height()), QPoint( toWidget(x,viewport.bottom()+1).x(), 0));
		else
			finestra->drawLine(toWidget(x,0.0f), toWidget(x,0.0f)+QPoint(0,-3));
	}
	
	for(x=viewport.top(); x>viewport.bottom(); x-=1.0) { // ralletes y
		if(m_squares)
			finestra->drawLine(toWidget(viewport.left(),x), toWidget(viewport.right()+1, x));
		else
			finestra->drawLine(toWidget(0.0f,x), toWidget(0.0f,x)+QPoint(3,0));
	}
	
	ceixos.setColor(QColor(100,100,255));
	ceixos.setStyle(Qt::SolidLine);
	finestra->setPen(ceixos);
	
	//dibuixa eixos viewport
	finestra->drawLine(QPoint(0, centre.y()), QPoint(this->width(), centre.y()));
	finestra->drawLine(QPoint(centre.x(), 0), QPoint(centre.x(),this->height()));
	//EO dibuixa eixos viewport
	//write coords
	finestra->drawText(QPoint(3+this->width()/2, 10                ), QString("%1").arg(viewport.bottom()));//Botom coords
	finestra->drawText(QPoint(3+this->width()/2, this->height()-5  ), QString("%1").arg(viewport.top()));	//Top coords
	finestra->drawText(QPoint(8                , this->height()/2-5), QString("%1").arg(viewport.left()));	//Left coords
	finestra->drawText(QPoint(this->width()-30 , this->height()/2-5), QString("%1").arg(viewport.right()));	//Right coords
	//EO write coords
}


void QGraph::paintEvent( QPaintEvent * ){
	if(!valid)
		pintafunc(NULL);
	front = buffer;
	finestra.begin(&front, this);
	QPen pfunc;
	QPen ccursor;
	QPoint ultim;
	
	pfunc.setColor(QColor(0,150,0));
	pfunc.setCapStyle(Qt::RoundCap);
	pfunc.setWidth(2);
	
	if(this->hasMouseTracking() && this->cursor().shape() != Qt::PointingHandCursor) {
		ultim = toWidget(mark.x(), mark.y(), NULL);
		
		ccursor.setColor(QColor(0xc0,0,0));
		ccursor.setStyle(Qt::SolidLine);
		finestra.setPen(ccursor);
		finestra.drawLine(toWidget(viewport.left(),mark.y()), QPoint(this->width(), toWidget(viewport.right()+1, mark.y()).y()));
		finestra.drawLine(QPoint(toWidget(mark.x(),viewport.top()).x(), this->height()), QPoint( toWidget(mark.x(),viewport.bottom()+1).x(), 0));
		
		micepos->setText(QString("<qt>x=<b>%1</b> &nbsp; y=<b>%1</b></qt>") .arg(mark.x(),3,'f',2).arg(mark.y(),3,'f',2));
		
		if(ultim.x()+micepos->width()+10 > this->width())
			ultim.setX(this->width()-micepos->width()-10);
		if(ultim.y()+micepos->height()+10 > this->height())
			ultim.setY(this->height()-micepos->height()-10);
		if(ultim.y() < 0)
			ultim.setY(-10);
		
		micepos->setGeometry(ultim.x()+10, ultim.y()+10, 160, 23);
		micepos->show();
	} else if(this->hasMouseTracking() && pushed==Qt::LeftButton) {
		QBrush rectbr(yellow);
		rectbr.setStyle(Dense6Pattern);
		ccursor.setColor(QColor(0xc0,0,0));
		ccursor.setStyle(Qt::SolidLine);
		finestra.setPen(ccursor);
		finestra.setBrush(rectbr);
		micepos->hide();
		
		QPoint p=last;
		QPoint p1=press;
		finestra.drawRect(QRect(p1,p));
		
		//QPoint p=toViewport(last)+viewport.topLeft(); //REAL mode
		//QPoint p1=toViewport(press)+viewport.topLeft();
// 		p=toWidget((double)p.x(), (double)p.y(), NULL);
// 		p1=toWidget((double)p1.x(), (double)p1.y(), NULL);
// 		finestra.drawRect(QRect(p1,p));
	} else micepos->hide();
// 	qDebug("<%d>", pushed);
	finestra.end();
	
	///////////////////////////////
	//bitBlt(this,0,0,&buffer,0,0,this->width(),this->height());
	bitBlt(this,0,0,&front,0,0,this->width(),this->height());
}

void QGraph::mousePressEvent(QMouseEvent *e){
	pushed |= e->button(); //Left=1; Right=1<<1=2; Center=1<<2=4; Compatibilize with 3d's graph?
// 	qDebug("%d", toViewport(e->pos()).x());
	if(e->button() == Qt::LeftButton || e->button() == Qt::MidButton){
		press = e->pos();
		ant = toViewport(e->pos());
		this->setCursor(QCursor(Qt::PointingHandCursor));
	}
}

void QGraph::wheelEvent(QWheelEvent *e){
	int d = e->delta()>0 ? -1 : 1;
// 	qDebug("%d<>(%d, %d)(%d, %d)", d, d-1, d+1, d+1, d-1);
	if(viewport.left()-d < 1 && viewport.top()+d > 1 && viewport.right()+d > 1 && viewport.bottom()-d < 1) {
// // 		qDebug("no surto");
// 		qDebug("%d<>(%d, %d)(%d, %d)", d, viewport.left()-d, viewport.top()+d, viewport.right()+d, viewport.bottom()-d);
		viewport.setLeft(viewport.left() - d);
		viewport.setTop(viewport.top() + d);
		viewport.setRight(viewport.right() + d);
		viewport.setBottom(viewport.bottom() - d);
// 		qDebug("(%d, %d)-(%d, %d)", viewport.left(), viewport.top(), viewport.right(),viewport.bottom());
		update_scale();
		update_points();
	}
// 	qDebug("(%d, %d)-(%d, %d)", viewport.left(), viewport.top(), viewport.right(),viewport.bottom());
}

void QGraph::mouseReleaseEvent(QMouseEvent *e){
	pushed &= ~e->button();
	this->setCursor(QCursor(Qt::CrossCursor));
	if(e->button() == Qt::LeftButton){
		if((toViewport(press) - toViewport(e->pos())).isNull())
			return;
		
		QPoint p=toViewport(e->pos())+viewport.topLeft();
		QPoint p1=toViewport(press)+viewport.topLeft();
		
		viewport = QRect(p1, p);
		if(viewport.bottom() > viewport.top()){
			int a=viewport.bottom();
			viewport.setBottom(viewport.top());
			viewport.setTop(a);
		}
		if(viewport.left() > viewport.right()){
			int a=viewport.left();
			viewport.setLeft(viewport.right());
			viewport.setRight(a);
		}
		
		update_scale();
		update_points();
	}
	this->repaint(false);
}

void QGraph::mouseMoveEvent(QMouseEvent *e){
	mark=calcImage(fromWidget(e->pos()));
	
	if(pushed & Qt::MidButton && ant != toViewport(e->pos())){
		QPoint rel = e->pos() - press - (toWidget(.5,.5)-toWidget(0.,0.));
		rel = toViewport(rel);
		viewport.setLeft(viewport.left() - rel.x()); viewport.setRight(viewport.right() - rel.x());
		viewport.setTop(viewport.top() - rel.y()); viewport.setBottom(viewport.bottom() - rel.y());
		
		update_points();
		press = e->pos();
		ant = toViewport(e->pos());
		valid=false;
	} else if (pushed == Qt::LeftButton) {
		last = e->pos();
	}
	
	this->repaint(false);
}

void QGraph::keyPressEvent ( QKeyEvent * e ){
	switch(e->key()){
		case Qt::Key_Right:
			viewport.setLeft(viewport.left() +1); viewport.setRight(viewport.right() +1);
			break;
		case Qt::Key_Left:
			viewport.setLeft(viewport.left() -1); viewport.setRight(viewport.right() -1);
			break;
		case Qt::Key_Down:
			viewport.setTop(viewport.top() -1); viewport.setBottom(viewport.bottom() -1);
			break;
		case Qt::Key_Up:
			viewport.setTop(viewport.top() +1); viewport.setBottom(viewport.bottom() +1);
			break;
		case Qt::Key_Minus:
			resolucio=(resolucio*viewport.width())/(viewport.width()+2);
			viewport.setCoords(viewport.left() - 1, viewport.top() +1, viewport.right() + 1, viewport.bottom() -1);
			update_scale();
			break;
		case Qt::Key_Plus:
			if(viewport.height() < -3 && viewport.width() > 3){
				resolucio=(resolucio*viewport.width())/(viewport.width()-2);
				viewport.setCoords(viewport.left() + 1, viewport.top() -1, viewport.right() -1, viewport.bottom() +1);
				update_scale();
			} else return;
			break;
		default:
			return;
	}
	valid=false;
	update_points();
	this->repaint(false);
}

QDoublePoint QGraph::calcImage(QDoublePoint dp){
	if(funclist.count()>0){
		for (QValueList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
			if((*it).selected()) {
				dp=(*it).calc(dp);
				break;
			}
		}
	}
	return dp;
}


void QGraph::unselect(){
	if(funclist.count()>0){
		for (QValueList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
			(*it).setSelected(false);
		}
	}
}

void QGraph::update_points(){
	if(funclist.count()>0){
		for (QValueList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
			(*it).update_points(viewport, resolucio);
		}
		valid=false;
	}
}

void QGraph::pintafunc(QPaintDevice *qpd){
	QPoint ultim(0,0), act(0,0);
	QPen pfunc;
	QPen ccursor;
	buffer = QPixmap(this->width(), this->height());
	buffer.fill(Qt::white);
	
	pfunc.setColor(QColor(0,150,0));
	pfunc.setCapStyle(Qt::RoundCap);
	pfunc.setWidth(2);
	
	if(qpd)
		finestra.begin(qpd);
	else
		finestra.begin(&buffer, this);
	
	bool nan=false, outside=false;
	dibuixa_eixos(&finestra);
	finestra.setPen(pfunc);
	
	if(funclist.count()>0){
		for (QValueList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
			if((*it).isShown()){
			pfunc.setColor((*it).color());
			pfunc.setWidth((*it).selected()+1);
			finestra.setPen(pfunc);
			int i = (*it).npoints();
			register int j;
			for(/*register int*/ j=0; j<i;j++){
				if(!nan){
					act=toWidget((*it).points[j].x(), (*it).points[j].y(), &nan);
// 					qDebug("%4d -- %4d", act.y(), act.x());//y->2
					
					if(act.x()!=ultim.x() && act.x() > 0 && (act.y() > 0 || (act.y()==0 && !outside)) && act.x() < this->width() && act.y() <= this->height()){
						finestra.drawLine(ultim, act);
						outside=false;
					} else if(act.x() > 0 && act.x() < this->width() && ((act.y() < 0 && ultim.y()>0)  || (act.y() > this->height() && ultim.y() < this->height()))){
						bool out;
						QPoint act2 = act;
						trunca(&ultim, &act2, &out);
						if(!outside)
							finestra.drawLine(ultim, act2);
						outside = out;
					}
				}else
					act=toWidget((*it).points[j].x(), (*it).points[j].y(), &nan);
				
				if(act.x() != ultim.x())
					ultim=act;
			}
			}
		}
	}
	valid=true;
	finestra.end();
}

void QGraph::trunca(QPoint *ultim, QPoint *act, bool* outside){
	*outside=true;
	if(ultim->y() < 0){
		double relation = ((double)ultim->x()-act->x())/((double)ultim->y()-act->y());
		ultim->setX(relation!=0.0? ultim->x() + round(relation*act->y()) : act->x());
		ultim->setY(0);
	} else if(act->y()<0){
		double relation = ((double)(act->x()-ultim->x()))/((double)(act->y()-ultim->y())*-1);
		act->setX(round(ultim->x() + ultim->y()*relation));
		act->setY(0);
	} else if(act->y() > this->height()) {
		act->setX(ultim->x()+1);
		act->setY(this->height());
	}/* else if(ultim->y() > this->height()) {
// 		double relation = ((double) (act->x()-ultim->x()))/((double)(act->y()-ultim->y()));
		qDebug("4. %f", 0.);
	}*/
}

bool QGraph::addFunction(const function& func){
	bool exist=false;
	QTime t;
	
	for (QValueList<function>::iterator it = funclist.begin(); it != funclist.end() && !exist; ++it ){
		if((*it).expression() == func.expression()){
			exist=true;
			(*it)=func;
		}
	}
	
	if(!exist)
		funclist << func;
	update_points();
	this->repaint(false);
	return exist;
}

bool QGraph::editFunction(const QString& tochange, const function& func){
	bool exist=false;
// 	qDebug("<<%s>>", tochange.ascii());
	for (QValueList<function>::iterator it = funclist.begin(); it != funclist.end() && !exist; ++it ){
		if((*it).expression() == tochange){
			exist=true;
			(*it)=func;
		}
	}
	
	update_points();
	this->repaint(false);
	return false;
}

bool QGraph::editFunction(const unsigned int& num, const function& func){
	if(num<funclist.count())
		funclist[num]=func;
	update_points();
	this->repaint(false);
	return true;
}

bool QGraph::setSelected(const QString& exp){
	for (QValueList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
		(*it).setSelected((*it).expression() == exp);
	}
	
	update_points();
	this->repaint(false);
	return true;
}

bool QGraph::setShown(const QString& exp, const bool& shown){
	for (QValueList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
		if((*it).expression() == exp){
			(*it).setShown(shown);
		}
	}
	
	update_points();
	this->repaint(false);
	return true;
}

QPoint QGraph::toWidget(double coord_x, double coord_y, bool* nan){
	double part_negativa_x = -viewport.left();
	double part_negativa_y = -viewport.top();
	int y=0, aux=0;
	
	if(nan==0)
		y= round((part_negativa_y + coord_y ) * rang_y);
	else{
		if((aux = isinf(coord_y))!= 0) {
			y = (aux<0)? y=this->height() : y=0;
			*nan=true;
		} else if(isnan(coord_y) && nan!=0){
			*nan=true;
		} else {
			y=round((part_negativa_y + coord_y ) * rang_y);
			*nan=false;
		}
	}
	
	QPoint orig (round((part_negativa_x + coord_x) * rang_x),  round(y));
	return orig;
}

QDoublePoint QGraph::fromWidget(QPoint p){
	double part_negativa_x = -viewport.left();
	double part_negativa_y = -viewport.top();
	QDoublePoint p1;
	p1.setX(p.x()/rang_x-part_negativa_x);
	p1.setY(p.y()/rang_y-part_negativa_y);
	return p1;
}

QPoint QGraph::toViewport(const QPoint &mv){
	return QPoint((int) ceil(mv.x()/rang_x), (int) ceil(mv.y()/rang_y));
}

void QGraph::setResolution(int res) { resolucio = res; update_points(); }
void QGraph::setViewPort(QRect vp)  { viewport  = vp;  update_points(); }

void QGraph::resizeEvent(QResizeEvent *){
	buffer.resize(this->size());
	update_scale();
	pintafunc(NULL);
}

//////////////////////////////////////////////////////////////
bool QGraph::toImage(QString path){
	if(path != "" && path.endsWith(".svg")) {
		QPicture pic;
		QPainter  p;
// 		rang_x=(double) this->width()/((double)viewport.width()-1);
// 		rang_y=(double) this->height()/((double)viewport.height()-1);
		pintafunc(&pic);
		pic.save(path, "svg");
	} else if(path != "" && path.endsWith(".png")) {
		front.save(path, "PNG");
	} else
		return false;
	
	return true;
}
//////////////////////////////////////////////////////////////

#include "qgraph.moc"
