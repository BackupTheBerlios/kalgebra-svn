/***************************************************************************
 *   Copyright (C) 2006 by Aleix Pol                                       *
 *   aleixpol@gmail.com                                                    *
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

#include "object.h"
#include "container.h"

QString Object::toString() const
{
	QString ret = QString("%1").arg(m_type);
	
	return ret;
}

enum Object::OperatorType Operator::toOperatorType(QString e)
{
	enum OperatorType ret=onone;
	
	if(e=="plus") ret=plus;
	else if(e=="times") ret=times;
	else if(e=="min") ret=min;
	else if(e=="max") ret=max;
	else if(e=="and") ret=_and;
	else if(e=="or" ) ret=_or;
	else if(e=="xor") ret=_xor;
	else if(e=="gcd") ret=gcd;
	else if(e=="lcm") ret=lcm;
	else if(e=="max") ret=max;
	else if(e=="min") ret=min;
	else if(e=="quotient") ret=quotient;
	else if(e=="power") ret=power;
	else if(e=="minus") ret=minus;
	else if(e=="divide" ) ret=divide;
	else if(e=="rem") ret=rem;
	else if(e=="factorof") ret=factorof;
	else if(e=="lt") ret=lt;
	else if(e=="gt") ret=gt;
	else if(e=="eq") ret=eq;
	else if(e=="neq") ret=neq;
	else if(e=="leq") ret=leq;
	else if(e=="geq") ret=geq;
	else if(e=="implies") ret=implies;
	else if(e=="approx") ret=approx;
	else if(e=="root") ret=root;
	else if(e=="factorial") ret=factorial;
	else if(e=="abs") ret=abs;
	else if(e=="sin") ret=sin;
	else if(e=="cos") ret=cos;
	else if(e=="tan") ret=tan;
	else if(e=="sec") ret=sec;
	else if(e=="csc") ret=csc;
	else if(e=="cot") ret=cot;
	else if(e=="sinh") ret=sinh;
	else if(e=="cosh") ret=cosh;
	else if(e=="tanh") ret=tanh;
	else if(e=="sech") ret=sech;
	else if(e=="csch") ret=csch;
	else if(e=="coth") ret=coth;
	else if(e=="arcsin") ret=arcsin;
	else if(e=="arccos") ret=arccos;
	else if(e=="arctan") ret=arctan;
	else if(e=="arccos") ret=arccos;
	else if(e=="arccot") ret=arccot;
	else if(e=="arcoth") ret=arcoth;
	else if(e=="arccosh") ret=arccosh;
	else if(e=="arccsc" ) ret=arccsc;
	else if(e=="arccsch") ret=arccsch;
	else if(e=="arcsec" ) ret=arcsec;
	else if(e=="arcsech") ret=arcsech;
	else if(e=="arcsinh") ret=arcsinh;
	else if(e=="arctanh") ret=arctanh;
	else if(e=="exp") ret=exp;
	else if(e=="ln") ret=ln;
	else if(e=="log") ret=log;
	else if(e=="not") ret=_not;
	else if(e=="factorial") ret=factorial;
	else if(e=="minus") ret=minus;
	else if(e=="abs") ret=abs;
	else if(e=="conjugate") ret=conjugate;
	else if(e=="arg") ret=arg;
	else if(e=="real") ret=real;
	else if(e=="imaginary") ret=imaginary;
	else if(e=="floor") ret=floor;
	else if(e=="ceiling") ret=ceiling;
	else if(e=="sum") ret=sum;
	else if(e=="product") ret=product;
	
	return ret;
}

int Operator::nparams(Operator::OperatorType t)
{
	switch(t) {
		case plus:
		case times:
		case min:
		case max:
		case _and:
		case _or:
		case _xor:
		case gcd:
		case lcm:
		case function:
			return -1;
		case quotient:
		case power:
		case minus:
		case divide:
		case rem:
		case factorof:
		case lt:
		case gt:
		case eq:
		case neq:
		case leq:
		case geq:
		case implies:
		case approx:
		case root:
			return 2;
		case abs:
		case sin:
		case cos:
		case tan:
		case sec:
		case csc:
		case cot:
		case sinh:
		case cosh:
		case tanh:
		case sech:
		case csch:
		case coth:
		case arcsin:
		case arccos:
		case arctan:
		case arccot:
		case arcoth:
		case arccosh:
		case arccsc:
		case arccsch:
		case arcsec:
		case arcsech:
		case arcsinh:
		case arctanh:
		case exp:
		case ln:
		case log:
		case _not:
		case factorial:
		case conjugate:
		case arg:
		case real:
		case imaginary:
		case floor:
		case ceiling:
		case sum:
		case product:
			return 1;
		case none:
			break;
	}
	return 0;
}


Operator::Operator(Object *o) : Object(o->type())
{
	if(type() == Object::oper) {
		Operator *c = (Operator*) o;
		m_optype = c->operatorType();
	} else
		setType(Object::none);
}

unsigned int Operator::operatorWeight(OperatorType op)
{
	switch(op) {
		case lt:
		case gt:
		case eq:
		case neq:
		case leq:
		case geq:
			return 1;
		case sin:
		case cos:
		case tan:
			return 2;
		case minus:
		case plus:
			return 3;
		case times:
		case divide:
			return 4;
		case _and:
		case _or:
		case _xor:
			return 6;
		case power:
			return 5;
		default:
			return 1000;
	}
	return 666;
}

Ci::Ci(Object*o) : Object(o->type())
{
	if(type() == Object::variable) {
		Ci *c = (Ci*) o;
		m_name = c->name();
		m_function = c->isFunction();
	} else
		setType(Object::none);	
}

