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

#include "analitza.h"

Analitza::Analitza() : m_tree(NULL), m_vars(new Variables) { }

Analitza::~Analitza()
{
	if(m_tree!=NULL)
		delete m_tree;
	
	if(m_vars!=NULL)
		delete m_vars;
}

bool Analitza::setTextMML(const QString& s)
{
	m_err.clear();
	
	if(m_tree!=NULL)
		delete m_tree;
	
	QDomDocument doc;

	if (!doc.setContent(s)) {
		m_err << i18n("Error while parsing: %1").arg(s);
		return false;
	}
	
	m_tree = branch(doc.documentElement());
	return true;

}

enum Object::ObjectType Analitza::whatType(const QString& tag)
{
	Object::ObjectType ret=Object::none;
	if(tag=="cn")
		ret= Object::value;
	else if(tag=="ci")
		ret= Object::variable;
	else if(Operator::toOperatorType(tag)!=0)
		ret= Object::oper;
	else if(Container::toContainerType(tag)!=0)
		ret= Object::container;
	
	return ret;
}

Object* Analitza::branch(const QDomElement& elem)
{
	Container *c; Cn *num; Operator *op;
	QDomNode n;
	Object* ret=NULL;
	
	switch(whatType(elem.tagName())) {
		case Object::container: {
			c=new Container(Container::toContainerType(elem.tagName()));
			
			n = elem.firstChild();
			while(!n.isNull()) {
				if(n.isElement())
					c->appendBranch(branch(n.toElement()));
				
				n = n.nextSibling();
			}
			
			//Error collection
			QList<Object*>::iterator i=c->m_params.begin();
			Cn u=uplimit(*c), d=downlimit(*c);
			bool dGreaterU = (u.isCorrect() && d.isCorrect()) && d.value()>u.value();
			if(dGreaterU)
				m_err << "The downlimit is greater than the uplimit. Probably should be "+ QString("%1..%2").arg(u.value()).arg(d.value());
			//EOCollect
			ret = c;
			break;}
		case Object::value:
			num= new Cn(0.);
			num->setValue(elem);
			ret = num;
			break;
		case Object::oper:
			op= new Operator(Operator::toOperatorType(elem.tagName()));
			ret = op;
			break;
		case Object::variable:
			{
				Ci* var = new Ci(elem.text());
				var->setFunction(elem.attribute("type")=="function");
				ret=var;
			}
			break;
		case Object::none:
			qDebug() << "Not supported: " << elem.tagName();
	}
	return ret;
}

Cn Analitza::calculate()
{
// 	qDebug() << "calculate:" << m_tree;
	if(m_tree!=NULL)
		return calc(m_tree);
	else {
		m_err << i18n("Must specify an operation");
		return Cn(0.);
	}
}

Cn Analitza::calc(Object* root)
{
	Q_ASSERT(root!=NULL && root->type()!=Object::none);
	Cn ret=Cn(0.);
	Ci *a;
	
	switch(root->type()) {
		case Object::container:	
			ret = operate((Container*) root);
			break;
		case Object::value:
			ret=(Cn*) root;
			break;
		case Object::variable:
			a=(Ci*) root;
			
			if(m_vars->contains(a->name()))
				ret = calc(m_vars->value(a->name()));
			else if(a->isFunction())
				m_err << i18n("The function <em>%1</em> doesn't exist").arg(a->name());
			else
				m_err << i18n("The variable <em>%1</em> doesn't exist").arg(a->name());
			
			break;
		case Object::oper:
		default:
			break;
	}
	return ret;
}

Cn Analitza::operate(Container* c)
{
	Operator *op=NULL;
	Cn ret(0.);
	QList<Cn> numbers;
	
	if(c->containerType() > 100)
		qDebug() << "wow";
	
	if(c->m_params.count()==0) {
		m_err << i18n("Empty container: %1").arg(c->containerType());
		return Cn(0.);
	}
	
	if(c->m_params[0]->type() == Object::oper)
		op = (Operator*) c->m_params[0];
	
	if(op!= NULL && op->operatorType()==Object::sum)
		ret = sum(*c);
	else if(op!= NULL && op->operatorType()==Object::product)
		ret = product(*c);
	else switch(c->containerType()) {
		case Object::apply:
		case Object::math:
		case Object::bvar:
		case Object::uplimit:
		case Object::downlimit:
		{
			if(c->m_params[0]->type() == Object::variable) {
				Ci* var= (Ci*) c->m_params[0];
				
				if(var->isFunction())
					ret = func(c);
				else
					ret = calc(c->m_params[0]);
			} else {
				QList<Object*>::iterator it = c->m_params.begin();
				for(; it!=c->m_params.end(); it++) {
					if((*it)==NULL) {
						m_err << i18n("Null Object found");
						ret.setCorrect(false);
						return ret;
					} else if((*it)->type() != Object::oper) {
						numbers.append(calc(*it));
					}
				}
				
				if(op==NULL) {
					ret = numbers.first();
				} else if(op->nparams()>-1 && numbers.count()!=op->nparams() && op->operatorType()!=Object::minus) {
					m_err << i18n("Too much operators for <em>%1</em>").arg(op->operatorType());
					ret = Cn(0.);
				} else if(numbers.count()>=1 && op->type()==Object::oper) {
					if(numbers.count()>=2) {
						QList<Cn>::iterator it = numbers.begin();
						ret = *it;
						
						++it;
						for(; it != numbers.end(); ++it)
							reduce(op->operatorType(), &ret, *it, false);
						
					} else {
						ret=numbers.first();
						reduce(op->operatorType(), &ret, 0., true);
					}
				} else {
					ret = numbers.first();
				}
			}
		}
			break;
		case Object::declare:
		{
			if(c->m_params.count()<=1) {
				m_err << i18n("Need a var name and a value");
				return Cn(0.);
			}
			
			Ci *var = (Ci*) c->m_params[0];
			
			switch(c->m_params[1]->type()) {
				case Object::variable:
					m_vars->modify(var->name(), new Ci(c->m_params[1]));
					break;
				case Object::value:
					m_vars->modify(var->name(), new Cn(c->m_params[1]));
					break;
				case Object::oper:
					m_vars->modify(var->name(), new Operator(c->m_params[1]));
					break;
				case Object::container:
					m_vars->modify(var->name(), new Container(c->m_params[1]));
					break;
				case Object::none:
					m_err << i18n("Unvalid var type");
					break;
			}
		} break;
		case Object::lambda:
			ret = calc(c->m_params[c->m_params.count()-1]);
			break;
		case Object::cnone:
			break;
	}
	return ret;
}

Cn Analitza::sum(const Container& n)
{
	Cn ret(.0), *c;
	QString var= n.bvarList()[0];
	double ul= uplimit(n).value();
	double dl= downlimit(n).value();
	
	bool existed=m_vars->contains(var);
	m_vars->rename(var, var+"_"); //We save the var value
	m_vars->modify(var, new Cn(0.));
	c = (Cn*) m_vars->value(var);
	
	for(double a = dl; a<=ul; a++){
		*c = a;
		reduce(Object::plus, &ret, calc(n.m_params[4]), false);
	}
	if(existed)
		m_vars->rename(var+"_", var); //We restore the var value
	else
		m_vars->destroy(var);
	
	return ret;
}

Cn Analitza::product(const Container& n)
{
	Cn ret(1.), *c;
	QString var= n.bvarList()[0];
	double ul= uplimit(n).value();
	double dl= downlimit(n).value();
	
	bool existed=m_vars->contains(var);
	m_vars->rename(var, var+"_"); //We save the var value
	m_vars->modify(var, new Cn(0.));
	c = (Cn*) m_vars->value(var);
	
	for(double a = dl; a<=ul; a++){
		*c = a;
		reduce(Object::times, &ret, calc(n.m_params[4]), false);
	}
	if(existed)
		m_vars->rename(var+"_", var); //We restore the var value
	else
		m_vars->destroy(var);
	
	return ret;
}

Cn Analitza::uplimit(const Container& c) const
{
	for(QList<Object*>::const_iterator it=c.m_params.begin(); it!=c.m_params.end(); ++it) {
		Container *c = (Container*) (*it);
		if(c->type()==Object::container && c->containerType()==Object::uplimit && c->m_params[0]->type()==Object::value)
			return Cn(c->m_params[0]);
	}
	Cn r=Cn(0.);
	r.setCorrect(false);
	return r;
}

Cn Analitza::downlimit(const Container& c) const
{
	for(QList<Object*>::const_iterator it=c.m_params.begin(); it!=c.m_params.end(); ++it) {
		Container *c = (Container*) (*it);
		if(c->type()==Object::container && c->containerType()==Object::downlimit && c->m_params[0]->type()==Object::value)
			return Cn(c->m_params[0]);
	}
	Cn r=Cn(0.);
	r.setCorrect(false);
	return r;
}

bool Analitza::isFunction(Ci func) const
{
	if(!m_vars->contains(func.name()))
		return false;
	
	Container *c = (Container*) m_vars->value(func.name());
	if(c->type()==Object::container && c->containerType() == Object::lambda)
		return true;
	else
		return false;
}

Cn Analitza::func(const Container& n)
{
	Cn ret(.0);
	Ci funct(n.m_params[0]);
	
	if(funct.type()!=Object::variable || !funct.isFunction() || !m_vars->contains(funct.name())) {
		m_err << i18n("The function <em>%1</em> doesn't exist").arg(funct.name());
		return ret;
	}
	
	if(!isFunction(funct)) {
		m_err << i18n("<em>%1</em> is not a function").arg(funct.name());
		return ret;
	}
	
	Container *function = (Container*) m_vars->value(funct.name());
	
	QStringList var = function->bvarList();
	
	for(int i=0; i<var.count(); i++) {
		m_vars->rename(var[i], var[i]+"_"); //We save the var value
		m_vars->modify(var[i], n.m_params[i+1]);
	}
	
	ret=calc(function->m_params[var.count()]);
	
	for(int i=0; i<var.count(); i++) {
		m_vars->remove(var[i]);
		m_vars->rename(var[i]+"_", var[i]); //We save the var value
	}
	
	return ret;
}

void Analitza::reduce(enum Object::OperatorType op, Cn *ret, Cn oper, bool unary)
{
	int residu;
	double a=ret->value(), b=oper.value(), c;
	bool boolean=false;
	
	switch(op) {
		case Object::plus:
			a += b;
			break;
		case Object::times:
			a *= b;
			break;
		case Object::divide:
			a /=b;
			break;
		case Object::minus:
			a = unary ? -a : a-b;
			break;
		case Object::power:
			a = pow(a, b);
			break;
		case Object::rem:
			if(floor(b)!=0.)
				a = static_cast<int>(floor(a)) % static_cast<int>(floor(b));
			else
				a=0.;//FIXME
			break;
		case Object::quotient:
			a = floor(a / b);
			break;
		case Object::factorof:
			if(floor(b)!=0.)
				a = (((int)a % (int)b)==0) ? 1.0 : 0.0;
			else
				a = 0.; //FIXME
			boolean = true;
			break;
		case Object::factorial:
			b = floor(a);
			for(a=1.; b>1.; b--)
				a*=b;
			break;
		case Object::sin:
			a=sin(a);
			break;
		case Object::cos:
			a=cos(a);
			break;
		case Object::tan:
			a=tan(a);
			break;
		case Object::sec:
			a=1./cos(a);
			break;
		case Object::csc:
			a=1./sin(a);
			break;
		case Object::cot:
			a=1./tan(a);
			break;
		case Object::sinh:
			a=sinh(a);
			break;
		case Object::cosh:
			a=cosh(a);
			break;
		case Object::tanh:
			a=tanh(a);
			break;
		case Object::sech:
			a=1.0/cosh(a);
			break;
		case Object::csch:
			a=1.0/sinh(a);
			break;
		case Object::coth:
			a=cosh(a)/sinh(a);
			break;
		case Object::arcsin:
			a=asin(a);
			break;
		case Object::arccos:
			a=acos(a);
			break;
		case Object::arctan:
			a=acos(a);
			break;
		case Object::arccot:
			a=log(a+pow(a*a+1., 0.5));
			break;
		case Object::arcsinh:
			a=0.5*(log(1.+1./a)-log(1.-1./a));
			break;
		case Object::arccosh:
			a=log(a+sqrt(a-1.)*sqrt(a+1.));
			break;
	// 	case Object::arccsc:
	// 	case Object::arccsch:
	// 	case Object::arcsec:
	// 	case Object::arcsech:
	// 	case Object::arcsinh:
	// 	case Object::arctanh:
		case Object::exp:
			a=exp(a);
			break;
		case Object::ln:
			a=log(a);
			break;
		case Object::log:
			a=log10(a);
			break;
		case Object::abs:
			a= a>=0. ? a : -a;
			break;
		//case Object::conjugate:
		//case Object::arg:
		//case Object::real:
		//case Object::imaginary:
		case Object::floor:
			a=floor(a);
			break;
		case Object::ceiling:
			a=ceil(a);
			break;
		case Object::min:
			a= a < b? a : b;
			break;
		case Object::max:
			a= a > b? a : b;
			break;
		case Object::gt:
			a= a > b? 1.0 : 0.0;
			boolean=true;
			break;
		case Object::lt:
			a= a < b? 1.0 : 0.0;
			boolean=true;
			break;
		case Object::eq:
			a= a == b? 1.0 : 0.0;
			boolean=true;
			break;
		case Object::approx:
			a= abs(a-b)<0.001? 1.0 : 0.0;
			boolean=true;
			break;
		case Object::neq:
			a= a != b? 1.0 : 0.0;
			boolean=true;
			break;
		case Object::geq:
			a= a >= b? 1.0 : 0.0;
			boolean=true;
			break;
		case Object::leq:
			a= a <= b? 1.0 : 0.0;
			boolean=true;
			break;
		case Object::_and:
			a= a && b? 1.0 : 0.0;
			boolean=true;
			break;
		case Object::_not:
			a=!a;
			boolean = true;
			break;
		case Object::_or:
			a= a || b? 1.0 : 0.0;
			boolean = true;
			break;
		case Object::_xor:
			a= (a || b) && !(a&&b)? 1.0 : 0.0;
			boolean = true;
			break;
		case Object::implies:
			a= (a && !b)? 0.0 : 1.0;
			boolean = true;
			break;
		case Object::gcd: //code by michael cane aka kiko :)
			while (b > 0.) {
				residu = (int) floor(a) % (int) floor(b);
				a = b;
				b = residu;
			}
			break;
		case Object::lcm: //code by michael cane aka kiko :)
			c=a*b;
			while (b > 0.) {
				residu = (int) floor(a) % (int) floor(b);
				a = b;
				b = residu;
			}
			a=(int)c/(int)a;
			break;
		case Object::root:
			a = b==2.0 ? sqrt(a) : pow(a, 1.0/b);
			break;
			
		default:
			m_err << i18n("The operator <em>%1</em> hasn't been implemented").arg(op);
			break;
	}
// 	
	ret->setValue(a);
}

QStringList Analitza::bvarList() const //FIXME: if
{
	Container *c = (Container*) m_tree;
	if(m_tree != NULL && c->type()==Object::container) {
		c = (Container*) c->m_params[0];
		
		if(c->type()==Object::container)
			return c->bvarList();
	}
	return QStringList();
	
}

QString Analitza::toMathML() const
{
	if(m_tree==NULL)
		return QString::null;
	else
		return m_tree->toMathML();
}

QString Analitza::toString() const
{
	if(m_tree==NULL)
		return QString::null;
	else
		return m_tree->toString();
}


/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////

void Analitza::simplify()
{
	if(m_tree!=NULL)
		m_tree = simp(m_tree);
//	objectWalker(m_tree, 0);
// 	if(m_tree!=NULL)
// 		return simp(m_tree);
// 	else {
// 		m_err << i18n("Must specify an operation");
// 		return Cn(0.);
// 	}
}

Object* Analitza::simp(Object* root)
{
	Q_ASSERT(root!=NULL && root->type()!=Object::none);
	Object* aux=NULL;
	if(!hasVars(root)) {
		if(root->type() !=Object::oper) {
			aux = root;
			root = new Cn(calc(root));
			delete aux;
		}
		return root;
	} else if(root->type()==Object::container) {
		Container *c= (Container*) root;
		QList<Object*>::iterator it = c->m_params.begin();
		for(; it!=c->m_params.end(); it++)
			*it = simp(*it);
	}
	return root;
}

bool Analitza::hasVars(Object* o)
{
	Q_ASSERT(o!=NULL);
	bool r=false;
	switch(o->type()) {
		case Object::variable:
			r=true;
			break;
		case Object::container: {
			Container *c = (Container*) o;
			QList<Object*>::iterator it = c->m_params.begin();
			
			for(; it!=c->m_params.end(); it++)
				r |= hasVars(*it);
			
		} break;
		case Object::none:
		case Object::value:
		case Object::oper:
			r=false;
	}
	return r;
}

