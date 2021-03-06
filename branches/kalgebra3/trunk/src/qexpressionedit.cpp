#include "qexpressionedit.h"

QExpressionEdit::QExpressionEdit(QWidget *parent, const char *name, Mode inimode)
	: QTextEdit(parent, name), m_histPos(0), help(true), m_auto(true), a(NULL)
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setMargin(2);
	this->setFixedHeight(QFontMetrics(this->currentFont()).height()+12);
	this->setVScrollBarMode(QScrollView::AlwaysOff);
	this->setHScrollBarMode(QScrollView::AlwaysOff);
	this->setTabChangesFocus(true);
	this->setTextFormat(Qt::PlainText);
	
	m_history.append("");
	
	m_helptip = new QLabel("", this, "lol", WType_Popup);
	m_helptip->setFrameShape(QFrame::Box);
	m_helptip->setPaletteBackgroundColor(QColor(255,230,255));
	m_helptip->setAlignment(AlignAuto | AlignVCenter | AlignHCenter);
	m_helptip->setFocusPolicy(NoFocus);
	//m_helptip->setGeometry(QRect(this->mapFromGlobal(QPoint(100,0)), QPoint(160, 23)));
	m_helptip->setGeometry(QRect(QPoint(100,0), QPoint(260, 23)));
	//m_helptip->show();
	
	m_highlight= new QAlgebraHighlighter(this);
	
	setMode(inimode);
	connect(this, SIGNAL(returnPressed()), this, SLOT(returnP()));
	connect(this, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(cursorMov(int, int)));
	connect(this, SIGNAL(signalHelper(const QString&)), this, SLOT(ajudant(const QString&)));
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
			return Analitza::isMathML(this->text());
	}
}

void QExpressionEdit::setMode(Mode en)
{
	if(isMathML() && en==Expression) { //We convert it into MathML
		Analitza a;
		a.setTextMML(this->text());
		
		this->setText(Analitza::treu_tags(a.toString()));
	} else if(!isMathML() && en==MathML) {
		QExp e(this->text());
		e.parse();
		//if(a.error() != "") {...}
		
		this->setText(e.mathML());
	}
	m_highlight->setMode(en);
}

void QExpressionEdit::returnP()
{
	removenl();
	if(!this->text().isEmpty()) {
		m_history.last() = this->text();
		m_history.append("");
		m_histPos=m_history.count()-1;
	}
}

void QExpressionEdit::keyPressEvent(QKeyEvent * e)
{
	bool ch=false;
	
	if(e->key()==Qt::Key_Backtab) {
		setMode(isMathML() ? Expression : MathML);
		return;
	} else if(e->key()==Qt::Key_F2){
		Analitza a;
		if(isMathML())
			a.setTextMML(text());
		else
			a.setText(text());
		a.simplify();
		this->setText(Analitza::treu_tags(a.toString()));
		this->setModified(true);
		return;
	}
	
	switch(e->key()){
		/*case Qt::Key_Return:
		case Qt::Key_Enter:
			returnP();*/
		case Qt::Key_Up:
			m_histPos++;
			ch=true;
			break;
		case Qt::Key_Down:
			m_histPos--;
			ch=true;
			break;
		case Qt::Key_Plus: case Qt::Key_Asterisk: case Qt::Key_Slash:
			if(this->text().length() == this->selectedText().length() && m_auto) {
				this->setText("ans");
				this->setCursorPosition(0, 3);
			}
		default:
			QTextEdit::keyPressEvent(e);
			m_history.last() = this->text();
			break;
	}
	
	if(ch){
		if(m_histPos<0) m_histPos=0;
		if(m_histPos>=m_history.count()) m_histPos=m_history.count()-1;
		this->setText(m_history[m_histPos]);
	}
	
}

QString QExpressionEdit::findPrec(const QString& exp, int &act, int cur, int &param, QString tit)
{
	QString paraula=tit, p;
	int nparams=0;
	int cat=0;
	if(!tit.isNull()) cat=1;
	
// 	qDebug("####in####%s", tit.ascii());
	
	for(; act<cur || act<exp.length(); ++act){
		if(exp.at(act).isLetter()) {
			paraula=QString::null;
			for(; exp[act].isLetter(); act++)
				paraula += exp[act];
			
			
			if(exp[act]=='(' && act<cur) {//This is a function
				int param_rec=0;
				
				act++;
				p=findPrec(exp, act, cur, param_rec, paraula);
				
				if(param_rec != -1){
					param = param_rec;
// 					qDebug("****out2***<%s> %s %s", p.ascii(), paraula.ascii(), tit.ascii());
					return p;
				}
			} else act--;
		} else if(exp.at(act) == ',') {
			nparams++;
		} else if(exp.at(act) == '(') {
			cat++;
		} else if(exp.at(act) == ')') {
			cat--;
// 			qDebug("cat: %d", cat);
			if(cat == 0) {
				param=-1; //Means this is a useless func
				return QString::null;
			} else if(cat <0){
				param=-3;
				return QString::null;
			}
		}
// 		qDebug("word: %s", exp.mid(act, exp.length()-act).ascii());
	}
	param=nparams;
	
// 	qDebug("out: %d -- %d", param, cur-act);
// 	qDebug("####out####%s %s <%s>", p.ascii(), paraula.ascii(), tit.ascii());
	return tit;
}

QString QExpressionEdit::editingWord(int pos, int &param)
{ //simplification use only
	int p=0;
	param=0;
	return findPrec(this->text().mid(0,pos), p, pos, param, "");
}

void QExpressionEdit::cursorMov(int, int pos)
{
	int param;
	QString s = editingWord(pos, param);
	helpShow(s, param);
}

void QExpressionEdit::helpShow(const QString& funcname, unsigned int param)
{
	int op = Analitza::isOperador(funcname);
	if(op) {
		if(op == -1) {
			emit signalHelper(QString("<qt><em>%1</em>(..., <b>par%2</b>, ...)").arg(funcname).arg(param+1));
		} else {
			QString sample = (param < op) ? 
						QString("<qt><em>%1</em>(").arg(funcname) :
						QString("<qt text='red'><em>%1</em>(").arg(funcname);
			for(int i=0; i<op; ++i) {
				if(i==param)
					sample += QString("<b>par%1</b>").arg(i+1);
				else
					sample += QString("par%1").arg(i+1);
				if(i<op-1) sample+= ", ";
			}
			emit signalHelper(sample+")</qt>");
		}
	} else if(a!=NULL && a->vars.find(funcname)!=NULL) { //if it is a function defined by the user
		QStringList params = Analitza::bvar(a->vars.value(funcname));
		
		QString sample = (param < params.count()) ? QString("<qt><em>%1</em>(").arg(funcname) : QString("<qt text='red'><em>%1</em>(").arg(funcname);
		
		for(unsigned int i=0; i<params.count(); ++i) {
			if(i==param)
				sample += QString("<b>%1</b>").arg(params[i]);
			else
				sample += QString("%1").arg(params[i]);
			if(i<params.count()-1) sample+= ", ";
		}
		emit signalHelper(sample+")</qt>");
	} else
		emit signalHelper("");
// 	qDebug(funcname.ascii());
// 	m_helptip->show();
}

void QExpressionEdit::setAutocomplete(bool a)
{
	m_auto = a;
}

bool QExpressionEdit::autocomplete()
{
	return m_auto;
}

void QExpressionEdit::removenl()
{
	this->setText(this->text().remove('\n'));
}

void QExpressionEdit::ajudant(const QString& msg)
{
	QFontMetrics fm( font() );
	int para = 0, curPos = 0;
	getCursorPosition( &para, &curPos );
	int pixelsOffset = fm.width( text(), curPos );
	pixelsOffset -= contentsX();
	QPoint pos = mapToGlobal( QPoint( pixelsOffset, height() ) );
	
	ajudant(msg, pos-QPoint(0, 50));
}

void QExpressionEdit::ajudant(const QString& msg, QPoint p)
{
	if(!msg.isEmpty()){
		QFontMetrics fm(m_helptip->font());
		m_helptip->setText(msg);
		m_helptip->setGeometry(QRect(p, p+QPoint(fm.width(Analitza::treu_tags(msg))+20, 23)));
		//qDebug("%d, %d -%s-", fm.width(msg), msg.length(), msg.latin1());
		//m_helptip->adjustSize();
		m_helptip->show();
		m_helptip->raise();
		this->setFocus();
	} else
		m_helptip->hide();
}

#include "qexpressionedit.moc"
