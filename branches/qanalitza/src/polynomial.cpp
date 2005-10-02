#include "polynomial.h"

Polynomial::Polynomial() : m_err(false){}
Polynomial::~Polynomial(){}


bool Polynomial::isPoly(const QDomElement& s){
	bool ispoly=false;
	//qDebug("<%s>%s", s.tagName().ascii(), s.childNodes().item(0).toElement().tagName().ascii());
	if(s.tagName()=="apply" && s.childNodes().item(0).toElement().tagName()=="plus"){
		ispoly=true;
		for(unsigned int i=1;i<s.childNodes().length() && ispoly ; i++){
			ispoly=Monomial::isMono(s.childNodes().item(i).toElement()) || Scalar::isScalar(s.childNodes().item(i).toElement());
		}
	} else if(Monomial::isMono(s)) ispoly=true;
	return ispoly;
}



bool Polynomial::setValue(const QDomElement& s){
	bool ispoly=false;
	m_var="";
	poly.clear();
	Monomial m = Monomial(0., 0., "");
	if(s.tagName()=="apply" && s.childNodes().item(0).toElement().tagName()=="plus"){
		ispoly=true;
		for(unsigned int i=1;i<s.childNodes().length() && ispoly; i++){
			if(Monomial::isMono(s.childNodes().item(i).toElement())){
				m.setValue(s.childNodes().item(i).toElement());
				*this += m;
			} else if(Scalar::isScalar(s.childNodes().item(i).toElement())){
				m.setValue(s.childNodes().item(i).toElement());
				*this += m;
			} else {
				ispoly=false;
			}
			m_var=m.var;
		}
	} else if(Monomial::isMono(s)) {
		m.setValue(s);
		poly.append(m);
		ispoly=true;
	}
	return ispoly;
}

QDomElement Polynomial::value(QDomDocument d){
	QDomElement e;
	e=d.createElement("apply");
	e.appendChild(d.createElement("plus"));
	
	for(QValueList<Monomial>::iterator it = poly.begin(); it != poly.end(); ++it) {
		e.appendChild((*it).value(d));
	}
	return e;
}

/*******************************************/
/**************Operadors a saco*************/
/*****************PolyOpMono****************/
/*******************************************/

Polynomial Polynomial::operator+(const Monomial& m){
	Polynomial p = *this;
	bool found=false;
	for(QValueList<Monomial>::iterator it = p.poly.begin(); !found && it != p.poly.end(); ++it) {
		if((*it).exp==m.exp) {
			(*it)+=m;
			found=true;
		}
	}
	if(!found)
		p.poly.append(m);
	return p;
}

Polynomial Polynomial::operator+=(const Monomial& m){
	bool found=false;
	for(QValueList<Monomial>::iterator it = poly.begin(); !found && it != poly.end(); ++it) {
		if((*it).exp==m.exp) {
			(*it)+=m;
			found=true;
		}
	}
	if(!found)
		poly.append(m);
// 	qDebug("hola += %f", m.m.value());
	return *this;
}

Polynomial Polynomial::operator*(const Monomial& m){
	Polynomial p = *this;
	for(QValueList<Monomial>::iterator it = p.poly.begin(); it != p.poly.end(); ++it) {
		(*it)*=m;
	}
	return p;
}

Polynomial Polynomial::operator*=(const Monomial& m){
	for(QValueList<Monomial>::iterator it = poly.begin(); it != poly.end(); ++it) {
		(*it)*=m;
	}
	return *this;
}

Polynomial Polynomial::operator-(const Monomial& m){
	Polynomial p = *this;
	bool found=false;
	for(QValueList<Monomial>::iterator it = p.poly.begin(); !found && it != p.poly.end(); ++it) {
		if((*it).exp==m.exp) {
			(*it)-=m;
			found=true;
		}
	}
	if(!found)
		p.poly.append(-m);
	return p;
}

Polynomial Polynomial::operator-=(const Monomial& m){
	bool found=false;
	for(QValueList<Monomial>::iterator it = poly.begin(); !found && it != poly.end(); ++it) {
		if((*it).exp==m.exp) {
			(*it)-=m;
			found=true;
		}
	}
	if(!found)
		poly.append(-m);
	return *this;
}

Polynomial Polynomial::operator/(const Monomial& m){
	Polynomial p = *this;
	for(QValueList<Monomial>::iterator it = p.poly.begin(); it != p.poly.end(); ++it) {
		(*it)/=m;
	}
	return p;
}

Polynomial Polynomial::operator/=(const Monomial& m){
	for(QValueList<Monomial>::iterator it = poly.begin(); it != poly.end(); ++it) {
		(*it)*=m;
	}
	return *this;
}

/*******************************************/
/**************Operadors a saco*************/
/*****************PolyOpPoly****************/
/*******************************************/

Polynomial Polynomial::operator+(Polynomial &a){
	Polynomial p=*this;
	for(QValueList<Monomial>::iterator it = a.poly.begin(); it != a.poly.end(); ++it) {
		p+=(*it);
	}
	return p;
}

Polynomial Polynomial::operator+=(Polynomial a){
	for(QValueList<Monomial>::iterator it = a.poly.begin(); it != a.poly.end(); ++it) {
		*this+=(*it);
	}
	return *this;
}

Polynomial Polynomial::operator*(Polynomial &a){
	Polynomial p=Polynomial();
	for(QValueList<Monomial>::iterator it = a.poly.begin(); it != a.poly.end(); ++it) {
		Monomial m=(*it);
		p+= *this*m;
		(*it).print("It's meeeeee");
	}
	
	return p;
}

Polynomial Polynomial::operator*=(Polynomial &a){
	Polynomial p=Polynomial();
	for(QValueList<Monomial>::iterator it = a.poly.begin(); it != a.poly.end(); ++it) {
		Monomial m=(*it);
		p+= *this*m;
	}
	*this = p;
	return *this;
}

Polynomial Polynomial::operator-(Polynomial &a){
	Polynomial p=*this;
	for(QValueList<Monomial>::iterator it = a.poly.begin(); it != a.poly.end(); ++it) {
		p-=(*it);
	}
	return p;
}

Polynomial Polynomial::operator-=(Polynomial &a){
	for(QValueList<Monomial>::iterator it = a.poly.begin(); it != a.poly.end(); ++it) {
		*this-=(*it);
	}
	return *this;
}

Polynomial Polynomial::operator-() const {
	Polynomial p=*this;
	for(QValueList<Monomial>::iterator it = p.poly.begin(); it != p.poly.end(); ++it) {
		(*it)=-(*it);
	}
	return *this;
}

Polynomial Polynomial::operator/( Polynomial &a){
	Polynomial p=*this;
	for(QValueList<Monomial>::iterator it = a.poly.begin(); it != a.poly.end(); ++it) {
		p/=(*it);
	}
	return p;
}

Polynomial Polynomial::operator/=( Polynomial &a){
	for(QValueList<Monomial>::iterator it = a.poly.begin(); it != a.poly.end(); ++it) {
		*this/=(*it);
	}
	return *this;
}

Polynomial Polynomial::pow(const Scalar &exp){
	Polynomial p = *this;
	for(int i=exp.value(); i>0; i--){
		*this *= p;
	}
	return *this;
}

Polynomial Polynomial::operator++(){return *this+=Monomial(1.,0., m_var);}
// Polynomial Polynomial::operator--(){return *this-=Monomial(1.,0.);}
Polynomial Polynomial::operator++(int){return *this+=Monomial(1.,0., m_var);}
// Polynomial Polynomial::operator--(int){return *this-=Monomial(1.,0.);}

Scalar Polynomial::degree() {
	Scalar max=0.0;
	for(QValueList<Monomial>::iterator it = poly.begin(); it != poly.end(); ++it) {
		if((*it).exp>max)
			max=(*it).exp;
	}
	return max;
}

Scalar Polynomial::member(const Scalar &exp) {
		for(QValueList<Monomial>::iterator it = poly.begin(); it != poly.end(); ++it) {
		if((*it).exp==exp)
			return (*it).m;
	}
	return Scalar(0.);
}




////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
///////////                                              ///////////////
///////////                Monomial Class                ///////////////
///////////                                              ///////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

bool Monomial::isMono(const QDomElement& s){
	bool ret=false;
	if(s.tagName()=="ci") {
		ret= true;
	} else if(s.tagName()=="apply" && s.childNodes().length()==3){
		if(s.childNodes().item(0).toElement().tagName()=="power" &&  s.childNodes().item(1).toElement().tagName()=="ci" && Scalar::isScalar(s.childNodes().item(2).toElement()))
			ret= true;
		else if(s.childNodes().item(0).toElement().tagName()=="times" && Scalar::isScalar(s.childNodes().item(1).toElement()) && s.childNodes().item(2).toElement().tagName()=="ci")//2x
			ret= true;
		else if(s.childNodes().item(0).toElement().tagName()=="times" && Scalar::isScalar(s.childNodes().item(1).toElement()) && s.childNodes().item(2).toElement().tagName()=="apply" && s.childNodes().item(2).childNodes().item(0).toElement().tagName()=="power" &&  s.childNodes().item(2).childNodes().item(1).toElement().tagName()=="ci" && Scalar::isScalar(s.childNodes().item(2).childNodes().item(2).toElement()))
			ret= true;
	}
	return ret;
}

bool Monomial::setValue(const QDomElement& s){ //We don't allow x^(1/2) as a monomial, even though exp is a double value
	bool ret=false;
	m=1.;
	exp=1.;
	if(s.tagName()=="ci") {
		var=s.text();
		ret=true;
	} else if(s.tagName()=="apply" && s.childNodes().length()==3){
		if(s.childNodes().item(0).toElement().tagName()=="power" &&  s.childNodes().item(1).toElement().tagName()=="ci" && Scalar::isScalar(s.childNodes().item(2).toElement())) { //x^2
			var = s.childNodes().item(1).toElement().text();
			exp.setValue(s.childNodes().item(2).toElement());
			ret=true;
		} else if(s.childNodes().item(0).toElement().tagName()=="times" && Scalar::isScalar(s.childNodes().item(1).toElement()) && s.childNodes().item(2).toElement().tagName()=="ci") {//3x
			m.setValue(s.childNodes().item(1).toElement());
			var = s.childNodes().item(2).toElement().text();
			ret=true;
		} else if(s.childNodes().item(0).toElement().tagName()=="times" && Scalar::isScalar(s.childNodes().item(1).toElement()) && s.childNodes().item(2).toElement().tagName()=="apply" && s.childNodes().item(2).childNodes().item(0).toElement().tagName()=="power" &&  s.childNodes().item(2).childNodes().item(1).toElement().tagName()=="ci" && Scalar::isScalar(s.childNodes().item(2).childNodes().item(2).toElement())){ //3x^2
			m.setValue(s.childNodes().item(1).toElement());
			exp.setValue(s.childNodes().item(2).childNodes().item(2).toElement());
			var = s.childNodes().item(2).childNodes().item(1).toElement().text();
			ret=true;
		}
	}
	return ret;
}

QDomElement Monomial::value(QDomDocument d){
	QDomElement e;
	
	if(exp.value()==0.0){
		e.appendChild(exp.value(d));
		qDebug("bonks");
	} else if(m.value()==1. && exp.value()==1.0){
		e=d.createElement("ci");
		e.appendChild(d.createTextNode(var));
	} else if(m.value()!=1. && exp.value() == 1.0){
		e=d.createElement("apply");
		e.appendChild(d.createElement("times"));
			QDomElement q = d.createElement("cn");
			q.appendChild(m.value(d));
		e.appendChild(q);
			QDomElement v = d.createElement("ci");
			v.appendChild(d.createTextNode(var));
		e.appendChild(v);
	} else if(m.value()==1. && exp.value() != 1.0){
		e=d.createElement("apply");
		e.appendChild(d.createElement("power"));
		QDomElement v = d.createElement("ci");
		v.appendChild(d.createTextNode(var));
		e.appendChild(v);
		QDomElement q = d.createElement("cn");
		q.appendChild(exp.value(d));
		e.appendChild(q);
	} else if(m.value()!=1. && exp.value() != 1.0){
		qDebug("%f%s^%f", m.value(), var.ascii(), exp.value() );
		e=d.createElement("apply");
		e.appendChild(d.createElement("times"));
		QDomElement q = d.createElement("cn");
		q.appendChild(m.value(d));
		e.appendChild(q);
		QDomElement k=d.createElement("apply");;
		k.appendChild(d.createElement("power"));
			QDomElement v = d.createElement("ci");
			v.appendChild(d.createTextNode(var));
			QDomElement y = d.createElement("cn");
			y.appendChild(exp.value(d));
			k.appendChild(v);
			k.appendChild(y);
		e.appendChild(k);
	}
	return e;
}


void Polynomial::print(QString append="") {
	qDebug("%s%s", append.ascii(), str().ascii());
}

QString Polynomial::str() {
	QString res=QString("");
	QValueList<Monomial>::iterator it = poly.begin();
	res += (*it).str();
	++it;
	
	for(; it != poly.end(); ++it) {
		res += "+"+(*it).str();
	}
	return res;
}

