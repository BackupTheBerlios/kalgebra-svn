
#include "qexpressionedit.h"

QExpressionEdit::QExpressionEdit(QWidget *parent, const char *name, Mode inimode) : QTextEdit(parent, name), m_histPos(0), help(true) {
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setMargin(2);
	this->setFixedHeight(QFontMetrics(this->currentFont()).height()+12);
	this->setVScrollBarMode(QScrollView::AlwaysOff);
	this->setHScrollBarMode(QScrollView::AlwaysOff);
	
	m_history.append("");
	
	m_helptip = new QLabel("", this);
	m_helptip->setFrameShape(QFrame::Box);
	m_helptip->setPaletteBackgroundColor(QColor(255,230,255));
	m_helptip->setAlignment(AlignAuto | AlignVCenter | AlignHCenter);
	m_helptip->setGeometry(QRect(this->mapFromGlobal(QPoint(0,0)), QPoint(160, 23)));
	m_helptip->hide();
	
	m_highlight= new QAlgebraHighlighter(this);
	setMode(inimode);
	connect(this, SIGNAL(returnPressed()), this, SLOT(returnP()));
	connect(this, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(cursorMov(int, int)));
}

QExpressionEdit::~QExpressionEdit() {}

bool QExpressionEdit::isMathML()
{
	switch(m_highlight->mode()) {
		case MathML:
			return true;
		case Expression:
			return false;
		default:
			return this->text().stripWhiteSpace()[0]=='<';
	}
}

void QExpressionEdit::returnP() {
	removenl();
	m_history.last() = this->text();
	m_history.append("");
	m_histPos=m_history.count()-1;
}

void QExpressionEdit::keyPressEvent(QKeyEvent * e){
	bool ch;
	int pos, par;
	switch(e->key()){
		case Qt::Key_Up:
			m_histPos++;
			ch=true;
			break;
		case Qt::Key_Down:
			m_histPos--;
			ch=true;
			break;
		default:
			ch=false;
			break;
	}
	
	if(ch){
		if(m_histPos<0) m_histPos=0;
		if(m_histPos>=m_history.count()) m_histPos=m_history.count()-1;
		this->setText(m_history[m_histPos]);
	} else {
		QTextEdit::keyPressEvent(e);
		m_history.last() = this->text();
	}
	
}

QString findP(const QString& exp, int &act, int cur, int &param){
	QString paraula, p;
	param=0;
	int nparams=0;
	bool word;
	
	for(; act<cur || act<exp.length() ; ++act){ //get word
		qDebug("<<<<<>>>>>>>>>>><<<<<<<>>>>>><%s", paraula.ascii());
		if(exp.at(act).isLetter()) {
			for(; exp[act].isLetter(); act++)
				paraula += exp[act];
			
			if(exp[act]=='(' && act<cur){//This is a function
				int param_rec=0;
				p=findP(exp, act, cur, param_rec);
// 				qDebug("xxxxxxxxxx: %s, act: %d, cur: %d", p.ascii(), act, cur);
				
				if(!p.isNull()) {
					param = param_rec;
					return p;
				}
			} else { //This was a var
				paraula=QString::null;
				
			}
		} else if(exp.at(act) == ',') {
			nparams++;
		} else if(exp.at(act) == ')') {
			nparams = 0;
			return QString::null;
		} else
			paraula=QString::null;
	}
	qDebug(";;;;;;;;;%d", nparams);
	param=nparams;
	return paraula;
}

void QExpressionEdit::cursorMov(int par, int pos) {
// 	qDebug("...par:%d....pos:%d.......%c......", par, pos, this->text().at(pos-1).latin1());
	int nparam=0, p=0;
	QString func = findP(this->text(), p, pos, nparam);
	qDebug("------------>%s", func.ascii());
	qDebug("param: %d\n\n\n", nparam);
	helpShow(func, nparam);
	
	#if 0
	QChar last=this->text().at(pos-1);
	if(last=='('){
		int i;
// 		qDebug("in");
		for(i=pos-2; i>0 && this->text()[i].isLetter(); --i);
		helpShow(this->text().mid(i, pos-i-1));
		
// 		qDebug("out: %s", this->text().mid(i, pos-i).ascii());
	} else if(last==')')
		signalHelper("");
	#endif
}

void QExpressionEdit::helpShow(const QString& funcname, int param) {
	int op = Analitza::isOperador(funcname);
	if(op) {
		if(op == -1)
			emit signalHelper(QString("<qt><b>%1</b>(par1, ...)</qt>").arg(funcname));
		else {
			QString sample = QString("<qt><b>%1</b>(").arg(funcname);
			for(int i=0; i<op; ++i) {
				if(i==param)
					sample += QString("<em>par%1</em>").arg(i+1);
				else
					sample += QString("par%1").arg(i+1);
				if(i<op-1) sample+= ", ";
			}
			emit signalHelper(sample+")</qt>");
		}
	} else {
		emit signalHelper(QString::null);
	}
// 	qDebug(funcname.ascii());
// 	m_helptip->show();
}

void QExpressionEdit::removenl() {
	this->setText(this->text().stripWhiteSpace().remove('\n'));
}


#include "qexpressionedit.moc"
