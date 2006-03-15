#ifndef QEXPRESSIONEDIT_H
#define QEXPRESSIONEDIT_H

#include <qtextedit.h>
#include "qalgebrahighlighter.h"

/**
	@author Aleix Pol i Gonzalez <aleixpol@gmail.com>
*/
class QExpressionEdit : public QTextEdit
{
Q_OBJECT
public:
	QExpressionEdit(QWidget *parent = 0, const char *name = 0);
	~QExpressionEdit();
	inline bool mode() { return m_highlight->mode(); }
	inline void setMode(Mode en) { return m_highlight->setMode(en); }
private:
	QAlgebraHighlighter *m_highlight;
	void removenl() { this->setText(this->text().stripWhiteSpace().remove('\n')); }
public slots:
	void returnP() { removenl(); }
};

#endif
