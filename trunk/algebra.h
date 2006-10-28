#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QTreeWidget>
#include <QToolBox>
#include <QTabWidget>

#include "qexpressionedit.h"
#include "qgraph.h"
#include "kfunctionedit.h"
#include "q3dgraph.h"
#include "console.h"

class QAlgebra : public QMainWindow {
Q_OBJECT
public:
	QAlgebra(QWidget *parent);
	~QAlgebra(){}
	QString calculate(QString exp);
private:
	//console
	QExpressionEdit *c_exp;
	Console *c_results;
	VariableView *c_variables;
	int outs;
	QDockWidget *c_dock_vars;
	
	//graf 2d
	QTreeWidget *b_funcs;
	QTabWidget *b_tools;
	QGraph *grafic;
	QDockWidget *b_dock_funcs;
	KFunctionEdit *b_funced;
	
	//graph 3d
	QExpressionEdit *t_exp;
	Q3DGraph *grafic3d;
	
public slots:
	void operate();
	void insert(QListWidgetItem * item);
	void loadScript();
	void saveLog();
	
	void afegeix();
	void new_func();
	void edit_func(const QModelIndex &);
	void edit_var(const QModelIndex &);
	void canvi(QTreeWidgetItem * item, QTreeWidgetItem * ant);
	void toggleSquares();
	void set_res_low();
	void set_res_std();
	void set_res_fine();
	void set_res_vfine();
	void different(QTreeWidgetItem * item, int column);
	void saveGraph();
	void functools(int);
	
	void changeStatusBar(const QString &);
	void tabChanged(int);
	void set_dots();
	void set_lines();
	void set_solid();
	void toggleTransparency();
	void save3DGraph();
	
	void about();
};

#endif
