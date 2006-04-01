#ifndef QALGEBRAHIGHLIGHTER_H
#define QALGEBRAHIGHLIGHTER_H

#include <qsyntaxhighlighter.h>
#include <qtextedit.h>
#include "qexp.h"

typedef enum {
	Disabled,
	Expression,
	MathML,
	Autodetect
} Mode;

typedef enum { //For mathml highlighting
	gt,
	lt,
	tag,
	value
} MMLtokEnum;



class QAlgebraHighlighter : public QSyntaxHighlighter {
	public:
		QAlgebraHighlighter(QTextEdit *textEdit);
		int highlightParagraph(const QString &text, int endStateOfLastPara);
		Mode mode() { return m_mode; }
		void setMode(const Mode& newMode){ m_mode=newMode; }
	private:
		TOKEN getToken(QString &a, unsigned int &l);
		//TOKEN getTokenMML(QString &a, unsigned int &l);
		bool wrong;
		tokEnum antnum;
		Mode m_mode;
};

#endif
