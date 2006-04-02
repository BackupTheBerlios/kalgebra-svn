#ifndef QEXPRESSIONEDIT_H
#define QEXPRESSIONEDIT_H

#include <qtextedit.h>
#include <qlabel.h>
#include "qalgebrahighlighter.h"

/**
	@author Aleix Pol i Gonzalez <aleixpol@gmail.com>
*/
class QExpressionEdit : public QTextEdit
{
Q_OBJECT
public:
	QExpressionEdit(QWidget *parent = 0, const char *name = 0, Mode ini=Autodetect);
	~QExpressionEdit();
	inline bool mode() { return m_highlight->mode(); }
	inline void setMode(Mode en) { return m_highlight->setMode(en); }
	
	bool isMathML();
private:
	QLabel *m_helptip;
	QAlgebraHighlighter *m_highlight;
	void removenl();
	
	void keyPressEvent(QKeyEvent * e );
	
	int m_histPos;
	QStringList m_history;
	bool help;
	
	void helpShow(const QString& funcname, int param=0);
	
public slots:
	void returnP();
	void cursorMov(int par, int pos);
	
signals:
	void signalHelper(const QString&);
};

#endif
