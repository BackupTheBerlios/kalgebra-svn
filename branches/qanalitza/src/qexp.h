/***************************************************************************
 *   Copyright (C) 2005 by aleix                                           *
 *   aleix@pilgrim                                                         *
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
#ifndef QEXP_H
#define QEXP_H

#include <stdio.h>
#include <stdlib.h>
#include <qstring.h>
#include <qvaluestack.h>
#include <iostream>
//#include <klocale.h>

/**
@author aleix
*/

QString i18n(QString);
typedef enum {
	S,
	R,
	A,
	E1,
	E2,
	E3,
	E,
} actEnum;

typedef enum {
	tAssig,
	tAdd,
	tSub,
	tMul,
	tDiv,
	tPow,
	tUmi,
	tFunc,
	tComa,
	tLpr, //(
	tRpr, //)
	tEof,
	tMaxOp,
	tVal
} tokEnum;

struct TOKEN {
	QString val;
	tokEnum tipus;
};

class QExp{
public:
	QExp(QString);
	~QExp();
	
private:
	QString mml;
	QString str; //Auxiliar pel parsing
	QString err;
	
	tokEnum tok;
	QString tokval;
	
	QValueStack<int> opr;
	QValueStack<QString> val;
	QValueStack<QString> func;
	
	int valTop, oprTop;
	bool firsttok;
	tokEnum prevtok;
	tokEnum antnum;

// 	actEnum parseTbl[tMaxOp][tMaxOp];
	
	TOKEN pillatoken(QString &a);
	int getTok();
	int shift();
	int reduce();
	bool isLast(QValueStack<int> opr);
public:
	int parse();
	QString error();
	QString mathML();
};

#endif
