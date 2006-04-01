#include "qalgebrahighlighter.h"

QAlgebraHighlighter::QAlgebraHighlighter(QTextEdit *textEdit) : QSyntaxHighlighter(textEdit), m_mode(Autodetect){}


int QAlgebraHighlighter::highlightParagraph(const QString &text, int endStateOfLastPara) {
	setFormat( 0, text.length(), Qt::black);
	if(m_mode==MathML || (m_mode==Autodetect && text[0]=='<'))
		return 0;
	QString op=text.stripWhiteSpace();
	unsigned int pos=0, len=0;
	wrong=true;
	TOKEN t;
	t=getToken(op, len);
	pos=0;
	while(pos < text.length()-1 || t.tipus!=tEof){
		if(t.tipus==tVal)	setFormat( pos, len, QColor(0,0,200));
		else if(t.tipus==tFunc)	setFormat( pos, len, QColor(0,50,0));
		else if(t.tipus==tVar)	setFormat( pos, len, QColor(100,0,0));
		else		setFormat( pos, len, QFont(this->textEdit()->currentFont().family(),
					this->textEdit()->currentFont().pointSize(),
					QFont::DemiBold, false));
	
		pos += len;
		t=getToken(op, len);
	}
	return endStateOfLastPara;
}

TOKEN QAlgebraHighlighter::getToken(QString &a, unsigned int &l){
	int i=0;
	l=a.length();
	a = a.stripWhiteSpace();
// 	printf("%s\n", a.ascii());
	TOKEN ret;
	ret.tipus = tMaxOp;
	if(a==""){
		ret.tipus = tEof;
		l=1;
	}
	else if(a[0].isDigit()) {//es un numero
		ret.val += a[0];
		a[0]=' ';
		for(i=1; a[i].isDigit() || a[i]=='.'; i++){
			a[i]=' ';
		}
		if(a[i] == '(' || a[i].isLetter()) a = " *" +a;
		ret.tipus= tVal;
	} else if(a[0].isLetter()) {//es una variable o func
		ret.val += a[0];
		for(i=1; a[i].isLetter(); i++)	a[i]=' ';
		if(a[i]=='(')
			ret.tipus=tFunc;
		else 
			ret.tipus= tVar;
	} else if(a[0]=='-' && a[1] == '>') {
		ret.tipus = tLambda;
		a[1] =' ';
	} else if(a[0]==':' && a[1] == '=') {
		ret.tipus = tAssig;
		a[1] =' ';
	} else if(a[0]=='+')
		ret.tipus = tAdd;
	else if(a[0]=='-')
		ret.tipus = (antnum == tVal || antnum==tRpr) ? tSub : tUmi;
	else if(a[0]=='/')
		ret.tipus = tDiv;
	else if(a[0]=='^')
		ret.tipus = tPow;
	else if(a[0]=='*' && a[1] == '*') {
		ret.tipus = tPow;
		a[1] =' ';
	} else if(a[0]=='*')
		ret.tipus = tMul;
	else if(a[0]=='(')
		ret.tipus = tLpr;
	else if(a[0]==')')
		ret.tipus = tRpr;
	else if(a[0]==',')
		ret.tipus = tComa;
	else
		wrong=true;
	
	a[0]=' ';
	a=a.stripWhiteSpace();
	antnum = ret.tipus;
	l-=a.length();
// 	qDebug("%d------------", a.length());
	return ret;
}