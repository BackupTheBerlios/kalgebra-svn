#ifndef KVAREDIT_H
#define KVAREDIT_H

#include <kdialogbase.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <klocale.h>
#include <qhbox.h>

/**
@author Kiko Canes i Gonzalez xD
*/
class KVarEdit : public KDialogBase
{
Q_OBJECT
public:
	KVarEdit(QWidget *parent = 0, const char *name = 0, bool modal = FALSE, WFlags f = 0 );
	~KVarEdit();
	QString text() { return m_exp->text(); }
	void setExpression(QString newExp) { m_exp->setText(newExp); } //This should edit variable lineedit
	void setVar(QString newVar){ qDebug("Kiko desperta: %s", newVar.ascii()); } //This should edit the variable name
	void setMode(int mode); //This should select what Option we need
private:
// 	QComboBox *m_vars;
	QLineEdit *m_exp;
	
	QRadioButton *m_opt_calc; //Per escollir guardar calcul
	QRadioButton *m_opt_exp; //Per escollir guardar expressio
private slots:
	void edit();
};

#endif
