#include "qalgebrahighlighter.h"


QAlgebraHighlighter::QAlgebraHighlighter(QTextDocument *doc) : QSyntaxHighlighter(doc), wrong(false), m_mode(Autodetect)
{
	negreta.setFontWeight(QFont::Bold);
}

void QAlgebraHighlighter::highlightBlock(const QString &text) {
	//setFormat(0, text.length(), Qt::black);
	wrong=false;
	if(Analitza::isMathML(text)) {
		QString lasttag;
		for(int i=0; i<text.length(); i++){
			if(text[i]=='<') { //We enter in a tag
				lasttag=QString();
				int j=i+1, k=0;
				for(k=i+1; k<text.length() && text[k]!='>'; k++){
					lasttag.append(text[k]);
					if(text[k]!=' ' && j==k-1)
						j=k;
				}
				j++;
				
				setFormat(i, 1, negreta);
				setFormat(j, 1, negreta);
				if(lasttag.startsWith("/")){
					setFormat(i+1, j-i-1, QColor(100,0,0));
					setFormat(i+1, 1, negreta);
				} else if(lasttag.endsWith("/")) {
					if(Analitza::whatType(lasttag.left(lasttag.length()-1).trimmed())==Object::oper) {
						setFormat(i+1, j-i-1, QColor(0,50,0));
					} else {
						setFormat(i+1, j-i-1, QColor(0,50,0));
						setFormat(j-1, 1, negreta);
					}
				} else if(j!=k) {
					setFormat(i+1, j-i-1, QColor(150,0,0));
					setFormat(j+1, k-j-1, QColor(150,100,0));
				} else
					setFormat(i+1, j-i-1, QColor(150,0,0));
				i=k;
			}
			else if(lasttag=="cn")
				setFormat(i, 1, QColor(0,0,200));
			else if(lasttag=="ci")
				setFormat(i, 1, QColor(100,0,0));
		}
	} else {
		int pos=0, len=0;
		QString op=text.trimmed();
		
		TOKEN t=getToken(op, len);
		for(pos=0; pos<text.length() && text[pos].isSpace(); pos++);
		
		while(pos < text.length() && t.tipus!=tEof){
			switch(t.tipus){
				case tVal:
					setFormat(pos, len, QColor(0,0,200));
					break;
				case tFunc:
					setFormat(pos, len, QColor(0,50,0));
					break;
				case tVar:
					setFormat(pos, len, QColor(100,0,0));
					break;
				default:
					setFormat(pos, len, negreta);
					break;
			}
			pos += len;
			t=getToken(op, len);
		}
	}
}

TOKEN QAlgebraHighlighter::getToken(QString &a, int &l){
	int i=0;
	l=a.length();
	a = a.trimmed();
	
	TOKEN ret;
	ret.tipus = tMaxOp;
	if(a==""){
		ret.tipus = tEof;
		l=1;
	}
	else if(a[0].isDigit() || (a[0]=='.' && a[1].isDigit())) {//es un numero
		ret.val += a[0];
		a[0]=' ';
		for(i=1; a[i].isDigit() || (a[i]=='.' && a[i+1]!='.'); i++){
			a[i]=' ';
		}
		ret.tipus= tVal;
	} else if(a[0].isLetter()) {//es una variable o func
		ret.val += a[0];
		for(i=1; a[i].isLetter(); i++){
			ret.val += a[i];
			a[i]=' ';
		}
		
		for(;a[i].isSpace();i++);

		if((a[i]=='(' || a[i].isLetterOrNumber()) && (Analitza::whatType(ret.val)==Object::oper))
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
	else if(a[0]=='.' && a[1] == '.') {
		ret.tipus = tLimits;
		a[1] =' ';
	} else if(a[0]=='*' && a[1] == '*') {
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
	a=a.trimmed();
	antnum = ret.tipus;
	l-=a.length();
// 	qDebug("%d------------", a.length());
	return ret;
}
