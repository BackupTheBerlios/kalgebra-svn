/***************************************************************************
 *   Copyright (C) 2005 by ,,,                                             *
 *   aleix@sklar                                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "analitza.h"

Analitza::Analitza(QString path){
	setPath(path);
	Analitza();
}

Analitza::Analitza(){QLocale::setDefault(QLocale::C); err=""; }

Analitza::~Analitza(){}

int Analitza::setPath(QString ar){
	QDomElement docElem;
	QFile file(ar);
	if ( !file.open( IO_ReadOnly ) ){
		err += i18n("Error while parsing: %1").arg(ar);
		return -1;
	}
	if ( !doc.setContent( &file ) ) {
		file.close();
		err += i18n("Error while parsing: %1").arg(ar);
		return -2;
	}
	file.close();
	
	docElem = doc.documentElement();
	elem = docElem.firstChild();
	return 0;
}

int Analitza::setTextMML(QString str){
	QDomDocument doc;
	QDomElement docElem;
	
	if ( !doc.setContent(str) ) {
		err += i18n("Error while parsing: %1").arg(str);
		return -1;
	}
		
	docElem = doc.documentElement();
	
	elem = docElem.firstChild();
	return 0;
}

int Analitza::setText(QString op){
	QExp a(op);
	a.parse();
	err="";
	if(a.error() != ""){
		err = a.error();
		return -1;
	}
	mmlexp=a.mathML();
	return setTextMML(a.mathML());
}

QString Analitza::textMML(){
	return mmlexp;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

QDomElement Analitza::evalua(QDomNode n){
	QDomNode j ;
	QDomElement e;
	QString operador;
	QValueList<QDomElement> nombres;
	unsigned int fills = 0;
	
	while( !n.isNull() ) {
		e = n.toElement();
		if(vars.isvar(e))
			nombres.append(vars.value(e.text()));
		else if (Polynomial::isPoly(e) || Scalar::isScalar(e) || isNum(e.tagName()))
			nombres.append(e);
		else if (e.tagName() == "declare"){
			j = e.firstChild(); //Segons l'estandar, es <ci>
			QString s = j.toElement().text();
			j = j.nextSibling();
//		vars.modifica(s, evalua(j.toElement()));
			vars.modifica(s, j.toElement());
		} else if(e.tagName() == "apply"){
			j = e.firstChild();
			nombres.append( evalua(j) );
		} else if (isOperador(e.tagName()))
			operador = e.tagName();
		fills++;
		n = n.nextSibling();
	}
	
	QDomDocument a;
	QDomElement ret;
	QValueList<QDomElement>::iterator it;
	
	it = nombres.begin();
	ret = *it;
	if(fills>2)
		it++;
	
	//if(fills!=1 || operador!=""){
		for( ; it != nombres.end(); it++)
			ret = opera(ret,*it,operador, fills==2?1:0);
	//}
	
	return ret ;
}

bool Analitza::isNum(QString s){
	if(s=="true")
		return true;
	else if(s=="false")
		return true;
	else if(s=="pi")
		return true;
	else if(s=="exponentiale")
		return true;
	else if(s=="eulergamma")
		return true;
	return false;
}

QDomElement Analitza::opera(QDomElement res, QDomElement oper, QString op, int minus=0){
	QDomElement e;
	
	//////////////////////
	/*qDebug("----operant----%s", op.ascii());
	treubug::print_dom(res);
	treubug::print_dom(oper);
	qDebug("++++operant++++%s", op.ascii());*/
	/////////////////////
	
	if(Scalar::isScalar(res) && Scalar::isScalar(oper)) {
		Scalar a=Scalar(), b=Scalar();
// 		qDebug("Scalarssss");
		a.setValue(res);
		b.setValue(oper);
		e= calc(op, a,b,a, minus);
		a.print("Resultat: ");
	} else if(Polynomial::isPoly(res) && Polynomial::isPoly(oper)){
		Polynomial a=Polynomial(), b=Polynomial();
		a.setValue(res);
		b.setValue(oper);
		e= calc(op, a,b,a, minus);
		qDebug("Polyssss op:%s res:%s", op.ascii(), e.text().ascii());
		a.print("Resultat: ");
// 		qDebug("*******************");
// 		treubug::print_dom(a.value(doc));
// 		qDebug("*******************");
	} else if(Polynomial::isPoly(res) && Scalar::isScalar(oper)){
		Polynomial a=Polynomial();
		Scalar b=Scalar();
		a.setValue(res);
		b.setValue(oper);
		e= calc(op, a,b,a, minus);
		qDebug("Polyssss<op>Scalar op:%s res:%s", op.ascii(), e.text().ascii());
		a.print("Resultat: ");
// 		qDebug("*******************");
// 		treubug::print_dom(a.value(doc));
// 		qDebug("*******************");
	}
	
	return e;
}

template <class T, class V, class W> QDomElement Analitza::calc(QString op, T a, V b, W c, int minus){
	if(op == "plus"){
		c = a+b;
	}else if(op =="times"){
		c = a*b;
	}else if(op=="divide"){
		c=a.divide(b);
	}else if(op=="minus"){
		c = minus ? -a : a-b;
	}else if(op=="power"){
		c = a.pow(b);
	}else if(op=="quotient"){
		c = a/b;
	}else if(op=="factorial"){
		c=a.factorial();
	} else if(op=="abs"){
		c= a.floor();
	} else if(op=="sin"){
		c=a.sin();
	} else if(op=="cos"){
		c=a.cos();
	} else if(op=="tan"){
		c=a.tan();
	} else if(op=="sec"){
		c=a.sec();
	} else if(op=="csc"){
		c=a.csc();
	} else if(op=="cot"){
		c=a.cot();
	} else if(op=="sinh"){
		c=a.sinh();
	} else if(op=="cosh"){
		c=a.cosh();
	} else if(op=="tanh"){
		c=a.tanh();
	} else if(op=="sech"){
		c=a.sech();
	} else if(op=="csch"){
		c=a.csch();
	} else if(op=="coth"){
		c=a.coth();
	} else if(op=="arcsin"){
		c=a.asin();
	} else if(op=="arccos"){
		c=a.acos();
	} else if(op=="arctan"){
		c=a.acos();
	} else if(op=="arccot"){
		c=a.arccot();
	} else if(op=="arccoth"){
		c=a.arccoth();
	} else if(op=="arccosh"){
		c=a.arccosh();
// 	} else if(op=="arccsc"){
// 	} else if(op=="arccsch"){
// 	} else if(op=="arcsec"){
// 	} else if(op=="arcsech"){
// 	} else if(op=="arcsinh"){
// 	} else if(op=="arctanh"){
	} else if(op=="exp"){
		c=a.exp();
	} else if(op=="ln"){
		c=a.log();
	} else if(op=="log"){
		c=a.log10();
	} else if(op=="abs"){
		c=a.abs();
	//} else if(op=="conjugate"){
	//} else if(op=="arg"){
	//} else if(op=="real"){
	//} else if(op=="imaginary"){
	} else if(op=="floor"){
		c=a.floor();
	} else if(op=="ceiling" ){
		c=a.ceil();
	} /*else if(op=="min" ){
		c= (a < b)? a : b;
	} else if(op=="max" ){
		c= (a > b)? a : b;
	}*/ else if(op=="gt" ){
		c= (a > b);
	} else if(op=="lt" ){
		c= (a < b);
	} else if(op=="geq" ){
		c= a>= b;
	} else if(op=="leq" ){
		c= a <= b;
	} else if(op=="and" ){
		c= a && b;
	} else if(op=="not"){
		c=!a;
	} else if(op=="or"){
		c= a || b;
	} else if(op=="xor"){
		c= (a || b) && !(a&&b);
	} else if(op=="implies"){
		c= a.implies(b);
	} else if(op=="gcd"){
		c=a.gcd(b);
	} else if(op=="lcm"){
		c=a.lcm(b);
	} else if(op=="root") {
		c=a.root(b);
	} else {
		if(op!="") err += i18n("The operator %1 hasn't been implemented").arg(op);
	}
	return c.value(doc);
}

int Analitza::isOperador(QString e){
	if (		e=="plus" ||
			e=="times" ||
			e=="min"||
			e=="max"||
			e=="and"||
			e=="or"||
			e=="xor"||
			e=="gcd"||
			e=="lcm") {
		return -1;
	} else if (	e=="quotient" ||
			e=="power" || 
			e=="minus" ||
			e=="divide"  ||
			e=="lt" ||
			e=="gt" ||
			e=="leq" ||
			e=="geq" ||
			e=="implies" ||
			e=="root"){
		return 2;
	} else if (	e=="factorial" || 
			e=="abs" || 
			e=="sin" || 
			e=="cos" || 
			e=="tan" || 
			e=="sec" || 
			e=="csc" || 
			e=="cot" ||
			e=="sinh" || 
			e=="cosh" || 
			e=="tanh" || 
			e=="sech" || 
			e=="csch" || 
			e=="coth" || 
			e=="arcsin" || 
			e=="arccos" || 
			e=="arctan" || 
			e=="arccos" || 
			e=="arccot" || 
			e=="arcoth" || 
			e=="arccosh" || 
			e=="arccsc"  || 
			e=="arccsch" || 
			e=="arcsec"  || 
			e=="arcsech" || 
			e=="arcsinh" || 
			e=="arctanh" || 
			e=="exp" || 
			e=="ln" || 
			e=="log"||
			e=="not" ||
			e=="factorial" || 
			e=="minus" || 
			e=="abs" || 
			e=="conjugate" ||
			e=="arg" || 
			e=="real" || 
			e=="imaginary" || 
			e=="floor" || 
			e=="ceiling") {
		return 1;
	}
	
	return false;
}

QDomElement Analitza::Calcula(){
	return evalua(elem);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

QString Analitza::toString(){
	QString ret;
// 	QMessageBox::warning(0, "Titulo", elem.toElement().text(),"Ok", "Cancelar",0,0,1);
	ret = QString("<math>%1</math>").arg(str(elem));
	return ret;
}

QString Analitza::get_op(QDomNode n){
	QString s;
	while( !n.isNull() ) {
		s=n.toElement().tagName();
		if (isOperador(s))
			return s;
		n= n.nextSibling();
	}
	return "";
}

unsigned int Analitza::toOpId(QDomNode n){
// 	if(n.parentNode().toElement().tagName() == "")
// 		return 0;
	
	QString e=get_op(n);
	if(e=="")
		return 0;
	else if(e=="lt" || e=="gt" || e=="eq" || e=="leq" || e=="geq")
		return 1;
	else if(e=="sin" || e=="cos" || e=="tan")
		return 2;
	else if(e=="minus" || e=="plus")
		return 3;
	else if(e=="times" || e=="quotient")
		return 4;
	else if(e=="and" || e=="or" || e=="xor")
		return 6;
	else if(e=="power")
		return 5;
	else
		return 10000;

}

QString Analitza::str(QDomNode n){
	QDomNode j,k=n;
	QDomElement e;
	QString operador;
	QValueList<QString> nombres;
	unsigned int fills = 0;
	while( !n.isNull() ) {
		e = n.toElement();
		if(e.tagName() == "apply" || e.tagName() == ""){
			j = e.firstChild();
			//No fa falta que tinguem tot entre parentesi
// 			cout << "hola :)" << endl;
// 			cout << toOpId(k) << "<->" << toOpId(j) << endl;
			if(toOpId(k) <= toOpId(j) || isOperador(get_op(j)) == 1)
				nombres.append(str(j));
			else
				nombres.append("(" + str(j) + ")");
		} else if (e.tagName() == "cn" ||e.tagName() == "ci") {
			nombres.append("<mn>" + e.text() + "</mn>");
		} else if(isNum(e.tagName())){
			nombres.append("<mn>" + e.tagName() + "</mn>");
		} else if (isOperador(e.tagName())) {
			operador = e.tagName();
		} else if (e.tagName()=="declare") {
			j = e.firstChild(); //Segons l'estandar, es <ci>
			nombres.append(j.toElement().text());
			j = j.nextSibling(); //Valor a guardar
			nombres.append(j.toElement().text());
			operador = "declare";
			fills=3;
			
		}
		fills++;
		n= n.nextSibling();
	}
	QString r="";
	QValueList<QString>::iterator it;
	
	it = nombres.begin();
	
	r = *it;
	if(fills>2)
		it++;
		
	for(; it != nombres.end(); ++it){
// 		cout << (*it).ascii() << endl;
		r = escriu(r,*it,operador, fills<=2?1:0);
	}
	
	return r;
}

QString Analitza::escriu(QString res, QString oper, QString op, int unari=0){
	res  =  res.stripWhiteSpace();
	oper = oper.stripWhiteSpace();
// 	qDebug("operador: %s n1: %s n2: %s\n", op, res, oper);
	QString r=res;
	if(op == "plus"){
		r = res + "<mo>+</mo>" +oper;
	}else if(op =="times"){
		r = res + "<mo>*</mo>" +oper;
	}else if(op=="divide"){
		r = res + "<mo>/</mo>" +oper;
	}else if(op=="minus"){
		r = unari ? "<mo>-</mo>"+res : res + "<mo>-</mo>" +oper;
	}else if(op=="power"){
		r = res + "<mo>^</mo>" + oper;
// 		r = "<msup><mrow>" + res + "</mrow><mrow>" + oper +"</mrow></msup>";
	} else if(op=="eq"){
		r = res + "<mo>=</mo>" +oper;
	} else if(op=="geq"){
		r = res + "<mo>&ge;</mo>" +oper;
	} else if(op=="leq"){
		r = res + "<mo>&le;</mo>" +oper;
	}else if(op=="quotient"){
		r = res + "<mo>/</mo>" +oper;
	}else if(op=="lt"){
		r = res + "<mo>&lt;</mo>" +oper;
	}else if(op=="gt"){
		r = res + "<mo>&gt;</mo>" +oper;
	}else if(op=="factorial"){
		r = res + "<mo>!</mo>";
	} else if(op=="abs"){
		r = "<mo>|</mo><mn>" + res + "<mo>|</mo>";
	} else if(op=="sin"){
		r = "sin(" + res + ")";
	} else if(op=="cos"){
		r = "cos(" + res + ")";
	} else if(op=="tan"){
		r = "tg(" + res + ")";;
	} else if(op=="sec"){
		r = "sec(" + res + ")";
	} else if(op=="csc"){
		r = "cosec(" + res + ")";
	} else if(op=="cot"){
		r = "cotg(" + res + ")";
	} else if(op=="sinh"){
		r = "sinh(" + res + ")";
	} else if(op=="cosh"){
		r = "cosh(" + res + ")";
	} else if(op=="tanh"){
		r = "tanh(" + res + ")";
	} else if(op=="sech"){
		r = "sech(" + res + ")";
	} else if(op=="csch"){
		r = "cosech(" + res + ")";
	} else if(op=="coth"){
		r = "cotgh(" + res + ")";
	} else if(op=="arcsin"){
		r = "arcsin(" + res + ")";
	} else if(op=="arccos"){
		r = "arccos(" + res + ")";
	} else if(op=="arctan"){
		r = "arctg(" + res + ")";
	} else if(op=="arccos"){
		r = "arccos(" + res + ")";
	} else if(op=="arccot"){
		r = "arccot(" + res + ")";
	} else if(op=="arcoth"){
		r = "arccotgh(" + res + ")";
	} else if(op=="arccosh"){
		r = "arccosh(" + res + ")";
	} else if(op=="arccsc"){
		r = "arccosec(" + res + ")";
	} else if(op=="arccsch"){
		r = "arccosech(" + res + ")";
	} else if(op=="arcsec"){
		r = "arcsec(" + res + ")";
	} else if(op=="arcsech"){
		r = "arcsech(" + res + ")";
	} else if(op=="arcsinh"){
		r = "arcsinh(" + res + ")";
	} else if(op=="arctanh"){
		r = "arctanh(" + res + ")";
	} else if(op=="exp"){
		r = "exp(" + res + ")";
	} else if(op=="ln"){
		r = "ln(" + res + ")";
	} else if(op=="log"){
		r = "log10(" + res + ")";
	} else if(op=="not"){
		r = "<mo></mo>" + res;
	} else if(op=="and"){
		r = res + "<mo>&Lambda;</mo>" + oper;
	} else if(op=="or"){
		r = res + "<mo>V</mo>" + oper;
	} else if(op=="xor"){
		r = res + "<mo>&oplus;</mo>" + oper;
	} else if(op=="gcd"){
		r = "gcd(" + res + ", "+ oper + ")";
	} else if(op=="lcm"){
		r = "lcm(" + res + ", "+ oper + ")";
	} else if(op=="declare"){
		r = res + "<mo>:=</mo>" + oper;
	} else if(op=="implies"){
		r = res + "<mo>=></mo>" + oper;
	} //else if(op=="conjugate"){
// 	} else if(op=="arg"){
// 	} else if(op=="real"){
// 	} else if(op=="imaginary"){
// 	} else if(op=="floor"){
// 		c=floor(a);
// 	} else if(op=="ceiling" ){
// 		c=ceil(a);
// 	}
	
	else {
// 		cout << "The operator: <" << op.latin1() << " /> is not implemented." << endl;
// 		r = QString("%1(%2)").arg(op).arg(res);
		
	}
	return r;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////                                                   /////////////////////
/////////////                         Other                     /////////////////////
/////////////                                                   /////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

QString Analitza::treu_tags(QString in){
	bool tag=false;
	QString out;
	for(unsigned int i=0; i<in.length(); i++){
		if(in[i]=='<')
			tag=true;
		else if(in[i]=='>')
			tag=false;
		else if(!tag)
			out += in[i];
	}
	return out;
}
