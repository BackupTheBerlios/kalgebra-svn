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
	return QString(":)");
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
			if(op!=NULL && op->weight() > child_op.weight() && child_op.nparams()!= 1)
				s="("+s+")";
			
			ret << s;
		} else 
			ret << m_params[i]->toString();
	}
	
	switch(containerType()) {
		case Object::declare:
			return ret.join(":=");
		case Object::lambda:
			return ret.join("");
		case Object::math:
			return ret.join(", ");
		case Object::apply:
			if(func){
				QString n = ret.takeFirst();
				return QString("%1(%2)").arg(n).arg(ret.join(", "));
			} else if(op==NULL)
				return ret.join(" ");
			else switch(op->operatorType()) {
				case Object::plus:
					return ret.join("+");
				case Object::times:
					return ret.join("*");
				case Object::divide:
					return ret.join("/");
				case Object::minus:
					return ret.join("-");
				case Object::power:
					return ret.join("^");
				default:
					break;
			}
			
			return QString("%1(%2)").arg(op->toString()).arg(ret.join(", "));
		case Object::bvar:
			return ret[0]+"->";
		default:
			return ret.join(" ;; ");
	}
	return ret.join("");
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
	
	switch(root->type()) {
		case Object::container:
			c= (Container*) root;
			qDebug() << qPrintable(s) << "| cont: " << (int) c->containerType();
			for(int i=0; i<c->m_params.count(); i++)
				objectWalker(c->m_params[i], ind+1);
			
			break;
		case Object::value:
			num= (Cn*) root;
			qDebug() << qPrintable(s) << "| num: " << num->value();
			break;
		case Object::oper:
			op= (Operator*) root;
			qDebug() << qPrintable(s) << "| operator: " << op->operatorType();
			break;
		case Object::variable:
			var = (Ci*) root;
			qDebug() << qPrintable(s) << "| variable: " << var->name() << "Func:" << var->isFunction();
			break;
		default:
			qDebug() << qPrintable(s) << "| dunno: " << (int) root->type();
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
