#ifndef KFUNCTIONEDIT_H
#define KFUNCTIONEDIT_H

#include <kdialogbase.h>
#include <kcolorcombo.h>
#include <kpushbutton.h>
#include <klineedit.h>
#include <qlayout.h>
#include <qlabel.h>
#include <klocale.h>
#include "qgraph.h"

/**
@author Aleix Pol i Gonzalez
*/
class KFunctionEdit : public KDialogBase
{
Q_OBJECT
public:
	KFunctionEdit(QWidget *parent = 0, const char *name = 0, bool modal = FALSE, WFlags f = 0 );
	~KFunctionEdit();
	QString text() { return m_func->text(); }
	void setText(QString newText) { m_func->setText(newText); }
	QColor color(){ return m_color->color(); }
	void setColor(QColor newColor) { m_color->setColor(newColor); }
private:
	KLineEdit *m_func;
	KColorCombo *m_color;
	QLabel *m_valid;
	QGraph *m_graph;
	
private slots:
	void edit();
};

#endif
