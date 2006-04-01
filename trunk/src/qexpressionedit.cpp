#include "qexpressionedit.h"

QExpressionEdit::QExpressionEdit(QWidget *parent, const char *name, Mode inimode) : QTextEdit(parent, name), m_histPos(0) {
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setMargin(2);
	this->setFixedHeight(QFontMetrics(this->currentFont()).height()+12);
	this->setVScrollBarMode(QScrollView::AlwaysOff);
	this->setHScrollBarMode(QScrollView::AlwaysOff);
	
	m_history.append("");
	
	m_highlight= new QAlgebraHighlighter(this);
	connect(this, SIGNAL(returnPressed()), this, SLOT(returnP()));
}

QExpressionEdit::~QExpressionEdit() {}

void QExpressionEdit::returnP() {
	removenl();
	m_history.last() = this->text();
	m_history.append("");
	m_histPos=m_history.count()-1;
}

void QExpressionEdit::keyPressEvent(QKeyEvent * e){
	bool ch;
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
		qDebug("pos: %d", m_histPos);
	} else {
		QTextEdit::keyPressEvent(e);
		m_history.last() = this->text();
	}
}

void QExpressionEdit::removenl() {
	this->setText(this->text().stripWhiteSpace().remove('\n'));
}


#include "qexpressionedit.moc"
