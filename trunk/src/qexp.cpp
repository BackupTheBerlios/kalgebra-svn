/***************************************************************************
 *   Copyright (C) 2005 by aleix                                           *
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
#include "qexp.h"

using namespace std;

// QString i18n(QString a){return a;}

const actEnum parseTbl[tMaxOp][tMaxOp] = {
//	 :=   ->  +   -   *   /   ^   M   f   ,   (   )   $
	{ R,  S,  S,  S,  S,  S,  S,  S,  S,  R,  S,  R,  R },	//:=
	{ R,  S,  S,  S,  S,  S,  S,  S,  S,  R,  S,  R,  R },	//-> Lambda
	{ R,  R,  K,  R,  S,  S,  S,  S,  S,  R,  S,  R,  R },	//+
	{ R,  R,  R,  K,  S,  S,  S,  S,  S,  R,  S,  R,  R },	//-
	{ R,  R,  R,  R,  K,  R,  S,  S,  S,  R,  S,  R,  R },	//*
	{ R,  R,  R,  R,  R,  R,  S,  S,  S,  R,  S,  R,  R },	///
	{ R,  R,  R,  R,  R,  R,  R,  S,  S,  R,  S,  R,  R },	//^
	{ R,  R,  R,  R,  R,  R,  S,  S,  S,  R,  S,  R,  R },	//UnaryMinus
	{ R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  S,  R,  R },	//func
	{ S,  S,  S,  S,  S,  S,  S,  S,  S,  R,  R,  R,  E },	//,
	{ S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  S, E1 },	//(
	{ R,  R,  R,  R,  R,  R,  R,  R, E3,  R, E2,  R,  R },	//)
	{ S,  S,  S,  S,  S,  S,  S,  S,  S,  E,  S, E3,  A },	//$
};

QExp::QExp(QString exp){
	str = exp;
}

QExp::~QExp(){}

TOKEN QExp::pillatoken(QString &a){
	int i=0;
	a = a.stripWhiteSpace();
// 	printf("%s\n", a.ascii());
	TOKEN ret;
	ret.tipus = tMaxOp;
	if(a=="")
		ret.tipus = tEof;
	else if(a[0].isDigit() || (a[0]=='.' && a[1].isDigit())) {//es un numero
		int coma=0;
		if(a[0]=='.') {
			ret.val += '0';
			coma++;
		}
		ret.val += a[0];
		a[0]=' ';
		for(i=1; a[i].isDigit() || a[i]=='.'; i++){
			coma = (a[i]=='.')? coma+1 : coma;
			ret.val += a[i];
			a[i]=' ';
		}
		if(a[i] == '(' || a[i].isLetter())
			a = " *" +a;
		
		if(coma>1){
			err += i18n("Too much comma in %1<br />\n").arg(ret.val);
		}
		
		ret.val = QString::QString("<cn>%1</cn>").arg(ret.val);
		ret.tipus= tVal;
	} else if(a[0].isLetter()) {//es una variable o func
		ret.val += a[0];
		for(i=1; a[i].isLetter(); i++){
			ret.val += a[i]; a[i]=' ';
		}
		if(a[i]=='('){
			ret.tipus=tFunc;
		} else {
			ret.val = QString::QString("<ci>%1</ci>").arg(ret.val);
			ret.tipus= tVal;
		}
	} else if(a[0]=='-' && a[1] == '>') {
		ret.tipus = tLambda;
		a[1] =' ';
	} else if(a[0]==':' && a[1] == '=') {
		ret.tipus = tAssig;
		a[1] =' ';
	} else if(a[0]=='+')
		ret.tipus = tAdd;
	else if(a[0]=='-')
		ret.tipus = (antnum == tVal || antnum==tRpr) ? tSub : tUmi;
	else if(a[0]=='/')
		ret.tipus = tDiv;
	else if(a[0]=='^')
		ret.tipus = tPow;
	else if(a[0]=='*' && a[1] == '*') {
		ret.tipus = tPow;
		a[1] =' ';
	} else if(a[0]=='*')
		ret.tipus = tMul;
	else if(a[0]=='(')
		ret.tipus = tLpr;
	else if(a[0]==')')
		ret.tipus = tRpr;
	else if(a[0]==',')
		ret.tipus = tComa;
	else
		err += i18n("Unknown token %1<br />\n").arg(a[0]);
	
	a[0]=' ';
	antnum = ret.tipus;
	return ret;
}

int QExp::getTok(){
	QString s;
	TOKEN t;
	if(firsttok){
		firsttok=false;
		prevtok=tEof;
	}
	t=pillatoken(str);
	tok = t.tipus;
	tokval = t.val;
	
	prevtok=tok;
	return 0;
}

int QExp::shift(){
// 	cout << "------>" << tokval.ascii() << "'" << endl;
	if(tok==tVal){
		val.push(tokval);
	} else if(tok==tFunc){
		func.push(tokval);
		opr.push((char)tok);
	} else
		opr.push((char)tok);
	if(getTok())
		return 1;
	return 0;
}

int QExp::reduce(){
	tokEnum oper = (tokEnum) opr.top();
	opr.pop();
	
	if(oper == tAdd){
		val.push(QString("<apply><plus />%1%2</apply>").arg(val.pop()).arg(val.pop()));
	} else if(oper == tSub){
		val.push(QString("<apply><minus />%1%2</apply>").arg(val.pop()).arg(val.pop()));
	} else if(oper == tUmi){
		val.push(QString("<apply><minus />%1</apply>").arg(val.pop()));
	} else if(oper == tMul){
		val.push(QString("<apply><times />%1%2</apply>").arg(val.pop()).arg(val.pop()));
	} else if(oper == tDiv){
		val.push(QString("<apply><quotient />%1%2</apply>").arg(val.pop()).arg(val.pop()));
	} else if(oper == tPow){
		val.push(QString("<apply><power />%1%2</apply>").arg(val.pop()).arg(val.pop()));
	} else if(oper == tAssig){ // :=
		val.push(QString("<declare>%1%2</declare>").arg(val.pop()).arg(val.pop()));
	} else if(oper == tFunc){
		if(Analitza::isOperador(func.top()))
			val.push(QString("<apply><%1 />%2</apply>").arg(func.pop()).arg(val.pop()));
		else
			val.push(QString("<apply><ci type='function'>%1</ci>%2</apply>").arg(func.pop()).arg(val.pop()));
	} else if(oper == tLambda){ // ->
		val.push(QString("<lambda><bvar>%1</bvar>%2</lambda>").arg(val.pop()).arg(val.pop()));
	} else if(oper == tComa){
		val.push(QString("%1%2").arg(val.pop()).arg(val.pop()));
	} else if(oper == tRpr){
		opr.pop();
	}
	return 0;
}

int QExp::parse(){
	opr.push(tEof);
	firsttok = true;
	antnum= tEof;
	
	if(getTok()) return 1;
	while(err==""){
		if(tok==tVal){
			if(shift()) return 1;
			continue;
		}
// 		printf("acc=%d stk=%d, tok=%d\n", parseTbl[opr.top()][tok], opr.top(), tok);
		switch(parseTbl[opr.top()][tok]){
			case K:
				val.push(QString("%1%2").arg(val.pop()).arg(val.pop()));
				opr.pop();
				break;
			case R:
				if(reduce()) return 1;
				break;
			case S:
				if(shift()) return 1;
				break;
			case A:
				mml = "<math>" + val.pop() + "</math>";
// 				cout << val.top().ascii() << endl;
				return 0;
			case E1:
				err += i18n("Missing right paranthesis<br />\n");
				return 1;
			case E2:
				err += i18n("Missing operator< p/>\n");
				return 2;
			case E3:
				err += i18n("Unbalanced right parenthesis<br />\n");
				return 3;
			case E:
				err += i18n("Error<br />\n");
				return 4;
			default:
// 				cerr << "?? (" << (int) opr.top() << ", " << (int) tok << ") = "  << (int) parseTbl[opr.top()][tok] << endl;
				break;
		}
	}
	return -1;
}

QString QExp::mathML(){
	return mml;
}

QString QExp::error(){
	return err;
}
