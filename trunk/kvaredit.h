#ifndef KVAREDIT_H
#define KVAREDIT_H

#include <QToolTip>
#include <QDialog>
#include <QLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QDialogButtonBox>

#include "qexpressionedit.h"
//Added by qt3to4:
#include <QLabel>

/**
@author Aleix Pol i Gonzalez
*/

class KVarEdit : public QDialog
{
Q_OBJECT
public:
	KVarEdit(QWidget *parent = 0, bool modal = FALSE);
	~KVarEdit();
	QString text() const { return m_exp->text(); }
	void setVar(const QString& newVar);	//This should edit the variable name
	void setMode(int mode);			//This should select what Option we need
	void setAnalitza(Analitza *na) { vars= na->m_vars; m_exp->setAnalitza(na); }
	Object* val();
private:
	QExpressionEdit *m_exp;
	
	QRadioButton *m_opt_calc;		//Per escollir guardar calcul
	QRadioButton *m_opt_exp;		//Per escollir guardar expressio
	
	QLabel *m_valid;
	Variables *vars;
	bool m_correct;
	QString m_var;
	
	QDialogButtonBox *buttonBox;
private slots:
	void edit();
	void ok();
};

#endif
