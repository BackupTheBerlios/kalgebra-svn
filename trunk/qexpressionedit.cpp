#include "qexpressionedit.h"

#include <QLabel>
#include <QKeyEvent>
#include <QTreeView>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QHeaderView>

QExpressionEdit::QExpressionEdit(QWidget *parent, Mode inimode)
	: QTextEdit(parent), m_histPos(0), help(true), m_auto(true), a(NULL), m_check(true), m_correct(true), m_ans("ans")
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setFixedHeight(QFontMetrics(this->currentFont()).height()+12);
	this->setTabChangesFocus(true);
	this->setAcceptRichText(false);
	setAutoFormatting(AutoNone);
	m_history.append("");
	
	m_helptip = new QLabel(this,
		Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool | Qt::X11BypassWindowManagerHint);
	m_helptip->setFrameShape(QFrame::Box);
	m_helptip->setFocusPolicy(Qt::NoFocus);
	m_helptip->setAutoFillBackground(false);
	m_helptip->hide();
	QPalette palette;
	palette.setColor(m_helptip->backgroundRole(), QColor(255,230,255));
	m_helptip->setPalette(palette);
	
	m_highlight= new QAlgebraHighlighter(this->document());
	
	m_completer = new QCompleter(this);
	m_completer->setWidget(this);
	QTreeView *treeView = new QTreeView;
	m_completer->setPopup(treeView);
	treeView->setRootIsDecorated(false);
	treeView->header()->hide();
	treeView->resizeColumnToContents(1);
	treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	treeView->setMinimumWidth(300);
	updateCompleter();
	
	treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
	treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
	treeView->setColumnHidden(2, true);
	
	connect(this, SIGNAL(returnPressed()), this, SLOT(returnP()));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorMov()));
	connect(this, SIGNAL(signalHelper(const QString&)), this, SLOT(ajudant(const QString&)));
	connect(m_completer, SIGNAL(activated(const QString&)), this, SLOT(completed(const QString&)));
// 	connect(m_completer, SIGNAL(activated(const QModelIndex&)), this, SLOT(completed(const QModelIndex&)));
	
	setMode(inimode);
}

void QExpressionEdit::updateCompleter()
{
	if(!isMathML()) {
		QFile file(":/resources/operators.txt");
		if(!file.open(QFile::ReadOnly))
			return;
		QStringList words;

		for(int num_words = 0;;++num_words) {
			QByteArray line = file.readLine();
			if (line.isEmpty())
				break;
			words << line.trimmed();
		}
		
		/*if(a!=NULL) {
			qDebug("----------%s", a->m_vars->getNoms().join(", ").toAscii().data());
			words << a->m_vars->getNoms();
		}
		
		m_completer->setModel(new QStringListModel(words, m_completer));
		*/
		
		/////////////////
		int count = words.count();
		if(a!=NULL)
			count+=a->m_vars->count();
		
		QStandardItemModel *m_words = new QStandardItemModel(count, 3, m_completer);
		int i;
		for (i=0; i<words.count(); ++i) {
			m_words->setData(m_words->index(i, 0), words[i]);
			m_words->setData(m_words->index(i, 2), "op");
		}
		
		if(a!=NULL) {
			QHash<QString, Object*>::const_iterator it = a->m_vars->begin();
			for(; it != a->m_vars->end(); ++it) {
				m_words->setData(m_words->index(i, 0), it.key());
// 				m_words->setData(m_words->index(i, 1), Analitza::treu_tags(a->str(it.value())));
				m_words->setData(m_words->index(i, 2), "var");
				
				i++;
			}
		}
		m_completer->setModel(m_words);
	}
}

void QExpressionEdit::completed(const QString& newText)
{
	int c = newText.length() - lastWord(textCursor().selectionStart()).length();
	QString toInsert=newText.right(c);
	if(Analitza::whatType(newText) == Object::oper)
		toInsert += "(";
	insertPlainText(toInsert);
}

/*void QExpressionEdit::completed(const QModelIndex& index)
{
	QStandardItem *it = m_words->itemFromIndex(index);
	qDebug() << it;
	if(it!=NULL) {
		QString newText = it->data(0).toString();
		int c = newText.length() - lastWord(textCursor().selectionStart()).length();
		insertPlainText(newText.right(c));
	}
}*/

QExpressionEdit::~QExpressionEdit() {}

bool QExpressionEdit::isMathML() const
{
	switch(m_highlight->mode()) {
		case MathML:
			return true;
		case Expression:
			return false;
		default:
			return Analitza::isMathML(this->toPlainText());
	}
}

void QExpressionEdit::setMode(Mode en)
{
	/*if(!text().isEmpty()) {
		Analitza a;
		if(isMathML() && en==Expression) { //We convert it into MathML
			a.setTextMML(this->toPlainText());
			this->setPlainText(Analitza::treu_tags(a.toString()));
			setCorrect(!a.isCorrect());
		} else if(!isMathML() && en==MathML) {
			QExp e(this->toPlainText());
			e.parse();
			setCorrect(!a.isCorrect());
			
			this->setPlainText(e.mathML());
		}
	}
	m_highlight->setMode(en);*/
	setCorrect(true);
}

void QExpressionEdit::returnP()
{
	removenl();
	if(!this->toPlainText().isEmpty()) {
		m_history.last() = this->toPlainText();
		m_history.append("");
		m_histPos=m_history.count()-1;
	}
}

void QExpressionEdit::keyPressEvent(QKeyEvent * e)
{
	bool ch=false;
	
	switch(e->key()) {
		case Qt::Key_Backtab:
			//setMode(isMathML() ? Expression : MathML);
			return;
		case Qt::Key_F2:
			Analitza a;
			/*if(isMathML()){
				a.setTextMML(toPlainText());
				a.simplify();
				this->setPlainText(a.textMML());
			} else {*/
			QExp e(toPlainText());
			e.parse();
			a.setTextMML(e.mathML());
			a.simplify();
			this->setPlainText(a.toString());
// 			}
			return;
	}
	
	switch(e->key()){
		case Qt::Key_Return:
		case Qt::Key_Enter:
// 			qDebug() << m_completer->popup()->isVisible();
			emit returnPressed();
			return;
		case Qt::Key_Up:
			if(!m_completer->popup()->isVisible()) {
				m_histPos++;
				ch=true;
			}
			break;
		case Qt::Key_Down:
			if(!m_completer->popup()->isVisible()) {
				m_histPos--;
				ch=true;
			}
			break;
		case Qt::Key_Plus:
		case Qt::Key_Asterisk:
		case Qt::Key_Slash:
			if(this->toPlainText().length() == (this->textCursor().position()-this->textCursor().anchor()) && m_auto) {
				this->setPlainText(m_ans);
				QTextCursor tc = this->textCursor();
				tc.setPosition(m_ans.length());
				this->setTextCursor(tc);
			}
		default:
			QTextEdit::keyPressEvent(e);
			m_history.last() = this->toPlainText();
			QString last = lastWord(textCursor().selectionStart());
			if(!last.isEmpty()) {
				QFontMetrics fm(font());
				int curPos = this->textCursor().position();
				int pixelsOffset = fm.width(toPlainText(), curPos);
				QPoint pos = QPoint(pixelsOffset+10, height());
				
				QRect r = QRect(pos, QSize(200, 100));
				m_completer->setCompletionPrefix(last);
				m_completer->complete();
// 				qDebug() << last << r;
			} else {
				m_completer->popup()->hide();
			}
			break;
	}
	
	if(ch) {
		if(m_histPos<0)
			m_histPos=0;
		if(m_histPos>=m_history.count())
			m_histPos=m_history.count()-1;
		this->setPlainText(m_history[m_histPos]);
	}
}

QString QExpressionEdit::lastWord(int pos)
{
	QString exp=text();
	int act=pos-1;
	for(; act>=0 && exp[act].isLetter(); act--);
	
	return exp.mid(act+1, pos-act-1);
}

QString QExpressionEdit::findPrec(const QString& exp, int &act, int cur, int &param, const QString& tit)
{
	QString paraula=tit, p;
	int nparams=0, cat=0;
	if(!tit.isNull())
		cat=1;
	
	for(; act<cur; ++act){
		if(exp.at(act).isLetter()) {
			paraula=QString::null;
			for(; act<exp.length() && exp[act].isLetter(); act++){
				paraula += exp[act];
			}
			
			if(act<exp.length() && exp[act]=='(' && act<cur) {//This is a function
				int param_rec=0;
				
				act++;
				p=findPrec(exp, act, cur, param_rec, paraula);
				
				if(param_rec != -1){
					param = param_rec;
//					qDebug("****out2***<%s> %s %s", p.ascii(), paraula.ascii(), tit.ascii());
					return p;
				}
			} else act--;
		} else if(exp.at(act) == ',') {
			nparams++;
		} else if(exp.at(act) == '(') {
			cat++;
		} else if(exp.at(act) == ')') {
			cat--;
//			qDebug("cat: %d", cat);
			if(cat == 0) {
				param=-1; //Means this is a useless func
				return QString::null;
			} else if(cat <0){
				param=-3;
				return QString::null;
			}
		}
//		qDebug("word: %s", exp.mid(act, exp.length()-act).ascii());
	}
	param=nparams;
	
//	qDebug("####out###%s %s <%s>", p.ascii(), paraula.ascii(), tit.ascii());
	return tit;
}

QString QExpressionEdit::editingWord(int pos, int &param)
{ //simplification use only
	int p=0;
	param=0;
	return findPrec(this->toPlainText().mid(0,pos), p, pos, param, "");
}

void QExpressionEdit::cursorMov()
{
	if(text().isEmpty()) setCorrect(true);
	int param=0, pos=this->textCursor().position();
	QString s = editingWord(pos, param);
	helpShow(s, param);
}

void QExpressionEdit::helpShow(const QString& funcname, int param)
{
	int op = Operator::nparams(Operator::toOperatorType(funcname));
	if(op) {
		if(op == -1) {
			emit signalHelper(QString("<em>%1</em>(..., <b>par%2</b>, ...)").arg(funcname).arg(param+1));
		} else {
			QString sample = (param < op) ?
						QString("<em>%1</em>(").arg(funcname) :
						QString("<em style='color:red'>%1</em>(").arg(funcname);
			for(int i=0; i<op; ++i) {
				if(i==param)
					sample += QString("<b>par%1</b>").arg(i+1);
				else
					sample += QString("par%1").arg(i+1);
				if(i<op-1) sample+= ", ";
			}
			emit signalHelper(sample+")");
		}
	} else if(a!=NULL && a->m_vars->contains(funcname) && a->m_vars->value(funcname)->type()==Object::container) { //if it is a function defined by the user
		Container *c = (Container*) a->m_vars->value(funcname);
		QStringList params = c->bvarList();
		
		QString sample = (param < params.count()) ? QString("<em>%1</em>(").arg(funcname) : QString("<em style='color:red'>%1</em>(").arg(funcname);
		
		for(int i=0; i<params.count(); ++i) {
			if(i==param)
				sample += QString("<b>%1</b>").arg(params[i]);
			else
				sample += QString("%1").arg(params[i]);
			if(i<params.count()-1)
				sample+= ", ";
		}
		emit signalHelper(sample+")");
	} else
		emit signalHelper("");
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
	this->setPlainText(this->toPlainText().remove('\n'));
}

void QExpressionEdit::ajudant(const QString& msg)
{
	QFontMetrics fm( font() );
	int curPos = 0;
	curPos=this->textCursor().position();
	int pixelsOffset = fm.width( toPlainText(), curPos );
// 	pixelsOffset -= contentsX();
	QPoint pos = mapToGlobal( QPoint( pixelsOffset, height() ) );
	
	ajudant(msg, pos-QPoint(0, 50));
}

void QExpressionEdit::ajudant(const QString& msg, QPoint p)
{
	if(!msg.isEmpty()){
		QFontMetrics fm(m_helptip->font());
		m_helptip->setText(msg);
		m_helptip->setGeometry(QRect(p, p+QPoint(fm.width(msg)+20, 23)));
		
		m_helptip->show();
		m_helptip->raise();
		this->setFocus();
	} else
		m_helptip->hide();
}

void QExpressionEdit::setCorrect(bool correct)
{
	QPalette p;
	QColor c;
	m_correct=correct;
	
	if(m_correct && !isMathML())
		c = QColor(255,255,255);
	else if(m_correct)
		c = QColor(255,255,200);
	else
		c = QColor(255,222,222);
	
	p.setColor(QPalette::Base, c);
	this->setPalette(p);
}

void QExpressionEdit::focusInEvent ( QFocusEvent * event )
{
	QTextEdit::focusInEvent(event);
	switch(event->reason()) {
		case Qt::OtherFocusReason:
		case Qt::TabFocusReason:
			this->selectAll();
		default:
			break;
	}
}
