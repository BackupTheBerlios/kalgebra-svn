#ifndef OPERATOR_H
#define OPERATOR_H

#include <QStandardItemModel>
#include "object.h"

class Operator;
class OperatorsModel;

class Operator : public Object
{
	public:
		Operator(enum OperatorType t) : Object(oper), m_optype(t) {}
		Operator(Object *o);
		virtual ~Operator() {}
		
		void setOperator(enum OperatorType t) { m_optype=t; }
		static int nparams(enum OperatorType);
		int nparams() const { return nparams(m_optype); }
		enum OperatorType operatorType() const { return m_optype; }
		static enum OperatorType toOperatorType(QString s);
		unsigned int weight() const { return operatorWeight(m_optype); }
		static unsigned int operatorWeight(OperatorType);
		QString toString() const;
		QString toMathML() const;
		
		Operator operator=(const Operator &a) { setType(a.type()); m_optype=a.operatorType(); return *this;}
		static const OperatorsModel m_words;
	private:
		enum OperatorType m_optype;
};

class OperatorsModel : public QStandardItemModel
{
public:
	OperatorsModel(QObject *parent=NULL);
	QString operToString(const Operator& op) const;
	int count() const { return m_count; }
private:
	int m_count;
};

#endif
