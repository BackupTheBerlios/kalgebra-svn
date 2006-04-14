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

void Analitza::setVars(Variables v){ vars=v; }

int Analitza::setPath(QString ar){
	QDomDocument doc;
	QDomElement docElem;
	QFile file(ar);
	if ( !file.open( IO_ReadOnly ) ){
		err += i18n("Error while parsing: %1<br />\n").arg(ar);
		return -1;
	}
	if ( !doc.setContent( &file ) ) {
		file.close();
		err += i18n("Error while parsing: %1<br/>").arg(ar);
		return -2;
	}
	file.close();
	
	docElem = doc.documentElement();
	elem = docElem.firstChild();
	return 0;
}

int Analitza::setTextMML(QString str){
	QDomDocument doc;
	
	if ( !doc.setContent(str) ) {
		err += i18n("Error while parsing: %1<br />\n").arg(str);
		return -1;
	}
	elem = doc.documentElement();
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
	
	return setTextMML(mmlexp);
}

QString Analitza::textMML(){
	return mmlexp;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

double Analitza::Calcula(){
// 	QDomNodeList elems = elem.toElement().elementsByTagName(QString("apply"));
// 	return evalua(elems.item(0));
	err="";
	return evalua(elem.firstChild());
}

double Analitza::evalua(QDomNode n){
	QDomNode j, n_ini=n;
	QDomElement e;
	QString operador;
	QValueList<double> nombres;
	double ret=.0;
	int fills=0;
	int sons=0;
	
	while( !n.isNull() ) {
		e = n.toElement();
		
		if(e.tagName() == "apply" || e.tagName() == "lambda"){
			j = e.firstChild();
			nombres.append(evalua(j));
		} else if (e.tagName() == "cn" || e.tagName() == "ci" || isNum(e.tagName())) {
			nombres.append(toNum(e));
		} else if (e.tagName() == "declare"){
			j = e.firstChild(); //Should be a <ci>
			QString s = j.toElement().text();
			j = j.nextSibling();
			vars.modifica(s, j.toElement());
			//ret = evalua(j);
		} else if(isOperador(e.tagName())) {
			operador = e.tagName();
			sons=isOperador(operador);
		} else if (e.tagName() != "bvar")
			err += i18n("The operator <em>%1</em> hasn't been implemented<br />\n").arg(e.tagName());
		if(e.tagName() == "sum" || e.tagName() == "product")
			break;
		fills++;
		n = n.nextSibling();
	}
	
	if(operador=="sum") ret =sum(n);
	else if(fills-1==sons || (sons==-1 && fills>=3) || operador == "" || operador == "minus"){
		QDomDocument a;
		QValueList<double>::iterator it = nombres.begin();
		ret = *it;
		
		if(fills>2)
			it++;
		for(; it != nombres.end(); ++it)
			ret = opera(ret,*it,operador, fills<=2?1:0);
	} else { 
		if(sons==-1)
			err += i18n("The <em>%1</em> operator, should have more than 1 parameter<p />").arg(operador);
		else
			err += i18n("Can't have %1 parameter with <em>%2</em> operator, it should have %3 parameter<p />").arg(fills-1).arg(operador).arg(sons);
	}
	return ret;
}

double Analitza::sum(QDomNode n){
	double ret=.0;
	QString var=bvar(n.parentNode())[0];
	vars.rename(var, QString("%1_").arg(var)); //We save the var value
	double ul=toNum(uplimit(n.parentNode()).toElement());
	double dl=toNum(downlimit(n.parentNode()).toElement());
	
	for(double a=dl; a<ul; a+=1.){
		vars.modifica(var, a);
		ret=opera(ret, evalua(first_val(n.parentNode())),"plus",1);
	}
	vars.remove(var);
	vars.rename(QString("%1_").arg(var), var); //We restore the var value
	return ret;
}


double Analitza::opera(double res, double oper, QString op, int minus=0){
	double a=res, b=oper;
	bool boolean=false;
	
	if(op == "plus"){
		a += b;
	}else if(op =="times"){
		a *= b;
	}else if(op=="divide"){
		a /=b;
	}else if(op=="minus"){
		a = minus ? -a : a-b;
	}else if(op=="power"){
		a = pow(a, b);
	}else if(op=="rem"){
		a = (int)a % (int)b;
	}else if(op=="quotient"){
		a = (int)a / (int)b;
	}else if(op=="factorof"){
		a = (((int)a % (int)b)==0) ? 1.0 : 0.0;
		boolean = true;
	}else if(op=="factorial"){
		b = a;
		for(a=1; b>1; b--)
			a*=b;
	} else if(op=="sin"){
		a=sin(a);
	} else if(op=="cos"){
		a=cos(a);
	} else if(op=="tan"){
		a=tan(a);
	} else if(op=="sec"){
		a=1/cos(a);
	} else if(op=="csc"){
		a=1/sin(a);
	} else if(op=="cot"){
		a=1/tan(a);
	} else if(op=="sinh"){
		a=sinh(a);
	} else if(op=="cosh"){
		a=cosh(a);
	} else if(op=="tanh"){
		a=tanh(a);
	} else if(op=="sech"){
		a=1.0/cosh(a);
	} else if(op=="csch"){
		a=1.0/sinh(a);
	} else if(op=="coth"){
		a=cosh(a)/sinh(a);
	} else if(op=="arcsin"){
		a=asin(a);
	} else if(op=="arccos"){
		a=acos(a);
	} else if(op=="arctan"){
		a=acos(a);
	} else if(op=="arccot"){
		a=log(a+pow(a*a+1, 0.5));
	} else if(op=="arccoth"){
		a=0.5*(log(1+1/a)-log(1-1/a));
	} else if(op=="arccosh"){
		a=log(a+sqrt(a-1)*sqrt(a+1));
// 	} else if(op=="arccsc"){
// 	} else if(op=="arccsch"){
// 	} else if(op=="arcsec"){
// 	} else if(op=="arcsech"){
// 	} else if(op=="arcsinh"){
// 	} else if(op=="arctanh"){
	} else if(op=="exp"){
		a=exp(a);
	} else if(op=="ln"){
		a=log(a);
	} else if(op=="log"){
		a=log10(a);
	} else if(op=="abs"){
		a=a>=0. ? a : -a;
	//} else if(op=="conjugate"){
	//} else if(op=="arg"){
	//} else if(op=="real"){
	//} else if(op=="imaginary"){
	} else if(op=="floor"){
		a=floor(a);
	} else if(op=="ceiling" ){
		a=ceil(a);
	} else if(op=="min" ){
		a= a < b? a : b;
	} else if(op=="max" ){
		a= a > b? a : b;
	} else if(op=="gt" ){
		a= a > b? 1.0 : 0.0;
		boolean=true;
	} else if(op=="lt" ){
		a= a < b? 1.0 : 0.0;
		boolean=true;
	} else if(op=="eq" ){
		a= a == b? 1.0 : 0.0;
		boolean=true;
	} else if(op=="neq" ){
		a= a != b? 1.0 : 0.0;
		boolean=true;
	} else if(op=="geq" ){
		a= a >= b? 1.0 : 0.0;
		boolean=true;
	} else if(op=="leq" ){
		a= a <= b? 1.0 : 0.0;
		boolean=true;
	} else if(op=="and" ){
		a= a && b? 1.0 : 0.0;
		boolean=true;
	} else if(op=="not"){
		a=!a;
		boolean = true;
	} else if(op=="or"){
		a= a || b? 1.0 : 0.0;
		boolean = true;
	} else if(op=="xor"){
		a= (a || b) && !(a&&b)? 1.0 : 0.0;
		boolean = true;
	} else if(op=="implies"){
		a= (a && !b)? 0.0 : 1.0;
		boolean = true;
	} else if(op=="gcd"){ //code by miquel canes :)
		int residu;
		while (b > 0) {
			residu = (int) a % (int) b;
			a = b;
			b = residu;
		}
	} else if(op=="lcm"){ //code by miquel canes :)
		int residu;
		double c=a*b;
		while (b > 0) {
			residu = (int) a % (int) b;
			a = b;
			b = residu;
		}
		a=(int)c/(int)a;
	} else if(op=="root") {
		a = (b==2.0)? sqrt(a) : pow(a, 1.0/b);
	}else {
		if(op!="") err += i18n("The operator <em>%1</em> hasn't been implemented<br/>").arg(op);
	}
	
	return a;
}

QStringList Analitza::bvar(QDomNode el){
	QStringList lambdas;
	QDomNodeList lambs = el.toElement().elementsByTagName(QString("bvar"));
	
	for(unsigned int i=0; i<lambs.count(); i++)
		lambdas << lambs.item(i).toElement().text();
	
	return lambdas;
}

QDomNode Analitza::uplimit(QDomNode el){
	return el.toElement().elementsByTagName(QString("uplimit")).item(0).firstChild();
}

QDomNode Analitza::downlimit(QDomNode el){
	return el.toElement().elementsByTagName(QString("downlimit")).item(0).firstChild();
}

QDomNode Analitza::first_val(QDomNode el){
	QDomNode a=el.firstChild();
	while(!a.isNull()){
		if(a.toElement().tagName() == "apply" || a.toElement().tagName() == "cn"  || a.toElement().tagName() == "ci")
			break;
		a=a.nextSibling();
	}
	return a;
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

double Analitza::toNum(QDomElement val){
	double ret=0.0;
	bool wrong;
	
	if(val.tagName()=="apply")
		return evalua(val);
	else if(val.tagName()=="ci") {
		ret = toNum(vars.value(val.text(), &wrong)); //a is a var
		if (wrong) {
			if(val.attribute("type").isNull())
				err += i18n("The variable <em>%1</em> doesn't exist<br />\n").arg(val.text());
			else if(val.attribute("type") == QString("function"))
				err += i18n("The function <em>%1</em> doesn't exist<br />\n").arg(val.text());
		}
	} else if(val.tagName() == "cn"){ // a is a number
		if(val.attribute("type","real") == "real"){
			ret= val.text().stripWhiteSpace().toDouble(&wrong); //TODO: Base on double not implemented
		} else if(val.attribute("type") == "integer"){
			int aux = val.text().stripWhiteSpace().toInt(&wrong, val.attribute("base", "10").toInt(NULL, 10));
			ret = (double) aux;
		} else if(val.attribute("type") == "e-notation")	{ /*TODO: Not implemented */ }
		else if(val.attribute("type") == "rational")		{ /*TODO: Not implemented */ }
		else if(val.attribute("type") == "complex-cartesian")	{ /*TODO: Not implemented */ }
		else if(val.attribute("type") == "complex-polar")	{ /*TODO: Not implemented */ }
		else if(val.attribute("type") == "constant"){
			if(val.text() == "&pi;")		{ ret = 3.1415926535897932384626433; }
			else if (val.text() == "&ee;" || val.text() == "&ExponentialE;"){ ret = exp(1.); }
			else if (val.text() == "&true;")	{ ret = toNum(vars.value("true")); }
			else if (val.text() == "&false;")	{ ret = toNum(vars.value("false")); }
			else if (val.text() == "&gamma;")	{ ret = 0.5772156649; }
			/*else if (val.text() == "&ImagniaryI;")	; //TODO: Not implemented 
			else if (val.text() == "&infin;")	; //TODO: Not implemented  }
			else if (val.text() == "&NaN;")		; //TODO: Not implemented  }*/
		}
	} else if(val.tagName()=="true")		ret = 1.0;
	else if(val.tagName()=="false")		ret = 0.0;
	else if(val.tagName()=="pi")		ret = 3.1415926535897932384626433;
	else if(val.tagName()=="exponentiale")	ret = exp(1.);
	else if(val.tagName()=="eulergamma")	ret = 0.5772156649;
	
	return ret;
}

int Analitza::isOperador(QString e){
	if (		e=="plus" ||
			e=="times" ||
			e=="min" ||
			e=="max" ||
			e=="and" ||
			e=="or"  ||
			e=="xor" ||
			e=="gcd" ||
			e=="lcm" ||
			e=="max" ||
			e=="min") {
		return -1;
	} else if (	e=="quotient" ||
			e=="power" ||
			e=="minus" ||
			e=="divide"  ||
			e=="rem" ||
			e=="factorof" ||
			e=="lt" ||
			e=="gt" ||
			e=="eq" ||
			e=="neq" ||
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
			e=="log" ||
			e=="not" ||
			e=="factorial" ||
			e=="minus" ||
			e=="abs" ||
			e=="conjugate" ||
			e=="arg" ||
			e=="real" ||
			e=="imaginary" ||
			e=="floor" ||
			e=="ceiling" ||
			e=="sum" ||
			e=="product") {
		return 1;
	}
	
	return false;
}

QStringList Analitza::lambda(){
	return bvar(elem.firstChild());
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
	return str(elem.firstChild());
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
	QString e=get_op(n);
	if(e=="")
		return 0;
	else if(e=="lt" || e=="gt" || e=="eq" || e=="neq" || e=="leq" || e=="geq")
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
	QDomNode j, k=n;
	QDomElement e;
	QString operador;
	QValueList<QString> nombres;
	unsigned int fills = 0;
// 	qDebug("%s", n.toElement().text().ascii());
	while( !n.isNull() ) {
		e = n.toElement();
		if(e.tagName() == "apply" || e.tagName() == ""){
			j = e.firstChild();
			if(toOpId(k) <= toOpId(j) || isOperador(get_op(j)) == 1) //It is not necessary to have everything between ()
				nombres.append(str(j));
			else
				nombres.append("(" + str(j) + ")");
		} else if (e.tagName() == "cn"){
			nombres.append(QString("<span class='num'>%1</span>").arg(e.text()));
		} else if (e.tagName() == "ci") {
			nombres.append(QString("<span class='var'>%1</span>").arg(e.text()));
		} else if(isNum(e.tagName())){
			nombres.append(e.tagName());
		} else if (isOperador(e.tagName())) {
			operador = e.tagName();
		} else if (e.tagName()=="declare") {
			j = e.firstChild(); //A <ci>
			nombres.append(j.toElement().text());
			j = j.nextSibling(); //Value to save
			nombres.append(str(j));
			operador = "declare";
			fills=3;
		} else if (e.tagName()=="lambda"){
			j = e.firstChild();
			QStringList lambdas = bvar(j);
			for (QStringList::Iterator it = lambdas.begin(); it != lambdas.end(); ++it){
				nombres.append(*it);
				j=j.nextSibling();
			}
			fills=3;
			nombres.append(str(j));
			operador = "lambda";
		}
		fills++;
		n= n.nextSibling();
	}
	
	QValueList<QString>::iterator it = nombres.begin();
	QString r= *it;
	
	if(fills>2)
		it++;
		
	for(; it != nombres.end(); ++it)
		r = escriuS(r,*it,operador, fills<=2?1:0);
	
	return r;
}

QString Analitza::escriuS(QString res, QString oper, QString op, int unari=0){
	res  =  res.stripWhiteSpace();
	oper = oper.stripWhiteSpace();
	
	QString r=res;
	if(op == "plus"){
		r = QString("%1<span class='op'>+</span>%2").arg(res).arg(oper);
	}else if(op =="times"){
		r = QString("%1<span class='op'>*</span>%2").arg(res).arg(oper);
	}else if(op=="quotient"){
		r = QString("%1<span class='op'>/</span>%2").arg(res).arg(oper);
	}else if(op=="minus"){
		r = unari ? QString("<span class='op'>-</span>%1").arg(res) : QString("%1<span class='op'>-</span>%2").arg(res).arg(oper);
	}else if(op=="power"){
		r = QString("%1<span class='op'>^</span>%2").arg(res).arg(oper);
	} else if(op=="declare"){
		r = QString("%1<span class='op'>:=</span>%2").arg(res).arg(oper);
	} else if(op=="lambda"){
		r = QString("%1<span class='op'>-></span>%2").arg(res).arg(oper);
	} else if(op != "") {
		if(unari)
			r = QString("<span class='func'>%1</span><span class='op'>(</span>%2<span class='op'>)</span>").arg(op).arg(res);
		else
			r = QString("<span class='func'>%1</span><span class='op'>(</span>%2,%3<span class='op'>)</span>").arg(op).arg(res).arg(oper);
		//qDebug("The operator: <%s /> is not implemented.", operador.latin1());
	}
	return r;
}

QString Analitza::escriuMMLP(QString res, QString oper, QString op, int unari=0){
	res  =  res.stripWhiteSpace();
	oper = oper.stripWhiteSpace();
	
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
		r = "coth(" + res + ")";
	} else if(op=="arcsin"){
		r = "arcsin(" + res + ")";
	} else if(op=="arccos"){
		r = "arccos(" + res + ")";
	} else if(op=="arctan"){
		r = "arctan(" + res + ")";
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
	} else if(op=="lambda"){
		r = res + "<mo> -> </mo>" + oper;
	} else if(op=="implies"){
		r = res + "<mo>=></mo>" + oper;
	} //else if(op=="conjugate"){
// 	} else if(op=="arg"){
// 	} else if(op=="real"){
// 	} else if(op=="imaginary"){
// 	} else if(op=="floor"){
// 		a=floor(a);
// 	} else if(op=="ceiling" ){
// 		a=ceil(a);
// 	}
	else {
		//qDebug("The operator: <%s /> is not implemented.", operador.latin1());
	}
	return r;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////                                                   /////////////////////
/////////////                  Other (static)                   /////////////////////
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

void print_dom(QDomNode in, int ind){
	QString a("");
	
	if(ind >100){
		qDebug("...");
		return;
	}
	
	for(int i=0; i<ind; i++)
		a.append("------");
	
	qDebug("%s%s(%s)", a.ascii(), in.toElement().tagName().ascii(), in.toElement().text().ascii());
	
	for(unsigned int i=0 ; i<in.childNodes().length(); i++){
		if(in.childNodes().item(i).nodeType()==QDomNode::ElementNode)
			print_dom(in.childNodes().item(i), ind+1);
	}
}
