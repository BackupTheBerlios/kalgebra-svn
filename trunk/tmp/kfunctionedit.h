#ifndef KFUNCTIONEDIT_H
#define KFUNCTIONEDIT_H

#include <QLayout>
#include <QLabel>
#include <QLocale>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QComboBox>
#include <QList>

#include "qgraph.h"
#include "qexpressionedit.h"
#include "qalgebrahighlighter.h"

/**
@author Aleix Pol i Gonzalez
*/

class ColorCombo : public QComboBox
{
Q_OBJECT
public:
	ColorCombo(QWidget* parent=0);
	~ColorCombo(){}
	
	QColor color() const;
	void setColor(const QColor &);
	
	void resizeEvent(QResizeEvent * event);
};

class KFunctionEdit : public QDialog
{
Q_OBJECT
public:
	KFunctionEdit(QWidget *parent = 0, bool modal = FALSE, Qt::WFlags f = 0 );
	~KFunctionEdit();
	QString text() const { return m_func->toPlainText(); }
	void setText(const QString &newText);
	QColor color() const { return m_color->color(); }
	void setColor(const QColor &newColor);
	bool isCorrect() const { return m_correct; }
	bool isMathML() const { return m_func->isMathML();}
protected:
	QExpressionEdit *m_func;
	
	QLabel *m_valid;
	QGraph *m_graph;
	ColorCombo *m_color;
	
	bool m_correct;
	QDialogButtonBox * buttonBox;
private slots:
	void edit();
	void ok();
	void colorChange(int);
};

#endif
