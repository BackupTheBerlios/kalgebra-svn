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
	QDomDocument doc;
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

bool Analitza::isNum(QString s){
// 	cout << s.as	cii() << endl;
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
	bool err;
// 	cout << "<" << val.tagName() << ">" << val.text() << "</" << val.tagName() << ">" << endl;
	if(val.tagName()=="apply")
		return toNum(evalua(val));
	else if(val.tagName() == "cn"){ // a es un nombre
		if(val.attribute("type","real") == "real"){
			ret= val.text().stripWhiteSpace().toDouble(&err); //TODO: Base on double not implemented
		} else if(val.attribute("type") == "e-notation"){
			//TODO: Not implemented
		} else if(val.attribute("type") == "integer"){
			int aux = val.text().stripWhiteSpace().toInt(&err, val.attribute("base", "10").toInt(NULL, 10));
			ret = (double) aux;
		}
		else if(val.attribute("type") == "rational"){
			//TODO: Not implemented
		}
		else if(val.attribute("type") == "complex-cartesian"){
			//TODO: Not implemented
		}
		else if(val.attribute("type") == "complex-polar"){
			//TODO: Not implemented
		}
		else if(val.attribute("type") == "constant"){
			if(val.text() == "&pi;"){
				ret = 3.141592653f;
			} else if (val.text() == "&ee;" || val.text() == "&ExponentialE;"){
				ret = 2.718281828f;
			}  else if (val.text() == "&ImagniaryI;"){
				//TODO: Not implemented
			} else if (val.text() == "&gamma;"){
				ret = 0.5772156649f;
			} else if (val.text() == "&infin;"){
				//TODO: Not implemented
			} else if (val.text() == "&NaN;"){
				//TODO: Not implemented
			} else if (val.text() == "&true;"){
				ret = toNum(vars.value("true"));
			} else if (val.text() == "&false;"){
				ret = toNum(vars.value("false"));
			}
		}
	} else if(val.tagName() == "ci") //si a es variable
		return toNum(vars.value(val.text()));
	else if(val.tagName()=="true")
		ret = 1.0;
	else if(val.tagName()=="false")
		ret = 0.0;
	else if(val.tagName()=="pi")
		ret = 3.141592653f;
	else if(val.tagName()=="exponentiale")
		ret = 2.718281828f;
	else if(val.tagName()=="eulergamma")
		ret = 0.5772156649f;
// 	printf("\n---->%f\n", ret);
	return ret;
}

QDomElement Analitza::opera(QDomElement res, QDomElement oper, QString op, int minus=0){
	double a; 
	double b;
	bool boolean=false;
	
	a = toNum(res);
	b = toNum(oper);
	
// 	cout << op << " a:"<< a << " b: " << b << "<->" <<minus << endl;
	
	if(op == "plus"){
		a += b;
	}else if(op =="times"){
		a *= b;
	}else if(op=="divide"){
		//a = ((a % b)==0) ? 1.0 : 0.0;
		boolean = true;
	}else if(op=="minus"){
		a = minus ? -a : a-b;
	}else if(op=="power"){
		a = pow(a, b);
	}else if(op=="quotient"){
		a /= b;
	}else if(op=="factorial"){
		b = a;
		for(a=1; b>1; b--)
			a*=b;
	} else if(op=="abs"){
		a= floor(a);
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
		a=floor(a);
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
	} else {
		err += i18n("The operator %1 hasn't been implemented").arg(op);
// 		qDebug("lol %s", op.ascii());
	}
	
	QDomDocument d;
	QString z;
	if (boolean)
		z =  a!=0.0 ? "<true />" : "<false />" ;
		else
		z = QString( "<cn> %1 </cn>" ).arg(a, 0, 'g', 16);
	
	d.setContent(z);
	QDomElement ret = d.documentElement();
	return ret;
}

QDomElement Analitza::evalua(QDomNode n){
	QDomNode j ;
	QDomElement e;
	QString operador;
	QValueList<QDomElement> nombres;
	unsigned int fills = 0;
	
	while( !n.isNull() ) {
		e = n.toElement();
		if(e.tagName() == "apply"){
			j = e.firstChild();
			nombres.append( evalua(j) );
		} else if (e.tagName() == "cn" || e.tagName() == "ci" || isNum(e.tagName()))
			nombres.append(e);
		else if (e.tagName() == "declare"){
			j = e.firstChild(); //Segons l'estandar, es <ci>
			QString s = j.toElement().text();
			j = j.nextSibling();
// 			vars.modifica(s, evalua(j.toElement()));
			vars.modifica(s, j.toElement());
		}
		else if (isOperador(e.tagName()))
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

	for( ; it != nombres.end(); ++it)
		ret = opera(ret,*it,operador, fills==2?1:0);
	
	return ret ;
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
			e=="implies"  ){
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
	
	cout << QString::QString("aaaa--------->%1 %2\n").arg(operador).arg(fills).ascii() << endl;
	
	for(; it != nombres.end(); ++it){
		cout << (*it).ascii() << endl;
		r = escriu(r,*it,operador, fills<=2?1:0);
	}
	
// 	cout << "hola" << endl;
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
		r = "<mo>¬</mo>" + res;
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
// 		a=floor(a);
// 	} else if(op=="ceiling" ){
// 		a=ceil(a);
// 	}
	
	else {
// 		cout << "The operator: <" << op.latin1() << " /> is not implemented." << endl;
	}
	return r;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////                                                   /////////////////////
/////////////                   Classe Variables                /////////////////////
/////////////                                                   /////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

Variables::Variables(){
// 	afegeix("x", 33.0f);
	modifica("true", 1.0f);
	modifica("false", 0.0f);
	modifica("pi", 3.141592653f);
	modifica("e", 2.718281828f);
	modifica("euler", 0.5772156649f);
}

inline void Variables::afegeix(QString id, QDomElement val){
	modifica(id,val);
}

void Variables::treu(QString id){
	QValueList<struct VARIABLE>::iterator it;
	struct VARIABLE aux;
	it = vars.begin() ;
	for(; it != vars.end(); ++it){ 
		aux = *it;
		if(aux.nom == id){
			vars.remove(it);
			break;
		}
	}
}

void Variables::modifica(QString id, double new_val){
	QString z = QString( "<cn> %1 </cn>" ).arg(new_val, 0, 'g', 16);
	QDomDocument a;
	QString err = "<cn>" + z + "</cn>";
	a.setContent(err);
	modifica(id,a.documentElement());
}

void Variables::modifica(QString id, QDomElement new_val){
	struct VARIABLE aux;
	
	QValueList<struct VARIABLE>::iterator it;
	it = vars.begin() ;
	for(; it != vars.end(); ++it){ 
		aux = *it;
		if(aux.nom == id){
			vars.remove(it);
			break;
		}
	}
	aux.nom=id;
	aux.valor=new_val;
	vars.append(aux);
}
QDomElement Variables::value(QString id){
	struct VARIABLE aux;
	QValueList<struct VARIABLE>::iterator it;
	it = vars.begin() ;
	for(; it != vars.end(); ++it){ 
		aux = *it;
		if(aux.nom.stripWhiteSpace() == id.stripWhiteSpace()){
			cout << "-->" << aux.valor.toDocument().toString().ascii() << endl;
			return aux.valor;
		}
	}
	//error
	QDomDocument a; QString err("<cn \"constant\"> &NaN </cn>");
	a.setContent(err);
	return a.documentElement();
}

QStringList Variables::getNoms(){
	struct VARIABLE aux;
	QStringList out;
	QValueList<struct VARIABLE>::iterator it;
	it = vars.begin() ;
	for(; it != vars.end(); ++it){ 
		aux = *it;
		out << aux.nom;
	}
	return out;
}
