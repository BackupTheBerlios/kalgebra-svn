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

#include "container.h"
#include "value.h"

Container::Container(const Container& c) : Object(Object::container)
{
	Q_ASSERT(c.type()==Object::container);
	if(c.type()!=Object::container) {
		setType(none);
		return;
	}
	
	m_params = c.copyParams();
}

Container::Container(const Object *o) : Object(o->type())
{
	Q_ASSERT(o!=NULL);
	if(type() == Object::container) {
		Container *c = (Container*) o;
		m_cont_type = c->containerType();
		
		m_params = c->copyParams();
	} else {
		setType(Object::none);
		m_cont_type = cnone;
	}
}

Operator Container::firstOperator() const
{
	Operator ret(Object::onone);
	bool found=false;
	for(int i=0; i<m_params.count() && !found; i++) {
		if(m_params[i]->type()==Object::oper) {
			ret = m_params[i];
			found = true;
		}
	}
	
	return ret;
}

QString Container::toMathML() const
{
	QString ret;
	QList<Object*>::const_iterator i;
	for(i=m_params.constBegin(); i!=m_params.constEnd(); ++i) {
		ret += (*i)->toMathML();
	}
	
	QString tag;
	switch(m_cont_type) {
		case Object::declare:
			tag="declare";
			break;
		case Object::lambda:
			tag="lambda";
			break;
		case Object::math:
			tag="math";
			break;
		case Object::apply:
			tag="apply";
			break;
		case Object::uplimit:
			tag="uplimit";
			break;
		case Object::downlimit:
			tag="downlimit";
			break;
		case Object::bvar:
			tag="bvar";
			break;
		default:
			tag="dunno";
			break;
	}
	return QString("<%1>%2</%1>").arg(tag).arg(ret);
}

QString Container::toString() const
{
	QStringList ret;
	bool func=false;
	
	Operator *op=NULL;
	for(int i=0; i<m_params.count(); i++) {
		if(m_params[i]==NULL) {
			qDebug() << "kkk";
			return "<<kk>>";
		}
		
		if(m_params[i]->type() == Object::oper)
			op = (Operator*) m_params[i];
		else if(m_params[i]->type() == Object::variable) {
			Ci *b = (Ci*) m_params[i];
			if(b->isFunction())
				func=true;
			ret << b->toString();
		} else if(m_params[i]->type() == Object::container) {
			Container *c = (Container*) m_params[i];
			QString s = c->toString();
			Operator child_op = c->firstOperator();
			if(op!=NULL && op->weight()>child_op.weight() && child_op.nparams()!=1)
				s=QString("(%1)").arg(s);
			
			if(c->containerType() == Object::bvar) {
				Container *ul = ulimit(), *dl = dlimit();
				if(ul!=NULL || dl!=NULL) {
					if(dl!=NULL)
						s += dl->toString();
					s += "..";
					if(ul!=NULL)
						s += ul->toString();
				}
			}
			
			if(c->containerType()!=Object::uplimit && c->containerType()!=Object::downlimit)
				ret << s;
		} else 
			ret << m_params[i]->toString();
	}
	
	QString toret;
	switch(containerType()) {
		case Object::declare:
			toret += ret.join(":=");
			break;
		case Object::lambda:
			toret += ret.join("");
			break;
		case Object::math:
			toret += ret.join("; ");
			break;
		case Object::apply:
			if(func){
				QString n = ret.takeFirst();
				toret += QString("%1(%2)").arg(n).arg(ret.join(", "));
			} else if(op==NULL)
				toret += ret.join(" ");
			else switch(op->operatorType()) {
				case Object::plus:
					toret += ret.join("+");
					break;
				case Object::times:
					toret += ret.join("*");
					break;
				case Object::divide:
					toret += ret.join("/");
					break;
				case Object::minus:
					if(ret.count()==1)
						toret += "-"+ret[0];
					else
						toret += ret.join("-");
					break;
				case Object::power:
					toret += ret.join("^");
					break;
				default:
					if(ret.count() == 1)
						toret += QString("%1 %2").arg(op->toString()).arg(ret.join(", "));
					else
						toret += QString("%1(%2)").arg(op->toString()).arg(ret.join(", "));
					break;
			}
			break;
		case Object::uplimit: //x->(n1..n2) is put at the same time
		case Object::downlimit:
			break;
		case Object::bvar:
			toret += ret.join("->")+"->";
			break;
		default:
			toret += ret.join(" ?? ");
			break;
	}
	return toret;
}

QList<Object*> Container::copyParams() const
{
	QList<Object*> ret;
	
	for(QList<Object*>::const_iterator it=m_params.begin(); it!=m_params.end(); it++) {
		switch((*it)->type()) {
			case Object::variable:
				ret.append(new Ci(*it));
				break;
			case Object::value:
				ret.append(new Cn(*it));
				break;
			case Object::oper:
				ret.append(new Operator(*it));
				break;
			case Object::container:
				ret.append(new Container(*it));
				break;
			case Object::none:
				break;
		}
	}
	return ret;
}

enum Object::ContainerType Container::toContainerType(const QString& tag)
{
	ContainerType ret=cnone;
	
	if(tag=="apply") ret=apply;
	else if(tag=="declare") ret=declare;
	else if(tag=="math") ret=math;
	else if(tag=="lambda") ret=lambda;
	else if(tag=="bvar") ret=bvar;
	else if(tag=="uplimit") ret=uplimit;
	else if(tag=="downlimit") ret=downlimit;
	
	return ret;
}

QStringList Container::bvarList() const //NOTE: Should return Ci's instead of Strings?
{
	QStringList bvars;
	QList<Object*>::const_iterator it;
	
	for(it=m_params.begin(); it!=m_params.end(); ++it) {
		Container* c = (Container*) (*it);
		if(c->containerType() == Object::bvar)
			bvars.append(((Ci*)c->m_params[0])->name());
	}
	
	return bvars;
}

Container* Container::ulimit() const
{
	for(QList<Object*>::const_iterator it=m_params.begin(); it!=m_params.end(); ++it) {
		Container *c = (Container*) (*it);
		if(c->type()==Object::container && c->containerType()==Object::uplimit && c->m_params[0]->type()==Object::value)
			return (Container*) c->m_params[0];
	}
	return NULL;
}

Container* Container::dlimit() const
{
	for(QList<Object*>::const_iterator it=m_params.begin(); it!=m_params.end(); ++it) {
		Container *c = (Container*) (*it);
		if(c->type()==Object::container && c->containerType()==Object::downlimit && c->m_params[0]->type()==Object::value)
			return (Container*) c->m_params[0];
	}
	return NULL;
}

bool Container::hasVars() const
{
	bool ret=false;
	
	if(m_params.isEmpty())
		ret = false;
	else {
		for(QList<Object*>::const_iterator i=m_params.begin(); !ret && i!=m_params.end(); ++i) {
			switch((*i)->type()) {
				case Object::variable:
					ret=true;
					break;
				case Object::container:
				{	
					Container *c = (Container*) (*i);
					ret |= c->hasVars();
					break;
				}
				default:
					ret=false;
			}
		}
	}
	return ret;
}

void objectWalker(const Object* root, int ind)
{
	Q_ASSERT(root!=NULL);
	Container *c; Cn *num; Operator *op; Ci *var;
	QString s;
	if(ind>100) return;
	
	for(int i=0; i<ind; i++)
		s += " |_____";
	
	if(root==NULL) {
		qDebug() << "at " << ind << "got a NULL";
		return;
	}
	
	switch(root->type()) { //TODO: include the function into a module and use toString
		case Object::container:
			c= (Container*) root;
			qDebug() << qPrintable(s) << "| cont: " << c->toMathML();
			for(int i=0; i<c->m_params.count(); i++)
				objectWalker(c->m_params[i], ind+1);
			
			break;
		case Object::value:
			num= (Cn*) root;
			qDebug() << qPrintable(s) << "| num: " << num->value();
			break;
		case Object::oper:
			op= (Operator*) root;
			qDebug() << qPrintable(s) << "| operator: " << op->toString();
			break;
		case Object::variable:
			var = (Ci*) root;
			qDebug() << qPrintable(s) << "| variable: " << var->name() << "Func:" << var->isFunction();
			break;
		default:			qDebug() << qPrintable(s) << "| dunno: " << (int) root->type();
			break;
	}
}

void print_dom(QDomNode in, int ind)
{
	QString a = QString();

	if(ind >100){
		qDebug("...");
		return;
	}

	for(int i=0; i<ind; i++)
		a.append("______|");

	if(in.hasChildNodes())
		qDebug("%s%s(%s) -- %d", qPrintable(a), qPrintable(in.toElement().tagName()), qPrintable(in.toElement().text()), in.childNodes().length());
	else
		qDebug("%s%s", qPrintable(a), qPrintable(in.toElement().tagName()));

	for(unsigned int i=0 ; i<in.childNodes().length(); i++){
		if(in.childNodes().item(i).isElement())
			print_dom(in.childNodes().item(i), ind+1);
	}
}
