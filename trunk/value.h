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
#ifndef VALUE_H
#define VALUE_H

#include <object.h>
#include <QtXml>

/**
	@author Aleix Pol <aleixpol@gmail.com>
*/

class Cn : public Object
{
	public:
		enum ValueFormat { none, nan, real, integer, boolean };
		Cn() : Object(Object::value), m_value(0.), m_boolean(false) {}
		Cn(const Cn& v) : Object(Object::value), m_value(v.value()), m_boolean(v.isBoolean()) { setCorrect(v.isCorrect()); }
		Cn(double b=0.) : Object(Object::value), m_value(b), m_boolean(false) {}
		Cn(Object *o);
		virtual ~Cn() {}
		void setValue(const QDomElement& e);
		void setValue(double v) { m_value=v; }
		double value() const { return m_value; }
		bool isBoolean() const { return m_boolean; }
		static double toNum(const QString& num, const QString& type, int base);
		static enum ValueFormat whatValueFormat(const QDomElement&);
		
		bool operator<(const Cn& d) const { return m_value<d.value(); }
		bool operator<(double d) const { return m_value<d; }
		bool operator<=(const Cn& d) const { return m_value<=d.value(); }
		bool operator<=(double d) const { return m_value<=d; }
		Cn operator=(double d) { m_value=d; return *this; }
		
		Cn operator++() { m_value++; return this; }
		Cn operator++(int) { m_value++; return this; }
		
		QString toString() const;
	private:
		double m_value;
		bool m_boolean;
		enum ValueFormat m_vformat;
};

#endif
