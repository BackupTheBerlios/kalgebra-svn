#include "qgraph.h"

QGraph::QGraph(QWidget *parent, const char *name) : QWidget(parent, name, Qt::WStyle_NormalBorder) {
	this->setMinimumHeight(150);
	this->setMinimumWidth(100);
	this->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
	QColor cfons;
	cfons.setRgb(255,255,255);
	this->setPaletteBackgroundColor(cfons);
	resolucio=32;
	
	viewport = QRect::QRect(QPoint::QPoint(-12, -10), QPoint::QPoint(12, 10));
	this->setFocusPolicy(QWidget::WheelFocus);
	micepos = new QLabel("lol", this);
	micepos->setFrameShape(QFrame::Box);
// 	micepos->setPaletteBackgroundColor(QColor(255,255,170));
	micepos->setPaletteBackgroundColor(QColor(255,230,255));
	micepos->setAlignment(AlignAuto | AlignVCenter | AlignHCenter);
	micepos->hide();
	this->setCursor(QCursor(Qt::CrossCursor));
	setMouseTracking(true);
}

QGraph::~QGraph() {
	finestra.flush();
}

void QGraph::dibuixa_eixos(QPainter *finestra){
	QColor ceixos;
	QPoint centre;
	ceixos.setRgb(100,100,255);
	
	finestra->setPen(ceixos);
	double x;
	
	//dibuixa eixos viewport
	finestra->drawLine(toWidget(viewport.left(),0), QPoint(this->width(), toWidget(viewport.right()+1, 0).y()));
	finestra->drawLine(	QPoint( toWidget(0,viewport.top()).x(), this->height()),
				QPoint( toWidget(0,viewport.bottom()+1).x(), 0));
	for(x=viewport.left(); x<viewport.right()+2; x+=1.0f) // ralletes X
		finestra->drawLine(toWidget(x,0.0f), toWidget(x,0.0f)+QPoint(0,-3));
	
	for(x=viewport.top(); x<viewport.bottom(); x+=1.0f) // ralletes y
		finestra->drawLine(toWidget(0.0f,x), toWidget(0.0f,x)+QPoint(3,0));
	
	//dibuixa eixos viewport
}


void QGraph::paintEvent( QPaintEvent * ){
	rang_x=(double) this->width()/((double)viewport.width()-1.0);
	rang_y=(double) this->height()/((double)viewport.height()-1);
	pintafunc(NULL);
}

void QGraph::mousePressEvent(QMouseEvent *e){
	if(e->button() == Qt::LeftButton || e->button() == Qt::MidButton){
		press = e->pos();
	}
}

void QGraph::wheelEvent(QWheelEvent *e){
	if(viewport.left() < -1 && viewport.top() > 1 && viewport.right() > 1 && viewport.bottom() < -1){
		int d = e->delta()>0 ? 1 : -1;
		viewport.setLeft(viewport.left() - d);
		viewport.setTop(viewport.top() - d);
		viewport.setRight(viewport.right() + d);
		viewport.setBottom(viewport.bottom() + d);
		update_points();
	}
	qDebug("(%d, %d)-(%d, %d)", viewport.left(), viewport.top(), viewport.right(),viewport.bottom());
}

void QGraph::mouseReleaseEvent(QMouseEvent *e){
	if(e->button() == Qt::LeftButton){
		if((toViewport(press) - toViewport(e->pos())).isNull())
			return;
		QPoint p=toViewport(e->pos())+viewport.topLeft(), p1=toViewport(press)+viewport.topLeft();
		if(p.x()<p1.x())
			viewport = QRect(p, p1);
		else
			viewport = QRect(p1, p);
		qDebug("(%4d, %4d)-(%4d, %4d)<<<<", press.x(), press.y(), e->pos().x(), e->pos().y());
		qDebug("(%4d, %4d)-(%4d, %4d)>>>>", viewport.left(), viewport.top(), viewport.right(),viewport.bottom());
		update_points();
	} else if(e->button() == Qt::MidButton){
		QPoint rel;
		rel = e->pos() - press;
		rel = toViewport(rel);
		viewport.setLeft(viewport.left() - rel.x()); viewport.setRight(viewport.right() - rel.x());
		viewport.setTop(viewport.top() + rel.y()); viewport.setBottom(viewport.bottom() + rel.y());
		update_points();
	}
}

void QGraph::mouseMoveEvent(QMouseEvent *e){
	QDoublePoint p=fromWidget(e->pos());
	micepos->setText(QString("<qt>x=<b>%1</b> &nbsp; y=<b>%1<b></qt>").arg(p.x(),3,'f',2).arg(p.y(),3,'f',2));
	micepos->setGeometry(e->pos().x()+13, e->pos().y()+5, 150, 23);
	micepos->show();
}

void QGraph::keyPressEvent ( QKeyEvent * e ){
	switch(e->key()){
		case Qt::Key_Right:
			viewport.setLeft(viewport.left() +1 ); viewport.setRight(viewport.right() +1 ); 
			update_points();
			break;
		case Qt::Key_Left:
			viewport.setLeft(viewport.left() - 1 ); viewport.setRight(viewport.right() - 1 ); 
			update_points();
			break;
		case Qt::Key_Down:
			viewport.setTop(viewport.top() -1 ); viewport.setBottom(viewport.bottom() - 1 ); 
			this->repaint();
			break;
		case Qt::Key_Up:
			viewport.setTop(viewport.top() +1  ); viewport.setBottom(viewport.bottom() + 1 ); 
			this->repaint();
			break;
		case Qt::Key_Minus:
			viewport.setCoords(viewport.left() - 1, viewport.top() - 1, viewport.right() + 1, viewport.bottom() + 1);
			update_points();
			break;
		case Qt::Key_Plus:
			if(viewport.height() > 3 && viewport.width() > 3){
				qDebug("(%d, %d)", viewport.height(), viewport.width());
				viewport.setCoords(viewport.left() + 1, viewport.top() + 1, viewport.right() - 1, viewport.bottom() - 1);
			}
			update_points();
			break;
		
	}
}

void QGraph::update_points(){
	if(funclist.count()>0){
		for (QValueList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
			(*it).update_points(viewport, resolucio);
		}
	}
	this->repaint();
}

void QGraph::pintafunc(QPaintDevice *qpd){
// 	resolucio = 1.90;
// 	float zoom=50.0f;
	QPoint ultim(0,0), act(0,0);
	QPen pfunc;
	pfunc.setColor(QColor(0,150,0));
// 	pfunc.setWidth(2);
	pfunc.setCapStyle(Qt::RoundCap);
	
	if(qpd)
		finestra.begin(qpd);
	else
		finestra.begin(this);
	dibuixa_eixos(&finestra);
	finestra.setPen(pfunc);
	
	int i=1+((viewport.left()-1)*-1+viewport.right()+1)*resolucio-1;
	bool nan=false;
	bool outside=false;
	
	if(funclist.count()>0){
		for (QValueList<function>::iterator it = funclist.begin(); it != funclist.end(); ++it ){
			pfunc.setColor((*it).color());
			finestra.setPen(pfunc);
			for(int j=0; j<i;j++){
				if(!nan){
					act=toWidget((*it).points[j].x(), (*it).points[j].y(), &nan);
					if(act.x() > 0 && (act.y() > 0 || (act.y()==0 && !outside)) && act.x() < this->width() && act.y() <= this->height()){
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
				ultim=act;
			}
		}
	}
	finestra.end();
}

void QGraph::trunca(QPoint *ultim, QPoint *act, bool* outside){
	*outside=true;
	if(ultim->y()<0){
		double relation = ((double)ultim->x()-act->x())/((double)ultim->y()-act->y());
		ultim->setX(relation!=0.0? ultim->x() + round(relation*act->y()) : act->x());
		ultim->setY(0);
// 		qDebug("1. %f", relation);
	} else if(act->y()<0){
		double relation = ((double)(act->x()-ultim->x()))/((double)(act->y()-ultim->y())*-1);
		act->setX( round(ultim->x() + ultim->y()*relation));
		act->setY(0);
	} else if(act->y() > this->height()) {
//		double relation = ((double) (act->x()-ultim->x()))/((double)(act->y()-ultim->y()));
//		qDebug("1(%d, %d)-(%d, %d)>>>%f", act->x(), act->y(), ultim->x(), ultim->y(), relation);
//		act->setX( ultim->x()+relation/ultim->y());
		act->setX( ultim->x()+1);
		act->setY(this->height());
//		qDebug("2(%d, %d)-(%d, %d)", act->x(), act->y(), ultim->x(), ultim->y());
	} /*else if(ultim->y() > this->height()) {
		double relation = ((double) (act->x()-ultim->x()))/((double)(act->y()-ultim->y()));
		qDebug("4. %f", relation);
	}*/
}

int QGraph::setFunc(QStringList text, QValueList<QColor> colors){
	funclist.clear();
	if(text.count() == 0){
		update_points();
		return -1;
	}
	int i=0;
	for ( QStringList::Iterator it = text.begin(); it != text.end(); ++it ) {
		funclist.append(function(*it, colors[i++]));
	}
	update_points();
	return 0;
}

QPoint QGraph::toWidget(double coord_x, double coord_y, bool* nan){
	double part_negativa_x = -viewport.left();
	double part_negativa_y = viewport.bottom();
	int y=0, aux=0;
// 	qDebug("%f, %f %x", coord_x, coord_y, nan);
	
	if(nan==0)
		y= round((part_negativa_y + coord_y*-1 ) * rang_y);
	else{
		if((aux = isinf(coord_y))!= 0) {
			y = (aux<0)? y=this->height() : y=0;
			*nan=true;
		} else if(isnan(coord_y) && nan!=0){
			*nan=true;
		} else {
			y=round((part_negativa_y + coord_y*-1 ) * rang_y);
			*nan=false;
		}
	}
	
	QPoint orig (round((part_negativa_x + coord_x) * rang_x),  round(y));
	return orig;
}

QDoublePoint QGraph::fromWidget(QPoint p){
	double part_negativa_x = -viewport.left();
	double part_negativa_y = viewport.bottom();
	QDoublePoint p1;
	p1.setX(p.x()/rang_x-part_negativa_x);
	p1.setY(-1*(p.y()/rang_y-part_negativa_y));
	return p1;
}

QPoint QGraph::toViewport(const QPoint &mv){
	return QPoint((int) ceil(mv.x()/rang_x), (int) ceil(mv.y()/rang_y));
}

void QGraph::setResolution(int res) { resolucio = res; update_points(); }
void QGraph::setViewPort(QRect vp)  { viewport  = vp;  update_points(); }

//////////////////////////////////////////////////////////////
bool QGraph::toImage(QString path){
	if(path != "") {
		QPicture pic;
		QPainter  p;
		rang_x=this->width()/(viewport.width()+1);
		rang_y=this->height()/(viewport.height()+1);
		pintafunc(&pic);
		pic.save(path, "svg");
	} else
		return false;
	
		return true;
}
//////////////////////////////////////////////////////////////

#include "qgraph.moc"
