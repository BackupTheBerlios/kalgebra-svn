#ifndef OPERATORSMODEL_H
#define OPERATORSMODEL_H

#include <QStandardItemModel>

#include "object.h"

class OperatorsModel : public QStandardItemModel
{
public:
	OperatorsModel();
	QString operToString(const Operator& op) const;
};

#endif
