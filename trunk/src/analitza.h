/***************************************************************************
 *   Copyright (C) 2005 by ,,,                                             *
 *   aleix@sklar                                                           *
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

#include <iostream>
#include <cmath>
#include <qstring.h>
#include <qlocale.h>
#include <qdom.h>
#include <qfile.h>
#include <qxml.h>
#include <qvaluelist.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include "qexp.h"
#include "variables.h"

/**
@author
*/

using namespace std;

void print_dom(QDomNode, int);

class Analitza{
public:
	
	Analitza(QString path);
	Analitza();
	~Analitza();
	
	QDomNode elem;
	int isOperador(QString);
	int setPath(QString);
	int setTextMML(QString);
	int setText(QString);
	QString textMML();
	
	Variables vars;
	QString err;
	static bool isNum(QString);
	////////////////////////////////////////
	
	double Calcula();
	QStringList lambda(); //retrieve lambda vars
	double toNum(QDomElement res);
	static QString treu_tags(QString in);
private:
	QStringList bvar(QDomNode);
	QDomNode uplimit(QDomNode);
	QDomNode downlimit(QDomNode);
	QDomNode first_val(QDomNode);
	QString mmlexp;
	double evalua(QDomNode n);
	double opera(double, double, QString, int);
	unsigned int toOpId(QDomNode);
	QString get_op(QDomNode);
	////////////////////////////////////////
	
	//////////
	double sum(QDomNode);

public:
	QString toString();
	QString str(QDomNode);
	
private:
	QString escriuS(QString res, QString oper, QString op, int);
	QString escriuMMLP(QString res, QString oper, QString op, int);
};

#endif
