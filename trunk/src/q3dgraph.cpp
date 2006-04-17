#include "q3dgraph.h"

Q3DGraph::Q3DGraph(QWidget *parent, const char *name) : QGLWidget(parent, name){
	this->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
	tefunc=false;
	trans=false;
	graus[0] = 90.0;
	graus[1] = 0.0;
	graus[2] = 0.0;
	z = -35.0;
	default_step = 0.2f;
	default_size = 8.0f;
	zoom = 1.0f;
	punts=0; //If I don't put this, the program crashes
	keyspressed=0;
	method = G_SOLID;
	this->setFocusPolicy(QWidget::WheelFocus);
// 	setMouseTracking(true);
}


Q3DGraph::~Q3DGraph(){
	if(punts!=NULL)
		delete [] punts;
}

void Q3DGraph::initializeGL() {
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	QImage t, b;
	GLuint texture[1];
	b = QImage( 8, 8, 32);
	b.fill( Qt::white.rgb() );
	for(int i=0; i<b.height(); i++)
		b.setPixel(0, i, 0);
	
	for(int i=0; i<b.width(); i++)
		b.setPixel(i, 0, 0);
		
	t = QGLWidget::convertToGLFormat( b );
	glGenTextures( 1, &texture[0] );
	glBindTexture( GL_TEXTURE_2D, texture[0] );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Q3DGraph::resizeGL( int width, int height ) {
	height = height?height:1;
	
	glViewport( 0, 0, (GLint)width, (GLint)height );
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Q3DGraph::mousePressEvent(QMouseEvent *e){
	if(e->button() == Qt::LeftButton){
		press = e->pos(); keyspressed |= LCLICK;
	}
}

void Q3DGraph::mouseReleaseEvent(QMouseEvent *e){
	if(e->button() == Qt::LeftButton)
		keyspressed &= ~LCLICK;
}

void Q3DGraph::mouseMoveEvent(QMouseEvent *e){
	if(keyspressed & LCLICK){
		QPoint rel = e->pos() - press;
		graus[0] += rel.y();
		graus[2] += -rel.x();
		graus[1] += 0.0;
		glRotatef(100.0f, graus[0], graus[1], graus[2]);
		this->repaint();
		
		press = e->pos();
	}
}

void Q3DGraph::dibuixa_eixos(){
	glColor3f(0.8, 0.8, 0.4);
	this->renderText(11.0, 0.0, 0.0, "X");
	this->renderText(0.0, 0.0,-11.0, "Y");
	this->renderText(0.0, 11.0, 0.0, "Z");
	
	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-10.0f, 0.0f, 0.0f);
			glVertex3f( 10.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f( 0.0f, 10.0f, 0.0f);
			glVertex3f( 0.0f,-10.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f( 0.0f, 0.0f, 10.0f);
			glVertex3f( 0.0f, 0.0f,-10.0f);
	glEnd();
}

void Q3DGraph::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if(keyspressed & KEYUP)		graus[0]-=3.f;
	if(keyspressed & KEYDOWN)	graus[0]+=3.f;
	if(keyspressed & KEYAVPAG)	graus[1]+=3.f;
	if(keyspressed & KEYREPAG)	graus[1]-=3.f;
	if(keyspressed & KEYLEFT)	graus[2]+=3.f;
	if(keyspressed & KEYRIGHT)	graus[2]-=3.f;
	if(keyspressed & KEYW)		z/=1.1f;
	if(keyspressed & KEYS)		z= z!=0. ? z*1.1f : .1f;
	if(keyspressed & KEYQ)		{ zoom/=2.0f; crea(); }
	if(keyspressed & KEYE)		{ zoom*=2.0f; crea(); }
	
	graus[0] = graus[0]>=360.f ? graus[0]-360.f : graus[0];
	graus[1] = graus[1]>=360.f ? graus[1]-360.f : graus[1];
	graus[2] = graus[2]>=360.f ? graus[2]-360.f : graus[2];
	
	
	glTranslatef(0.0f, 0.0f, z);
	glRotatef(graus[0], 1.0, 0.0, 0.0);
	glRotatef(graus[1], 0.0, 1.0, 0.0);
	glRotatef(graus[2], 0.0, 0.0, 2.0);
	double mida=default_size*zoom, step=default_step*zoom;
	dibuixa_eixos();
	int i,j;
	
	if(method == G_POINTS || method == G_LINES) {
		if(method == G_POINTS){	
			glBegin(GL_POINTS);
		} else if(method == G_LINES){
			glBegin(GL_LINES);
		}
		for(i=0; tefunc && i<(2*mida/step)-1; i++) {
			for(j=0; tefunc && j<2*mida/step-1; j++) {
				if(method == G_POINTS){
					glColor3d( i*step/mida, j*step/mida, punts[i][j]/5);
					glVertex3d( i*step-mida, j*step - mida, punts[i][j]);
				} else {
					if(method == G_LINES)
						glColor3d( i*step/mida, j*step/mida, punts[i][j]/5);
					else
						glColor3d( 0.0,0.0,0.0);
					glVertex3d( i*step-mida, j*step - mida, punts[i][j]);
					glVertex3d( (i?i-1:i)*step-mida, j*step - mida, punts[i?i-1:i][j]);
					
					glVertex3d( i*step-mida, j*step - mida, punts[i][j]);
					glVertex3d( i*step-mida, (j?j-1:j)*step - mida, punts[i][j?j-1:j]);
				}
			}
		}
		glEnd();
	} else if(method == G_SOLID){
		
		if(trans){
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		}else{
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
		
		double trans=0.8;
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		for(i=0; tefunc && i<(2*mida/step)-2; i++) {
			for(j=0; tefunc && j<2*mida/step-2; j++) {
				if(abs(punts[i][j]-punts[i][j+1])>700000. || abs(punts[i][j]-punts[i+1][j])>700000.){
// 					qDebug("assimptota %f %f", punts[i][j], punts[i][j+1]);
				} else {
				if(punts[i][j]>300.)
					qDebug("%f %f %f %f", punts[i][j], punts[i+1][j], punts[i][j+1], punts[i+1][j+1]);
				
				glTexCoord2f(0.0f, 1.0f);
				glColor4d(   i*step/mida/2, (j+1)*step/mida/2, punts[i][j+1]/5,  trans);
				glVertex3d(    i*step-mida, (j+1)*step - mida, punts[i][j+1]);
				
				
				glTexCoord2f(0.0f, 0.0f);
				glColor4d(   i*step/mida/2,     j*step/mida/2, punts[i][j]/5,    trans);
				glVertex3d(    i*step-mida,     j*step - mida, punts[i][j]);
				
				glTexCoord2f(1.0f, 0.0f);
				glColor4d((i+1)*step/mida/2,    j*step/mida/2, punts[i+1][j]/5,  trans);
				glVertex3d( (i+1)*step-mida,    j*step - mida, punts[i+1][j]);
				
				glTexCoord2f(1.0f, 1.0f);
				glColor4d((i+1)*step/mida/2, (j+1)*step/mida/2, punts[i+1][j+1]/5,trans);
				glVertex3d( (i+1)*step-mida, (j+1)*step - mida, punts[i+1][j+1]);
				}
			}
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	glFlush();
}

void Q3DGraph::crea() {
	double mida=default_size*zoom, step=default_step*zoom;
	int i, j;
	const int k=(int) 2*mida/step;
	QDomElement *x, *y;
	
	QTime t;
	t.restart();
	func3d.vars.modifica("x", 0);
	func3d.vars.modifica("y", 0);
	x=func3d.vars.find("x");
	y=func3d.vars.find("y");
	
	for(i=0; tefunc && i<k; i++) {
		*x= Analitza::toCn(i*step-mida);
		for(j=0; tefunc && j<k; j++) {
			*y= Analitza::toCn(j*step-mida);
			punts[i][j] = -1.*func3d.Calcula();
		}
	}
	qDebug(".                                     Lasts: %d ms", t.elapsed() );
}


void Q3DGraph::keyPressEvent( QKeyEvent *e ) {
	switch(e->key()) {
		case Qt::Key_Up:
			keyspressed |= KEYUP;
			break;
		case Qt::Key_Down:
			keyspressed |= KEYDOWN;
			break;
		case Qt::Key_Left:
			keyspressed |= KEYLEFT;
			break;
		case Qt::Key_Right:
			keyspressed |= KEYRIGHT;
			break;
		case Qt::Key_Prior:
			keyspressed |= KEYREPAG;
			break;
		case Qt::Key_Next:
			keyspressed |= KEYAVPAG;
			break;
		case Qt::Key_W:
			keyspressed |= KEYW;
			break;
		case Qt::Key_S:
			keyspressed |= KEYS;
			break;
		case Qt::Key_Q: //Be careful
			keyspressed |= KEYQ;
			break;
		case Qt::Key_E: //Be careful
			keyspressed |= KEYE;
			break;
	}
	this->repaint();
}

void Q3DGraph::keyReleaseEvent( QKeyEvent *e ){
	switch(e->key()) {
		case Qt::Key_Up:
			keyspressed &= ~KEYUP;
			break;
		case Qt::Key_Down:
			keyspressed &= ~KEYDOWN;
			break;
		case Qt::Key_Left:
			keyspressed &= ~KEYLEFT;
			break;
		case Qt::Key_Right:
			keyspressed &= ~KEYRIGHT;
			break;
		case Qt::Key_Prior:
			keyspressed &= ~KEYREPAG;
			break;
		case Qt::Key_Next:
			keyspressed &= ~KEYAVPAG;
			break;
		case Qt::Key_W:
			keyspressed &= ~KEYW;
			break;
		case Qt::Key_S:
			keyspressed &= ~KEYS;
			break;
		case Qt::Key_Q: //Be careful
			keyspressed &= ~KEYQ;
			break;
		case Qt::Key_E: //Be careful
			keyspressed &= ~KEYE;
			break;
		
	}
	this->repaint();
}

void Q3DGraph::timeOut(){
	graus[0] += 20.0;
	graus[1] += 20.0;
	graus[2] += 20.0;
	this->repaint();
}

void Q3DGraph::timeOutSlot(){
	timeOut();
	this->repaint();
}

int Q3DGraph::setFunc(QString Text){
	mem();
	
	int ret = func3d.setText(Text);
	tefunc=true;
	crea();
	this->repaint();
	return ret;
}

void Q3DGraph::mem(){
	if(punts!=NULL)
		return;
	int j= (int) 2*default_size/default_step;
	punts = new double* [j];
	for(int i=0; i<j; i++)
		punts[i] = new double[j];
	
}

int Q3DGraph::setFuncMML(QString TextMML){
	mem();
	
	int ret = func3d.setTextMML(TextMML);
	tefunc=true;
	crea();
	this->repaint();
	return ret;
}

void Q3DGraph::setMida(double newSize){
	default_size = newSize;
	this->repaint();
}
void Q3DGraph::setStep(double newRes){
	default_step = newRes;
	this->repaint();
}
void Q3DGraph::setZ(float coord_z){
	z = coord_z;
	this->repaint();
}

void Q3DGraph::setMethod(int m){
	method = m;
	this->repaint();
}

void Q3DGraph::setTraslucency(bool tr){
	trans = tr;
}

QPixmap Q3DGraph::toPixmap(){
	return this->renderPixmap();
}

#include "q3dgraph.moc"
