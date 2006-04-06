#ifndef KFUNCTIONEDIT_H
#define KFUNCTIONEDIT_H

#include <qlayout.h>
#include <qlabel.h>
#include <qhbox.h>
#include <klocale.h>
#include <kdialogbase.h>
#include <kcolorcombo.h>
#include <kpushbutton.h>
#include <klineedit.h>
#include "qgraph.h"
#include "qexpressionedit.h"
#include "qalgebrahighlighter.h"

/**
@author Aleix Pol i Gonzalez
*/
class KFunctionEdit : public KDialogBase
{
Q_OBJECT
public:
	KFunctionEdit(QWidget *parent = 0, const char *name = 0, bool modal = FALSE, WFlags f = 0 );
	~KFunctionEdit();
	inline QString text() { return m_func->text(); }
	void setText(const QString &newText);
	inline QColor color(){ return m_color->color(); }
	inline void setColor(const QColor &newColor) { m_color->setColor(newColor); }
	inline bool isCorrect(){ return m_correct; }
	
private:
	QExpressionEdit *m_func;
	KColorCombo *m_color;
	QLabel *m_valid;
	QGraph *m_graph;
	
	bool m_correct;
	
private slots:
	void edit(bool);
	void ok(){ accept();};
};

#endif
