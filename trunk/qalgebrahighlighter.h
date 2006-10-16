#include <QSyntaxHighlighter>
#include "qexpressionedit.h"
#include "qexp.h"

#ifndef QALGEBRAHIGHLIGHTER_H
#define QALGEBRAHIGHLIGHTER_H

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
		QAlgebraHighlighter(QTextDocument *doc);
		//int highlightParagraph(const QString &text, int endStateOfLastPara);
		Mode mode() { return m_mode; }
		void setMode(const Mode& newMode){ m_mode=newMode; rehighlight(); }
		bool isWrong() const { return wrong; }
	private:
		TOKEN getToken(QString &a, int &l);
		//TOKEN getTokenMML(QString &a, unsigned int &l);
		bool wrong;
		tokEnum antnum;
		Mode m_mode;
		
		QTextCharFormat negreta;
		
	protected:
		void highlightBlock(const QString &text);
};

#endif
