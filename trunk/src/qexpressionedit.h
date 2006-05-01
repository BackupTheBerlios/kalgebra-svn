#ifndef QEXPRESSIONEDIT_H
#define QEXPRESSIONEDIT_H

#include <qtextedit.h>
#include <qlabel.h>

#include "qalgebrahighlighter.h"
#include "analitza.h"
#include "qexp.h"


/**
*	@author Aleix Pol i Gonzalez <aleixpol@gmail.com>
*/
class QExpressionEdit : public QTextEdit
{
Q_OBJECT
public:
	QExpressionEdit(QWidget *parent = 0, const char *name = 0, Mode ini=Autodetect);
	~QExpressionEdit();
	inline bool mode() { return m_highlight->mode(); }
	void setMode(Mode en);
	void setAutocomplete(bool a);
	bool autocomplete();
	
	bool isMathML();
	QString editingWord(int pos, int &param);
	void setAnalitza(Analitza* in) {a=in;}
private:
	QLabel *m_helptip;
	QAlgebraHighlighter *m_highlight;
	void removenl();
	
	void keyPressEvent(QKeyEvent * e );
	
	int m_histPos;
	QStringList m_history;
	bool help;
	bool m_auto;
	
	void helpShow(const QString& funcname, unsigned int param=0);
	static QString findPrec(const QString& exp, int &act, int cur, int &param, QString tit);
	Analitza *a;
public slots:
	void returnP();
	void cursorMov(int par, int pos);
	
signals:
	void signalHelper(const QString&);
};

#endif
