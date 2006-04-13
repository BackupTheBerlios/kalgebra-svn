#include "qalgebrahighlighter.h"
#include "qexpressionedit.h"

QAlgebraHighlighter::QAlgebraHighlighter(QTextEdit *textEdit) : QSyntaxHighlighter(textEdit), m_mode(Autodetect){}

int QAlgebraHighlighter::highlightParagraph(const QString &text, int endStateOfLastPara) {
	setFormat(0, text.length(), Qt::black);
	
	if(QExpressionEdit::isMathML(text)) {
		QString lasttag;
		for(unsigned int i=0; i<text.length(); i++){
			if(text[i]=='<') { //We enter in a tag
				lasttag=QString();
				unsigned int j;
				for(j=i+1; j<text.length() && text[j]!='>'; j++){
					lasttag.append(text[j]);
				}
				
				setFormat(i, 1, QFont(this->textEdit()->currentFont().family(), this->textEdit()->currentFont().pointSize(), QFont::DemiBold, false));
				setFormat(j, 1, QFont(this->textEdit()->currentFont().family(), this->textEdit()->currentFont().pointSize(), QFont::DemiBold, false));
				if(lasttag.startsWith("/")){
					setFormat(i+1, j-i-1, QColor(100,0,0));
					setFormat(i+1, 1, QFont(this->textEdit()->currentFont().family(), this->textEdit()->currentFont().pointSize(), QFont::DemiBold, false));
				} else if(lasttag.endsWith("/")) {
					if(Analitza::isOperador(lasttag.left(lasttag.length()-1).stripWhiteSpace())) {
						setFormat(i+1, j-i-1, QFont(this->textEdit()->currentFont().family(), this->textEdit()->currentFont().pointSize(), QFont::DemiBold, false), QColor(0,50,0));
					} else {
						setFormat(i+1, j-i-1, QColor(0,50,0));
						setFormat(j-1, 1, QFont(this->textEdit()->currentFont().family(), this->textEdit()->currentFont().pointSize(), QFont::DemiBold, false));
					}
				} else
					setFormat(i+1, j-i-1, QColor(150,0,0));
				i=j;
			}
			else if(lasttag=="cn")
				setFormat(i, 1, QColor(0,0,200));
			else if(lasttag=="ci")
				setFormat(i, 1, QColor(100,0,0));
		}
	} else {
		wrong=true;
		unsigned int pos=0, len=0;
		QString op=text.stripWhiteSpace();
		
		TOKEN t=getToken(op, len);
		for(pos=0; pos<text.length() && text[pos].isSpace(); pos++);
		
		while(pos < text.length()-1 && t.tipus!=tEof){
			switch(t.tipus){
				case tVal:
					setFormat( pos, len, QColor(0,0,200));
					break;
				case tFunc:
					setFormat( pos, len, QColor(0,50,0));
					break;
				case tVar:
					setFormat( pos, len, QColor(100,0,0));
					break;
				default:
					setFormat(pos, len, QFont(this->textEdit()->currentFont().family(),
						this->textEdit()->currentFont().pointSize(),
						QFont::DemiBold, false));
					break;
			}
			pos += len;
			t=getToken(op, len);
		}

	}
	return endStateOfLastPara;
}

TOKEN QAlgebraHighlighter::getToken(QString &a, unsigned int &l){
	int i=0;
	l=a.length();
	a = a.stripWhiteSpace();
	
	TOKEN ret;
	ret.tipus = tMaxOp;
	if(a==""){
		ret.tipus = tEof;
		l=1;
	}
	else if(a[0].isDigit() || (a[0]=='.' && a[1].isDigit())) {//es un numero
		ret.val += a[0];
		a[0]=' ';
		for(i=1; a[i].isDigit() || a[i]=='.'; i++){
			a[i]=' ';
		}
		ret.tipus= tVal;
	} else if(a[0].isLetter()) {//es una variable o func
		ret.val += a[0];
		for(i=1; a[i].isLetter(); i++){
			ret.val += a[i];
			a[i]=' ';
		}
		
		if(a[i]=='(' && Analitza::isOperador(ret.val))
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