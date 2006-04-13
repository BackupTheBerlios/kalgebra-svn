#ifndef KVAREDIT_H
#define KVAREDIT_H

#include <kdialogbase.h>
#include <klocale.h>
#include <qtooltip.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qhbox.h>

#include "analitza.h"
#include "qexpressionedit.h"

/**
@author Aleix Pol i Gonzalez
*/
class KVarEdit : public KDialogBase
{
Q_OBJECT
public:
	KVarEdit(QString var, QString exp, Variables *v=NULL, QWidget *parent = 0, const char *name = 0, bool modal = FALSE, WFlags f = 0 );
	~KVarEdit();
	QString text() { return m_exp->text(); }
	void setExpression(QString newExp);	//This should edit variable lineedit
	void setVar(QString newVar);		//This should edit the variable name
	void setMode(int mode);			//This should select what Option we need
	QDomElement val();
private:
	QExpressionEdit *m_exp;
	
	QRadioButton *m_opt_calc;		//Per escollir guardar calcul
	QRadioButton *m_opt_exp;		//Per escollir guardar expressio
	
	QLabel *m_valid;
	Variables *vars;
	bool m_correct;
	QString m_var;
private slots:
	void edit(bool);
	void ok();
};

#endif
