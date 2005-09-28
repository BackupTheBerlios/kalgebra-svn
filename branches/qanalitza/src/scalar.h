#ifndef SCALAR_H
#define SCALAR_H
#include <qdom.h>
#include <cmath>

/**
@author Aleix Pol,,,
*/

class Scalar{
	double m_value;
	bool m_boolean;
public:
	Scalar(double val=0.0, bool b=false) : m_value(val), m_boolean(b) {}
	~Scalar();
	static bool isScalar(QDomElement e){ return (e.tagName()=="cn");}
	void setValue(const QDomElement& theValue);
	double value() const { return m_value; }
	QDomElement value(QDomDocument d);
	
	//ops
	Scalar operator+(const Scalar &a) const {return Scalar(m_value + a.m_value);}
	Scalar operator+=(const Scalar &a){ m_value += a.m_value; return *this; }
	Scalar operator*(const Scalar &a) const {return Scalar(m_value * a.m_value);}
	Scalar operator*=(const Scalar &a){ m_value *= a.m_value; return *this; }
	Scalar operator-(const Scalar &a) const {return Scalar(m_value - a.m_value);}
	Scalar operator-() const {return Scalar(-m_value);}
	Scalar operator-=(const Scalar &a){ m_value -= a.m_value; return *this; }
	Scalar operator/ (const Scalar &a) const {return Scalar(m_value / a.m_value);}
	Scalar operator/=(const Scalar &a){ m_value /= a.m_value; return *this; }
	
	Scalar operator+(const double &a) const {return Scalar(m_value + a);}
	Scalar operator+=(const double &a){ m_value += a; return *this; }
	Scalar operator*(const double &a) const {return Scalar(m_value * a);}
	Scalar operator*=(const double &a){ m_value *= a; return *this; }
	Scalar operator-(const double &a) const {return Scalar(m_value - a);}
	Scalar operator-=(const double &a){ m_value -= a; return *this; }
	Scalar operator/(const double &a) const {return Scalar(m_value / a);}
	Scalar operator/=(const double &a){ m_value /= a; return *this; }
	Scalar operator=(const double &a){ m_value = a; return *this; }
	
	Scalar operator*(const int &a) const {return Scalar(m_value * a);}
	Scalar operator*=(const int &a){ m_value *= a; return *this; }
	Scalar operator=(const bool &a){ m_value = a; m_boolean=true; return *this; }
	
	Scalar operator++() {m_value+=1.0; return *this; }
	Scalar operator--() {m_value-=1.0; return *this; }
	Scalar operator++(int) {m_value+=1.0; return *this; }
	Scalar operator--(int) {m_value-=1.0; return *this; }
	bool operator&&(const Scalar &a) {return (m_value && a.m_value);}
	bool operator||(const Scalar &a) {return (m_value || a.m_value);}
	bool operator!() {return (!m_value); }
	bool operator==(const Scalar &a) const { return (m_value==a.m_value);}
	bool operator!=(const Scalar &a) const {return (m_value!=a.m_value);}
	bool operator>(const Scalar &a) const {return (m_value>a.m_value);}
	bool operator<(const Scalar &a) const {return (m_value<a.m_value);}
	bool operator>=(const Scalar &a) const {return (m_value>=a.m_value);}
	bool operator<=(const Scalar &a) const {return (m_value<=a.m_value);}
	Scalar divide(const Scalar &a)const { return ((long) m_value% (long) a.m_value); }
	Scalar sin(){ return Scalar(std::sin(m_value)); }
	Scalar cos(){ return Scalar(std::cos(m_value)); }
	Scalar tan(){ return Scalar(std::tan(m_value)); }
	Scalar sec(){ return Scalar(1.0/std::cos(m_value)); }
	Scalar csc(){ return Scalar(1.0/std::sin(m_value)); }
	Scalar cot(){ return Scalar(1.0/std::tan(m_value)); }
	Scalar sinh(){ return Scalar(std::sinh(m_value)); }
	Scalar cosh(){ return Scalar(std::cosh(m_value)); }
	Scalar tanh(){ return Scalar(std::tanh(m_value)); }
	Scalar sech(){ return Scalar(1.0/std::cosh(m_value)); }
	Scalar csch(){ return Scalar(1.0/std::sinh(m_value)); }
	Scalar coth(){ return Scalar(1.0/std::tanh(m_value)); }
	Scalar asin(){ return Scalar(std::asin(m_value)); }
	Scalar acos(){ return Scalar(std::acos(m_value)); }
	Scalar atan(){ return Scalar(std::atan(m_value)); }
	Scalar arccot(){ return Scalar(std::log(m_value+std::sqrt(m_value*m_value+1.0))); }
	Scalar arccoth(){ return Scalar(0.5*(std::log(1.0+1.0/m_value)-std::log(1.0-1.0/m_value))); }
	Scalar arccosh(){ return Scalar(std::log(m_value+std::sqrt(m_value-1.0)*std::sqrt(m_value+1.0))); }
	Scalar exp(){ return Scalar(std::exp(m_value)); }
	Scalar log10(){ return Scalar(std::log10(m_value)); }
	Scalar log(){ return Scalar(std::log(m_value)); }
	Scalar floor() { return Scalar(std::floor(m_value)); }
	Scalar ceil(){ return Scalar(std::ceil(m_value)); }
	/*static Scalar gt(const Scalar &a, const Scalar &b) {return Scalar(a > b, true);}
	static Scalar lt(const Scalar &a, const Scalar &b) {return Scalar(a < b, true);}
	static Scalar geq(const Scalar &a, const Scalar &b) {return Scalar(a >= b, true);}
	static Scalar leq(const Scalar &a, const Scalar &b) {return Scalar(a <= b, true);}*/
// 	Scalar b_and() {return Scalar(a && b, true);}
// 	Scalar b_or() {return Scalar(a || b, true);}
// 	Scalar b_xor() {return Scalar((a || b) && !(a&&b), true);}
	Scalar implies(const Scalar &b) {return Scalar(m_value && !b.m_value, true);}
	Scalar sqrt() { return Scalar(std::sqrt(m_value)); }
	Scalar root(double exp){ return Scalar((exp==2.0)? std::sqrt(m_value) : std::pow(m_value, 1.0/exp)); }
	Scalar root(Scalar exp){ return Scalar((exp.m_value==2.0)? std::sqrt(m_value) : std::pow(m_value, 1.0/exp.m_value)); }
	Scalar pow(double exp){ return Scalar(std::pow(m_value, exp)); }
	Scalar pow(Scalar exp){ return Scalar(std::pow(m_value, exp.m_value)); }
	Scalar abs(){ return Scalar(m_value>=0? m_value : -m_value); }
	Scalar derive(){ return Scalar(.0); }
	Scalar factorial() {
		long a,b = m_value;
		for(a=1; b>1; b--)
			a*=b;
		return Scalar(a);
	}
	Scalar gcd(const Scalar &a) {//code by miquel canes :)
// 		long residu=0; long ia = (long) m_value, ib=(long)a.m_value; En quarentena
// 		long residu=0; long ia = 23, ib=32;
	/*	while (ia > 0) {
			residu = ia % ib;
			ia = ib;
			ib = residu;
			qDebug(QString(">>%1--%2--r:%3").arg(ia).arg(ib).arg(residu).ascii());
		}
		qDebug("aaaaaaa");
		return a;*/
		return Scalar(1.0);
	}
	Scalar lcm(const Scalar &a){ //code by miquel canes :)
		/*long residu; long ia = (long) m_value, ib=(long)a.m_value; En quarentena
		long ic=ia*ib;
		while (ib > 0) {
			residu = (int) ia % (int) ib;
			ia = ib;
			ib = residu;
		}
		return Scalar(ic/ia);*/
		return Scalar(a);
	}
};

#endif
