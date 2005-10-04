#ifndef POLYNOME_H
#define POLYNOME_H
#include <qvaluelist.h>
#include "scalar.h"

/**
@author Aleix Pol,,,
*/

QString i18n(QString a);

class Monomial { //Without error check
public:
	Scalar m;
	Scalar exp;
	QString var;

	Monomial(Scalar val=Scalar(1.0), Scalar exp=Scalar(1.0), QString v="") : m(val), exp(exp), var(v)  {}
	bool setValue(const QDomElement &);
	static bool isMono(const QDomElement& theValue);
	Monomial operator+ (const Monomial &a) const { return Monomial(m+a.m, exp, var); } //both exp must be equal
	Monomial operator+=(const Monomial &a){ m+=a.m; return *this; }
	Monomial operator* (const Monomial &a) const { return Monomial(m*a.m, exp+a.exp, var); }
	Monomial operator*=(const Monomial &a) { m*=a.m; exp+=a.exp; return *this; };
	Monomial operator- (const Monomial &a) const { return Monomial(m-a.m, exp, var); } //both exp must be equal
	Monomial operator- () const { return Monomial(-m, exp, var); }
	Monomial operator-=(const Monomial &a) { m-=a.m; return *this; };
	Monomial operator/ (const Monomial &a) const { return Monomial(m/a.m, exp-a.exp, var); }
	Monomial operator/=(const Monomial &a) { m/=a.m; exp-=a.exp; return *this; }
	Monomial operator* (const Scalar &a) const { return Monomial(m*a , exp, var); }
	Monomial operator*=(const Scalar &a) { m*=a; return *this; };
	Monomial operator/ (const Scalar &a) const { return Monomial(m/a, exp, var); }
	Monomial operator/=(const Scalar &a) { m/=a; return *this; }
	
	bool operator==(const Monomial &a) const {return (m==a.m) && (exp==a.exp);}
	bool operator!=(const Monomial &a) const {return (m!=a.m) || (exp!=a.exp);}
	bool operator> (const Monomial &a) const {return (exp>a.exp) || (m>=a.m && (exp>a.exp));}
	bool operator< (const Monomial &a) const {return (exp<a.exp) || (m<=a.m && (exp<a.exp));}
	bool operator>=(const Monomial &a) const {return *this>a || *this==a;}
	bool operator<=(const Monomial &a) const {return *this<a || *this==a;}
	
	Monomial pow(const double &expo){ return Monomial(std::pow(m.value(), expo), exp*expo, var); }
	Monomial pow(const Scalar &expo){ return Monomial(std::pow(m.value(), exp.value()), exp*expo.value(), var); }
	Monomial abs() const { return Monomial(m>=0.? m : -m, exp, var); } //Wrong but handy for the moment
	Monomial derive(){ return Monomial(m*exp, exp-1., var); }
	QDomElement value(QDomDocument d);
	void print(const QString append="") const { qDebug("%s%f%s^%f", append.ascii(), m.value(), var.ascii(), exp.value()); } //debugging purposes only
	QString str() const { return QString("%1%2^%3").arg(m.value()).arg(var.ascii()).arg(exp.value()); } //idem
};

class Polynomial{
	QValueList<Monomial> poly;
	QString m_var;
	QString m_err;
public:
	Polynomial();
	~Polynomial();
	
	Scalar degree();
	Scalar member(const Scalar &exp);
	bool setValue(const QDomElement& theValue);
	QDomElement value(QDomDocument);
	static bool isPoly(const QDomElement& theValue);
	
	Polynomial operator=(const bool& m) {m_err=i18n("Cant assign %1 to a Polynomial").arg(m); return *this; } //receiving a boolean value always means error
	Polynomial operator=(const Scalar& m) { poly.clear(); *this += Monomial(m, 0.); return *this; }
	Polynomial operator=(const Monomial& m) { poly.clear(); *this += m ; return *this; }
	
	Polynomial operator+(const Monomial& m);
	Polynomial operator+=(const Monomial& m);
	Polynomial operator*(const Monomial& m);
	Polynomial operator*=(const Monomial& m);
	Polynomial operator-(const Monomial& m);
	Polynomial operator-=(const Monomial& m);
	Polynomial operator/(const Monomial& m);
	Polynomial operator/=(const Monomial& m);
	
	//ops
	Polynomial operator+( Polynomial &a);
	Polynomial operator+=( Polynomial a);
	Polynomial operator*( Polynomial &a);
	Polynomial operator*=( Polynomial &a);
	Polynomial operator-( Polynomial &a);
	Polynomial operator-() const;
	Polynomial operator-=( Polynomial &a);
	Polynomial operator/( Polynomial &a);
	Polynomial operator/=( Polynomial &a);
	Polynomial operator++();
	Polynomial operator--();
	Polynomial operator++(int);
	Polynomial operator--(int);
	bool operator&&(Polynomial &a){ m_err=i18n("Can't operate && %1").arg(a.str()); return false;} //not implemented
	bool operator||(Polynomial &a){ m_err=i18n("Can't operate || %1").arg(a.str()); return false;} //not implemented
	bool operator!(){ m_err=i18n("Can't negate %1").arg((*this).str()); return false;} //not implemented
	bool operator==(Polynomial &a){ return poly==a.poly;}
	bool operator!=(Polynomial &a){ return poly!=a.poly;}
	//Here we check whether the degree is greater or not
	bool operator>(Polynomial &a){ return degree() > a.degree(); }
	bool operator<(Polynomial &a){ return degree() < a.degree(); }
	bool operator>=(Polynomial &a){ return degree() >= a.degree(); }
	bool operator<=(Polynomial &a){ return degree() <= a.degree(); }
	bool divide(Polynomial &a){ m_err=i18n("Can't divide a polynomial like %1").arg(a.str()); return *this==a;} //not accurate
	bool implies(Polynomial &a){ m_err=i18n("Can't calculate implication with %1").arg(a.str()); return false;} //not implemented
	Polynomial sin(){ m_err=i18n("Can't calculate Polynomial's %1 sinus").arg((*this).str()); return *this; } //not implemented
	Polynomial cos(){ m_err=i18n("Can't calculate Polynomial's %1 cosinus").arg((*this).str()); return *this; } //not implemented
	Polynomial tan(){ m_err=i18n("Can't calculate Polynomial's %1 tangent").arg((*this).str()); return *this; } //not implemented
	Polynomial sec(){ m_err=i18n("Can't calculate Polynomial's %1 secant").arg((*this).str()); return *this; } //not implemented
	Polynomial csc(){ m_err=i18n("Can't calculate Polynomial's %1 cosecant").arg((*this).str()); return *this; } //not implemented
	Polynomial cot(){ m_err=i18n("Can't calculate Polynomial's %1 cotangent").arg((*this).str()); return *this; } //not implemented
	Polynomial sinh(){ m_err=i18n("Can't calculate Polynomial's %1 hyperbolic sinus").arg((*this).str()); return *this; } //not implemented
	Polynomial cosh(){ m_err=i18n("Can't calculate Polynomial's %1 hyperbolic cosinus").arg((*this).str()); return *this; } //not implemented
	Polynomial tanh(){ m_err=i18n("Can't calculate Polynomial's %1 hyperbolic tangent").arg((*this).str()); return *this; } //not implemented
	Polynomial sech(){ m_err=i18n("Can't calculate Polynomial's %1 hyperbolic secant").arg((*this).str()); return *this; } //not implemented
	Polynomial csch(){ m_err=i18n("Can't calculate Polynomial's %1 hyperbolic cosecant").arg((*this).str()); return *this; } //not implemented
	Polynomial coth(){ m_err=i18n("Can't calculate Polynomial's %1 hyperbolic cotangent").arg((*this).str()); return *this; } //not implemented
	Polynomial asin(){ m_err=i18n("Can't calculate Polynomial's %1 arcsinus").arg((*this).str()); return *this; } //not implemented
	Polynomial acos(){ m_err=i18n("Can't calculate Polynomial's %1 arccosinus").arg((*this).str()); return *this; } //not implemented
	Polynomial atan(){ m_err=i18n("Can't calculate Polynomial's %1 arctangent").arg((*this).str()); return *this; } //not implemented
	Polynomial arccot(){ m_err=i18n("Can't calculate Polynomial's %1 arccotangent").arg((*this).str()); return *this; } //not implemented
	Polynomial arccoth(){ m_err=i18n("Can't calculate Polynomial's %1 hyperbolic arccotangent").arg((*this).str()); return *this; } //not implemented
	Polynomial arccosh(){ m_err=i18n("Can't calculate Polynomial's %1 hyperbolic arccosinus").arg((*this).str()); return *this; } //not implemented
	Polynomial exp(){ m_err=i18n("Can't calculate Polynomial's %1 exponential").arg((*this).str()); return *this; } //not implemented
	Polynomial log10(){ m_err=i18n("Can't calculate Polynomial's %1 log_10").arg((*this).str()); return *this; } //not implemented
	Polynomial log(){ m_err=i18n("Can't calculate Polynomial's %1 log_e").arg((*this).str()); return *this; } //not implemented
	Polynomial floor(){ m_err=i18n("Can't calculate Polynomial's %1 floor").arg((*this).str()); return *this; } //not implemented
	Polynomial ceil(){ m_err=i18n("Can't calculate Polynomial's %1 ceil").arg((*this).str()); return *this; } //not implemented
	Polynomial sqrt(){ m_err=i18n("Can't calculate Polynomial's %1 square root").arg((*this).str()); return *this; } //not implemented
	Polynomial root(const double &exp){ m_err=i18n("Can't calculate Polynomial's %1 root %2").arg((*this).str()).arg(exp); return *this; } //not implemented
	Polynomial root(Polynomial &exp){ m_err=i18n("Can't calculate Polynomial's %1 root %2").arg((*this).str()).arg(exp.str()); return *this; } //not implemented
	Polynomial root(Scalar &exp) { m_err=i18n("Can't calculate Polynomial's %1 root %2").arg((*this).str()).arg(exp.str()); return *this; } //not implemented
// 	Polynomial pow(double exp){ m_err=i18n("Can't calculate Polynomial's %1 power").arg(a.str()); return *this; } //not implemented
	Polynomial pow( Polynomial &exp){ m_err=i18n("Can't calculate Polynomial's %1 power %2").arg((*this).str().arg(exp.str())); return *this; } //not implemented
	Polynomial pow(const Scalar &exp);
	
	Polynomial factorial(){ m_err=i18n("Can't calculate Polynomial's %1 %s").arg((*this).str()); return *this; }  //not implemented
	Polynomial abs(){ m_err=i18n("Can't calculate Polynomial's %1 %s").arg((*this).str()); return *this; } //not implemented
	Polynomial derive();
	Polynomial gcd(Polynomial &a){ m_err=i18n("Can't calculate Polynomial's %1 %s").arg(a.str()); return *this; } //not implemented
	Polynomial lcm(Polynomial &a){ m_err=i18n("Can't calculate Polynomial's %1 %s").arg(a.str()); return *this; } //not implemented
	void print(QString append);
	QString str();
	
	/////////Polynomial op Scalar Overloads////////////
	Polynomial operator+ (const Scalar& s) { return *this+ Monomial(s, 0.); }
	Polynomial operator+=(const Scalar& s) { return *this+=Monomial(s, 0.); }
	Polynomial operator* (const Scalar& s) { return *this* Monomial(s, 0.); }
	Polynomial operator*=(const Scalar& s) { return *this*=Monomial(s, 0.); }
	Polynomial operator- (const Scalar& s) { return *this- Monomial(s, 0.); }
	Polynomial operator-=(const Scalar& s) { return *this-=Monomial(s, 0.); }
	Polynomial operator/ (const Scalar& s) { return *this/ Monomial(s, 0.); }
	Polynomial operator/=(const Scalar& s) { return *this/=Monomial(s, 0.); }
	bool divide(const Scalar &s){ m_err=i18n("Can't divide %1 with %2").arg((*this).str()).arg(s.str()); return false;} //not implemented
	bool implies(const Scalar &a){ m_err=i18n("Can't calculate implication with %1").arg(a.str()); return false;} //not implemented
	bool operator&&(const Scalar &a){ m_err=i18n("Can't operate && %1").arg(a.str()); return false;} //not implemented
	bool operator||(const Scalar &a){ m_err=i18n("Can't operate || %1").arg(a.str()); return false;} //not implemented
	bool operator==(const Scalar &a){ return poly.count()==1 && a==member(0.); }
	bool operator!=(const Scalar &a){ return poly.count()!=1 || a!=member(0.); }
	bool operator>(const Scalar &a){ m_err=i18n("Can't know whether %1 > %2").arg(a.str()).arg((*this).str()); return false; }
	bool operator<(const Scalar &a){ m_err=i18n("Can't know whether %1 < %2").arg(a.str()).arg((*this).str()); return false; }
	bool operator>=(const Scalar &a){ m_err=i18n("Can't know whether %1 >= %2").arg(a.str()).arg((*this).str()); return false; }
	bool operator<=(const Scalar &a){ m_err=i18n("Can't know whether %1 <= %2").arg(a.str()).arg((*this).str()); return false; }
	Polynomial gcd(const Scalar &a){ m_err=i18n("Can't calculate Polynomial's %1 %s").arg(a.str()); return *this; } //not implemented
	Polynomial lcm(const Scalar &a){ m_err=i18n("Can't calculate Polynomial's %1 %s").arg(a.str()); return *this; } //not implemented
};

#endif
