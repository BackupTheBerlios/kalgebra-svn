#include "operatorsmodel.h"

#define KEYWORDNUM 200

OperatorsModel::OperatorsModel() : QStandardItemModel(KEYWORDNUM, 3, NULL)
{
	QStringList ops;
	ops << "plus" << "times" << "minus" << "divide" << "quotient";
	ops << "power" << " root" << " factorial";
	ops << "_and" << "_or" << "_xor" << "_not";
	ops << "gcd" << "lcm" << "rem" << "factorof";
	ops << "max" << "min";
	ops << "lt" << "gt" << "eq" << "neq" << "leq" << "geq" << "implies";
	ops << "approx" << "abs" << "floor" << "ceiling";
	ops << "sin" << "cos" << "tan";
	ops << "sec" << "csc" << "cot";
	ops << "sinh" << "cosh" << "tanh";
	ops << "sech" << "csch" << "coth";
	ops << "arcsin" << "arccos" << "arctan";
	ops << "arccot" << "arcoth";
	ops << "arccosh" << "arccsc" << "arccsch";
	ops << "arcsec" << "arcsech" << "arcsinh" << "arctanh";
	ops << "exp" << "ln" << "log";
	ops << "conjugate" << "arg" << "real" << "imaginary";
	ops << "sum" << "product";
	
	int count = ops.count();
	
	for (int i=0; i<count; ++i) {
		setData(index(i, 0), ops[i]);
		setData(index(i, 1), Object::oper);
		setData(index(i, 2), Operator::toOperatorType(ops[i]));
	}
}

QString OperatorsModel::operToString(const Operator& op) const
{
	QStandardItem *it;
	
	for(int i=0; i<KEYWORDNUM; i++) {
		it=item(i,2);
		if(it!=NULL && it->data(Qt::EditRole).toInt()==op.operatorType()) {
			return item(i,0)->data(Qt::EditRole).toString();
		}
	}
	return QString();
}
