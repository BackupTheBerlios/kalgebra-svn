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
#include "variables.h"

Variables::Variables() : QHash<QString, Object*>()
{
	insert("true", new Cn(1.));
	insert("false", new Cn(0.));
	insert("pi", new Cn(3.1415926535897932384626433));
	insert("e", new Cn(2.718281828));
	insert("euler", new Cn(0.5772156649));
}

Variables::~Variables()
{
	QHash<QString, Object*>::iterator i;
	for (i = this->begin(); i != this->end(); i++)
		delete *i;
}


void Variables::modify(const QString& key, Object* o)
{
	if(contains(key))
		delete value(key);
	
	insert(key, o);
}


bool Variables::rename(const QString& orig, const QString& dest)
{
	bool existed = contains(orig);
	Object *aux = NULL;
	
	if(existed) {
		aux = take(orig);
		insert(dest, aux);
	}
	
	return existed;
}

bool Variables::destroy(const QString& s)
{
	delete take(s);
	return true;
}