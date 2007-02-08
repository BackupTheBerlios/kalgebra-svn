#include <QSyntaxHighlighter>
#include "expressionedit.h"
#include "exp.h"

#ifndef ALGEBRAHIGHLIGHTER_H
#define ALGEBRAHIGHLIGHTER_H

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



class AlgebraHighlighter : public QSyntaxHighlighter
{
	public:
		AlgebraHighlighter(QTextDocument *doc);
		//int highlightParagraph(const QString &text, int endStateOfLastPara);
		Mode mode() { return m_mode; }
		void setMode(const Mode& newMode){ m_mode=newMode; rehighlight(); }
		bool isWrong() const { return wrong; }
		void setPos(int p) { m_pos=p; }
	private:
		TOKEN getToken(QString &a, int &l);
		//TOKEN getTokenMML(QString &a, unsigned int &l);
		bool wrong;
		tokEnum antnum;
		Mode m_mode;
		int m_pos;
		
		QTextCharFormat negreta;
		int complementary(const QString&, int p);
	protected:
		void highlightBlock(const QString &text);
};

#endif
