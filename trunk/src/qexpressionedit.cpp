#include "qexpressionedit.h"

QExpressionEdit::QExpressionEdit(QWidget *parent, const char *name) : QTextEdit(parent, name) {
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setMargin(2);
	this->setFixedHeight(QFontMetrics(this->currentFont()).height()+12);
	this->setVScrollBarMode(QScrollView::AlwaysOff);
	this->setHScrollBarMode(QScrollView::AlwaysOff);
	
	m_highlight= new QAlgebraHighlighter(this);
	connect(this, SIGNAL(returnPressed()), this, SLOT(returnP()));
}

QExpressionEdit::~QExpressionEdit() {}


#include "qexpressionedit.moc"
