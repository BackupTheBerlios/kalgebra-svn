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

QString i18n(QString a){return a;}

const actEnum parseTbl[tMaxOp][tMaxOp] = {
//	 :=   +   -   *   /   ^   M   f   ,   (   )   $
	{ R,  S,  S,  S,  S,  S,  S,  S,  R,  S,  R,  R },	//:=
	{ R,  R,  R,  S,  S,  S,  S,  S,  R,  S,  R,  R },	//+
	{ R,  R,  R,  S,  S,  S,  S,  S,  R,  S,  R,  R },	//-
	{ R,  R,  R,  R,  R,  S,  S,  S,  R,  S,  R,  R },	//*
	{ R,  R,  R,  R,  R,  S,  S,  S,  R,  S,  R,  R },	///
	{ R,  R,  R,  R,  R,  S,  S,  S,  R,  S,  R,  R },	//^
	{ R,  R,  R,  R,  R,  S,  S,  S,  R,  S,  R,  R },	//UnaryMinus
	{ R,  R,  R,  R,  R,  R,  R,  R,  R,  S,  R,  R },	//func
	{ S,  S,  S,  S,  S,  S,  S,  S,  S,  R,  R,  E },	//,
	{ S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  S, E1 },	//(
	{ R,  R,  R,  R,  R,  R,  R, E3,  R, E2,  R,  R },	//)
	{ S,  S,  S,  S,  S,  S,  S,  S,  E,  S, E3,  A },	//$
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
	else if(a[0].isDigit()) {//es un numero
		ret.val += a[0];
		a[0]=' ';
		for(i=1; a[i].isDigit() || a[i]=='.'; i++){
			ret.val += a[i];
			a[i]=' ';
		}
		if(a[i] == '(' || a[i].isLetter())
			a = " *" +a;
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
	}else if(a[0]==':' && a[1] == '='){
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
		err += i18n(QString::QString("Unknown token %1\n").arg(a[0]));
	
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

bool QExp::isLast(QValueStack<int> opr){
	int ins=0;
	bool b=true;
	while(b && !opr.empty()){
		if(opr.top()==tLpr){
			if(ins == 0)
				b= true;
			else
				ins--;
		} else if(opr.top()==tLpr)
			ins++;
		else if(opr.top()==tComa)
			b= false;
		else if(opr.top()==tEof)
			b= false;
		opr.pop();
	}
	return b;
}

int QExp::reduce(){
	QString a="";
// 	cout << opr.top() << endl;
	if(opr.top() == tAdd){
		val.push(QString::QString("<apply><plus />%1%2</apply>").arg(val.pop()).arg(val.pop()));
	} else if(opr.top() == tSub){
		val.push(QString::QString("<apply><minus />%1%2</apply>").arg(val.pop()).arg(val.pop()));
	} else if(opr.top() == tUmi){
		val.push(QString::QString("<apply><minus />%1</apply>").arg(val.pop()));
	} else if(opr.top() == tMul){
		val.push(QString::QString("<apply><times />%1%2</apply>").arg(val.pop()).arg(val.pop()));
	} else if(opr.top() == tDiv){
		val.push(QString::QString("<apply><quotient />%1%2</apply>").arg(val.pop()).arg(val.pop()));
	} else if(opr.top() == tPow){
		val.push(QString::QString("<apply><power />%1%2</apply>").arg(val.pop()).arg(val.pop()));
	} else if(opr.top() == tAssig){ //:=
		val.push(QString::QString("<declare>%1%2</declare>").arg(val.pop()).arg(val.pop()));
	} else if(opr.top() == tFunc){
		a = QString::QString("<apply><%1 />%2</apply>").arg(func.pop()).arg(val.pop());
// 		if(isLast(opr))
// 			a+="</apply>";
		val.push(a);
// 		val.push(QString::QString("<apply><%1 />%2\n").arg(func.pop()).arg(val.pop()));

// 		opr.pop();
// 		cout << "-----sds---> '" << (int)opr.top() << "'" << endl;
	} else if(opr.top() == tComa){
		val.push(QString::QString("%1%2").arg(val.pop()).arg(val.pop()));
// 		val.push(val.pop() + QString::QString("%1").arg(val.pop()));
		//opr.pop();
	} else if(opr.top() == tRpr){
// 		val.push("</apply>");
		opr.pop();
// 		cout << "--------> '" << (int)opr.top() << "'" << endl;
	} //else
// 		cout << opr.top() << endl;
	opr.pop();
	return 0;
}

int QExp::parse(){
	oprTop=0; valTop=-1;
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
			case R:
				if(reduce()) return 1;
				break;
			case S:
				if(shift()) return 1;
				break;
			case A:
				mml = "<math>" + val.pop() + "</math>";
// 				cout << mml.ascii() << endl;
				return 0;
			case E1:
				err += i18n("Missing right paranthesis\n");
				return 1;
			case E2:
				err += i18n("Missing operator\n");
				return 2;
			case E3:
				err += i18n("Unbalanced right parenthesis\n");
				return 3;
			case E:
				err += i18n("Error\n");
				return 4;
			default:
				//cerr << "?? (" << (int) opr.top() << ", " << (int) tok << ") = "  << (int) parseTbl[opr.top()][tok] << endl;
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
