
#include "qexpressionedit.h"

QExpressionEdit::QExpressionEdit(QWidget *parent, const char *name, Mode inimode) : QTextEdit(parent, name), m_histPos(0), help(true)
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setMargin(2);
	this->setFixedHeight(QFontMetrics(this->currentFont()).height()+12);
	this->setVScrollBarMode(QScrollView::AlwaysOff);
	this->setHScrollBarMode(QScrollView::AlwaysOff);
	this->setTabChangesFocus(true);
	
	m_history.append("");
	
	/*m_helptip = new QLabel("", this);
	m_helptip->setFrameShape(QFrame::Box);
	m_helptip->setPaletteBackgroundColor(QColor(255,230,255));
	m_helptip->setAlignment(AlignAuto | AlignVCenter | AlignHCenter);
	m_helptip->setGeometry(QRect(this->mapFromGlobal(QPoint(0,0)), QPoint(160, 23)));
	m_helptip->hide();*/
	
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
			return isMathML(this->text());
	}
}

bool QExpressionEdit::isMathML(QString exp){
	return exp.stripWhiteSpace()[0]=='<';
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
	m_history.last() = this->text();
	m_history.append("");
	m_histPos=m_history.count()-1;
}

void QExpressionEdit::keyPressEvent(QKeyEvent * e)
{
	bool ch=false;
	
	if(e->key()==Qt::Key_Backtab){
		setMode(isMathML() ? Expression : MathML);
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
			
			/* else if(act>=cur) { //This was a var or an unfinnished func name
				param = -2; //If it returns -2 means it is the final var
				return paraula;
			}*/
			
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

void QExpressionEdit::cursorMov(int par, int pos)
{
	int param;
	QString s = editingWord(pos, param);
	helpShow(s, param);
}

void QExpressionEdit::helpShow(const QString& funcname, int param)
{
	int op = Analitza::isOperador(funcname);
	if(op) {
		if(op == -1) {
			emit signalHelper(QString("<qt><em>%1</em>(..., <b>par%2</b>, ...)").arg(funcname).arg(param+1));
		} else {
			QString sample = (param < op) ? QString("<qt><em>%1</em>(").arg(funcname) : QString("<qt text='red'><em>%1</em>(").arg(funcname);
			for(int i=0; i<op; ++i) {
				if(i==param)
					sample += QString("<b>par%1</b>").arg(i+1);
				else
					sample += QString("par%1").arg(i+1);
				if(i<op-1) sample+= ", ";
			}
			emit signalHelper(sample+")</qt>");
		}
	} else if (param==-2){ //in case it is a var or an unfinnished func name... Dunno what to do here yet...
// 		emit signalHelper("hola: " + funcname);
	} else if (param==-3)
		emit signalHelper("catalan error");
	else
		emit signalHelper("");
// 	qDebug(funcname.ascii());
// 	m_helptip->show();
}

void QExpressionEdit::removenl()
{
	this->setText(this->text().stripWhiteSpace().remove('\n'));
}


#include "qexpressionedit.moc"
