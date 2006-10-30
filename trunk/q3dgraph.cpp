#include "q3dgraph.h"

#if defined(Q_WS_MAC)
	#include <OpenGL/glu.h>
#else
	#include <GL/glu.h>
#endif

#include <QImage>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPixmap>
#include <QProgressDialog>
#include <QApplication>

#include "qexp.h"

Q3DGraph::Q3DGraph(QWidget *parent) : QGLWidget(parent),
		default_step(0.15f), default_size(8.0f), zoom(1.0f), punts(NULL), z(-35.),method(Solid), trans(false), tefunc(false), keyspressed(0)
{
	this->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
	this->setFocusPolicy(Qt::ClickFocus);
	graus[0] = 90.0;
	graus[1] = 0.0;
	graus[2] = 0.0;
}


Q3DGraph::~Q3DGraph()
{
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
	b = QImage(8, 8, QImage::Format_Mono);
	b.fill(QColor(0xff, 0xff, 0xff).rgb());
	for(int i=0; i<b.height(); i++)
		b.setPixel(4, i, 0);
	
	for(int i=0; i<b.width(); i++)
		b.setPixel(i, 4, 0);
		
	t = QGLWidget::convertToGLFormat( b );
	glGenTextures( 1, &texture[0] );
	glBindTexture( GL_TEXTURE_2D, texture[0] );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Q3DGraph::resizeGL( int width, int height ) {
	height = height ? height : 1;
	
	glViewport( 0, 0, (GLint)width, (GLint)height );
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Q3DGraph::mousePressEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton){
		press = e->pos(); keyspressed |= LCLICK;
	}
}

void Q3DGraph::mouseReleaseEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
		keyspressed &= ~LCLICK;
}

void Q3DGraph::mouseMoveEvent(QMouseEvent *e)
{
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

void Q3DGraph::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if(keyspressed & KEYDOWN)	graus[0]+=3.f;
	if(keyspressed & KEYUP)		graus[0]-=3.f;
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
	if(punts==NULL)
		return;
	Q_CHECK_PTR(punts);
	if(method==Dots) {
		glBegin(GL_POINTS);
		for(i=0; tefunc && i<(2*mida/step)-1; i++) {
			for(j=0; tefunc && j<2*mida/step-1; j++) {
				glColor3d( i*step/mida, j*step/mida, punts[i][j]/5);
				glVertex3d(i*step-mida, j*step-mida, punts[i][j]);
			}
		}
		glEnd();
	} else if(method == Lines) {
		glBegin(GL_LINES);
		
		for(i=0; tefunc && i<(2*mida/step)-1; i++) {
			for(j=0; tefunc && j<2*mida/step-1; j++) {
				glColor3d( i*step/mida, j*step/mida, punts[i][j]/5);
				
				glVertex3d( i*step-mida, j*step - mida, punts[i][j]);
				glVertex3d( (i?i-1:i)*step-mida, j*step - mida, punts[i?i-1:i][j]);
				
				glVertex3d( i*step-mida, j*step - mida, punts[i][j]);
				glVertex3d( i*step-mida, (j?j-1:j)*step - mida, punts[i][j?j-1:j]);
			}
		}
		glEnd();
	} else if(method == Solid) {
		if(trans){
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		} else {
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
		
		glEnable(GL_TEXTURE_2D);
		double transf=0.8;
		
		glPushMatrix();
		for(i=0; tefunc && i<(2*mida/step-2); i++) {
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(i*step-mida, -mida, 0);
			glBegin(GL_TRIANGLE_STRIP);
			for(j=0; tefunc && j<2*mida/step-1; j++) {
				glTexCoord2f(j%2 ? .0f : 1.f, 0.f);
				glColor4d((i*step-mida)/mida, (j*step-mida)/mida, 1./fabs(log10(5.+punts[i][j])), transf);
				glVertex3d(0., j*step, punts[i][j]);
				
				glTexCoord2f(j%2 ? .0f : 1.f, 1.f);
				glColor4d(((i+1)*step-mida)/mida, (j*step-mida)/mida, 1./fabs(log10(5.+punts[i+1][j])), transf);
				glVertex3d(step, j*step, punts[i+1][j]);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
	glFlush();
}

void Q3DGraph::crea() {
	double mida=default_size*zoom, step=default_step*zoom;
	const int k= static_cast<int>(2*mida/step);
	func3d.m_vars->modify("x", 0.);
	func3d.m_vars->modify("y", 0.);
	
	Cn *x=(Cn*)func3d.m_vars->value("x"), *y=(Cn*)func3d.m_vars->value("y");
	
// 	qApp->processEvents();
	Q_CHECK_PTR(punts);
	for(int i=0; tefunc && i<k; i++) {
		x->setValue(i*step-mida);
		for(int j=0; tefunc && j<k; j++) {
			y->setValue(j*step-mida);
			punts[i][j] = -func3d.calculate().value();
		}
	}
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
		case Qt::Key_PageUp:
			keyspressed |= KEYREPAG;
			break;
		case Qt::Key_PageDown:
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
// 	sendStatus(QString("-%1-").arg(keyspressed, 16));
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
		case Qt::Key_PageUp:
			keyspressed &= ~KEYREPAG;
			break;
		case Qt::Key_PageDown:
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
// 	sendStatus(QString(".%1.").arg(keyspressed, 16));
	this->repaint();
}

void Q3DGraph::timeOut(){
	graus[0] += 20.0;
	graus[1] += 20.0;
	graus[2] += 20.0;
	this->repaint();
}

int Q3DGraph::setFunc(QString Text)
{
	if(!Analitza::isMathML(Text)) {
		QExp e(Text);
		e.parse();
		Text=e.mathML();
		if(!e.error().isEmpty())
			return 0;
	}
	
	if(func3d.setTextMML(Text))
		return load();
	else
		return 0;
}

int Q3DGraph::setFuncMML(QString TextMML){
	int ret = func3d.setTextMML(TextMML);
	if(func3d.isCorrect())
		return load();
	else {
		sendStatus(i18n("Error: %1").arg(func3d.m_err.join(", ")));
		tefunc=false;
		this->repaint();
		return ret;
	}
}

int Q3DGraph::load() 
{
	func3d.m_vars->modify("x", 0.);
	func3d.m_vars->modify("y", 0.);
	func3d.calculate();
	
	if(func3d.isCorrect()) {
		QTime t;
		t.restart();
		sendStatus(i18n("Generating... Please wait"));
		mem();
		tefunc=true;
		crea();
		sendStatus(i18n("Done: %1ms").arg(t.elapsed()));
		this->repaint();
		return 0;
	} else {
		sendStatus(i18n("Error: %1").arg(func3d.m_err.join(", ")));
		tefunc=false;
		this->repaint();
		return -1;
	}
}

void Q3DGraph::mem()
{
	int j= static_cast<int>(2*default_size/default_step);
	if(punts!=NULL){
		for(int i=0; i<j; i++)
			delete punts[i];
		delete punts;
	}
	
	int midadelgrafo=0;
	punts = new double* [j];
	for(int i=0; i<j; i++){
		midadelgrafo+=sizeof(double)*j;
		punts[i] = new double[j];
	}
	Q_CHECK_PTR(punts);
	qDebug() << "Mida: " << midadelgrafo;
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

void Q3DGraph::setMethod(enum Type m){
	method = m;
	this->repaint();
}

QPixmap Q3DGraph::toPixmap(){
	return this->renderPixmap();
}

/*bool Q3DGraph::save(const KURL& url)
{
	if ( KIO::NetAccess::exists( url, false, this ) ) //The file already exist
		return false;

	QString type(KImageIO::type(url.path()));
	if (type.isNull())
		type = "PNG";

	bool ok = false;

	if(url.isLocalFile()) {
		KSaveFile saveFile(url.path());
		if ( saveFile.status() == 0 ) {
			if (toPixmap().save( saveFile.file(), type.latin1() ) )
				ok = saveFile.close();
		}
	} else {
		KTempFile tmpFile;
		tmpFile.setAutoDelete(true);
		if(tmpFile.status()==0) {
			if(toPixmap().save( tmpFile.file(), type.latin1())) {
				if(tmpFile.close())
					ok = KIO::NetAccess::upload( tmpFile.name(), url, this );
			}
		}
	}
	
//	QApplication::restoreOverrideCursor();
	
	if (!ok) {
		qDebug("Was unable to save it");
	}

	return ok;
}*/
