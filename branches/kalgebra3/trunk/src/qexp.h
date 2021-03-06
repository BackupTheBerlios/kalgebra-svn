/***************************************************************************
 *   Copyright (C) 2005 by Aleix Pol i Gonzàlez                            *
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

#ifndef QEXP_H
#define QEXP_H

#include <stdio.h>
#include <stdlib.h>
#include <qstring.h>
#include <qvaluestack.h>
#include <iostream>
#include <klocale.h>

#include "analitza.h"

/**
	@author Aleix Pol i Gonzalez
*/

typedef enum {
	S,
	R,
	A,
	E1,
	E2,
	E3,
	E,
	K
} actEnum;

typedef enum {
	tAssig, 	//:=
	tLambda,	//->
	tLimits,	//..
	tAdd,		//+
	tSub,		//-
	tMul,		//*
	tDiv,		// /
	tPow,		//^
	tUmi,		//-a
	tFunc,		//f(x)
	tComa,		//,
	tLpr,		//(
	tRpr,		//)
	tEof,
	tMaxOp,
	tVal,
	tVar
} tokEnum;

struct TOKEN {
	QString val;
	tokEnum tipus;
};

// void printPilaOpr(QValueStack<int> opr);
// QString opr2str(int);

class QExp{
public:
	QExp(QString);
	~QExp();
	
	int parse();
	QString error();
	QString mathML();
	
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
	
	TOKEN pillatoken(QString &a);
	int getTok();
	int shift();
	int reduce();
};

#endif
