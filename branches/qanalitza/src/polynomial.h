#ifndef POLYNOME_H
#define POLYNOME_H
#include <qvaluelist.h>
#include "scalar.h"

/**
@author Aleix Pol,,,
*/

class Monomial { //Without error check
public:
	Scalar m;
	Scalar exp;
	QString var;

	Monomial(Scalar val=Scalar(1.0), Scalar exp=Scalar(1.0), QString v="x") : m(val), exp(exp), var(v)  {}
	bool setValue(const QDomElement &);
	static bool isMono(const QDomElement& theValue);
	Monomial operator+ (const Monomial &a) const { return Monomial(m+a.m, exp, var); } //both exp must be equal 
	Monomial operator+=(const Monomial &a) { m+=a.m; return *this; }
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
	QString str() const { QString s = QString("%1%2^%3").arg(m.value()).arg(var.ascii()).arg(exp.value()); return s; } //idem
};

class Polynomial{
	QValueList<Monomial> poly;
	char m_var;
	bool m_err;
public:
	Polynomial();
	~Polynomial();
	
	Scalar degree();
	bool setValue(const QDomElement& theValue);
	QDomElement value(QDomDocument);
	static bool isPoly(const QDomElement& theValue);
	
	Polynomial operator=(const bool& m) {m_err=true; return *this; } //receiving a boolean value always means error
	
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
	bool operator&&(const Polynomial &a){ m_err=true; return false;} //not implemented
	bool operator||(const Polynomial &a){ m_err=true; return false;} //not implemented
	bool operator!(){ m_err=true; return false;} //not implemented
	bool operator==(const Polynomial &a){ m_err=true; return false;} //not implemented
	bool operator!=(const Polynomial &a){ m_err=true; return false;} //not implemented
	//Here we check whether the degree is greater or not
	bool operator>(const Polynomial &a){ m_err=true; return false;} //not implemented
	bool operator<(const Polynomial &a){ m_err=true; return false;} //not implemented
	bool operator>=(const Polynomial &a){ m_err=true; return false;} //not implemented
	bool operator<=(const Polynomial &a){ m_err=true; return false;} //not implemented
	bool divide(const Polynomial &a){ m_err=true; return false;} //not implemented
	bool implies(const Polynomial &a){ m_err=true; return false;} //not implemented
	Polynomial sin(){ m_err=true; return *this; } //not implemented
	Polynomial cos(){ m_err=true; return *this; } //not implemented
	Polynomial tan(){ m_err=true; return *this; } //not implemented
	Polynomial sec(){ m_err=true; return *this; } //not implemented
	Polynomial csc(){ m_err=true; return *this; } //not implemented
	Polynomial cot(){ m_err=true; return *this; } //not implemented
	Polynomial sinh(){ m_err=true; return *this; } //not implemented
	Polynomial cosh(){ m_err=true; return *this; } //not implemented
	Polynomial tanh(){ m_err=true; return *this; } //not implemented
	Polynomial sech(){ m_err=true; return *this; } //not implemented
	Polynomial csch(){ m_err=true; return *this; } //not implemented
	Polynomial coth(){ m_err=true; return *this; } //not implemented
	Polynomial asin(){ m_err=true; return *this; } //not implemented
	Polynomial acos(){ m_err=true; return *this; } //not implemented
	Polynomial atan(){ m_err=true; return *this; } //not implemented
	Polynomial arccot(){ m_err=true; return *this; } //not implemented
	Polynomial arccoth(){ m_err=true; return *this; } //not implemented
	Polynomial arccosh(){ m_err=true; return *this; } //not implemented
	Polynomial exp(){ m_err=true; return *this; } //not implemented
	Polynomial log10(){ m_err=true; return *this; } //not implemented
	Polynomial log(){ m_err=true; return *this; } //not implemented
	Polynomial floor(){ m_err=true; return *this; } //not implemented
	Polynomial ceil(){ m_err=true; return *this; } //not implemented
	Polynomial sqrt(){ m_err=true; return *this; } //not implemented
	Polynomial root(double exp){ m_err=true; return *this; } //not implemented
	Polynomial root(Polynomial exp){ m_err=true; return *this; } //not implemented
	Polynomial pow(double exp){ m_err=true; return *this; } //not implemented
	Polynomial pow(Polynomial exp){ m_err=true; return *this; } //not implemented
	Polynomial factorial(){ m_err=true; return *this; }  //not implemented
	Polynomial abs(){ m_err=true; return *this; } //not implemented
	Polynomial derive();
	Polynomial gcd(const Polynomial &a){ m_err=true; return *this; } //not implemented
	Polynomial lcm(const Polynomial &a){ m_err=true; return *this; } //not implemented
};

#endif
