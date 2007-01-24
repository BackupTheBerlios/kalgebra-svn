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

#ifndef ANALITZA_H
#define ANALITZA_H

#include <cmath>

#include "container.h"
#include "value.h"
#include "variables.h"

/**
	@author Aleix Pol <aleixpol@gmail.com>
*/

using namespace std;

class Analitza
{
public:
	Analitza();
	Analitza(const Analitza& a);
	~Analitza();
	
	bool setTextMML(const QString&);
	Object* branch(const QDomElement& e);
	static enum Object::ObjectType whatType(const QString& tag);
	static bool isMathML(const QString& s) { return !s.isEmpty() && s[0]=='<'; }
	QString operToString(const Operator&) const;
	
	void setVariables(Variables* v) { if(m_vars!=NULL) delete m_vars; m_vars = v; }
	void reduce(enum Object::OperatorType op, Cn *ret, Cn oper, bool unary);
	
	Cn calc(Object* e);
	void simpScalar(Container* c);
	void simpPolynomials(Container* c);
	Cn calculate();
	Cn operate(Container*);
	bool isCorrect() const { return m_err.isEmpty(); }
	
	Cn uplimit(const Container& c) const;
	Cn downlimit(const Container& c) const;
	Cn sum(const Container& c);
	Cn product(const Container& c);
	Cn func(const Container& c);
	bool isFunction(Ci var) const;
	
	QStringList bvarList() const;
	QString toString() const;
	QString toMathML() const;
	static bool equalTree(Object const* o1, Object const * o2);
	static Object* objectCopy(Object const*);
// 	QString str(Container*) const;
	
	void simplify();
	Object* simp(Object* root);
	static bool hasVars(Object*);
	Container* tree() const { return (Container*) m_tree; }
	
// private:
	Object *m_tree;
	Variables *m_vars;
	QStringList m_err;
};

#endif
