#include <QVBoxLayout>
#include <QTabWidget>
#include <QLayout>
#include <QSplitter>
#include <QStatusBar>
#include <QMenuBar>
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QFileDialog>
#include <QHeaderView>
#include <QSplitter>
#include <QDockWidget>

#include "algebra.h"
#include "qexpressionedit.h"
#include "kvaredit.h"

QAlgebra::QAlgebra(QWidget *p) : QMainWindow(p)
{
	this->setMinimumHeight(500);
	this->setMinimumWidth(900);
	
	QTabWidget *tabs = new QTabWidget(this);
	this->setCentralWidget(tabs);
	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	
	setStatusBar(new QStatusBar(this));
	setMenuBar(new QMenuBar(this));
	
	///////Consola
	QWidget *console = new QWidget(p);
	QVBoxLayout *c_layo = new QVBoxLayout(console);
	c_results = new Console(this);
	c_dock_vars = new QDockWidget(i18n("Variables"), this);
	c_dock_vars->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
	this->addDockWidget(static_cast<Qt::DockWidgetArea>(2), c_dock_vars);
	
	c_variables = new VariableView(c_dock_vars);
	c_exp = new QExpressionEdit(console);
	c_exp->setAnalitza(c_results->analitza());
	c_variables->setAnalitza(c_results->analitza());
	c_dock_vars->setWidget(c_variables);
	
	tabs->addTab(console, i18n("&Console"));
	console->setLayout(c_layo);
	c_layo->addWidget(c_results);
	c_layo->addWidget(c_exp);
	
	connect(c_exp, SIGNAL(returnPressed()), this, SLOT(operate()));
	connect(c_results, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(insert(QListWidgetItem *)));
	connect(c_results, SIGNAL(status(const QString &)), this, SLOT(changeStatusBar(const QString &)));
	connect(c_results, SIGNAL(changed()), c_variables, SLOT(updateVariables()));
	connect(c_results, SIGNAL(changed()), c_exp, SLOT(updateCompleter()));
	connect(c_variables, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(edit_var(const QModelIndex &)));
	
	////////menu
	QMenu *c_menu = menuBar()->addMenu(i18n("C&onsole"));
	c_menu->addAction(i18n("&New"), c_results, SLOT(clear()), QKeySequence::New);
	c_menu->addAction(i18n("&Load Script"), this, SLOT(loadScript()), Qt::CTRL+Qt::Key_L);
	c_menu->addAction(i18n("&Save Log"), this, SLOT(saveLog()), QKeySequence::Save);
	c_menu->addSeparator();
	c_menu->addAction(i18n("&Quit"), this, SLOT(close()), Qt::CTRL+Qt::Key_Q);
	////////////
	//////EOConsola
	
	//////2D Graph
	grafic = new QGraph(this);
	
	b_dock_funcs = new QDockWidget(i18n("Functions"), this);
	this->addDockWidget(static_cast<Qt::DockWidgetArea>(2), b_dock_funcs);
	QWidget *b_rwid = new QWidget(b_dock_funcs);
	QVBoxLayout *b_layo = new QVBoxLayout(b_rwid);
	b_funcs = new QTreeWidget(b_dock_funcs);
	b_funcs->headerItem()->setText(0, i18n("Color"));
	b_funcs->headerItem()->setText(1, i18n("Function"));
	b_funcs->header()->setResizeMode(0, QHeaderView::ResizeToContents);
	b_funcs->setSelectionMode(QAbstractItemView::SingleSelection);
	b_funcs->setRootIsDecorated(false);
	b_funcs->setSortingEnabled(false);
	b_funcs->clear();
	b_add = new QPushButton(i18n("&Add"), b_dock_funcs);
	
	tabs->addTab(grafic, i18n("&2D Graph"));
	b_dock_funcs->setWidget(b_rwid);
	b_dock_funcs->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
	b_layo->addWidget(b_funcs);
	b_layo->addWidget(b_add);
	
	connect(b_add, SIGNAL(clicked()), this, SLOT(new_func()));
	connect(b_funcs, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(edit_func(const QModelIndex &)));
	connect(b_funcs, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(canvi(QTreeWidgetItem *, QTreeWidgetItem *)));
	connect(grafic, SIGNAL(status(const QString &)), this, SLOT(changeStatusBar(const QString &)));
	connect(b_funcs, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(different(QTreeWidgetItem *, int)));
	
	////////menu
	QMenu *b_menu = menuBar()->addMenu(i18n("2&D Graph"));
	QAction* b_actions[6];
	b_actions[0] = b_menu->addAction(i18n("&Grid"), this, SLOT(toggleSquares()));
	b_actions[1] = b_menu->addAction(i18n("&Save"), this, SLOT(saveGraph()));
	b_menu->addSeparator()->setText(i18n("Resolution"));
	b_actions[2] = b_menu->addAction(i18n("Poor"), this, SLOT(set_res_low()));
	b_actions[3] = b_menu->addAction(i18n("Normal"), this, SLOT(set_res_std()));
	b_actions[4] = b_menu->addAction(i18n("Fine"), this, SLOT(set_res_fine()));
	b_actions[5] = b_menu->addAction(i18n("Very Fine"), this, SLOT(set_res_vfine()));
	
	QActionGroup *res = new QActionGroup(b_menu);
	res->addAction(b_actions[2]);
	res->addAction(b_actions[3]);
	res->addAction(b_actions[4]);
	res->addAction(b_actions[5]);
	
	b_actions[0]->setCheckable(true);
	b_actions[0]->setChecked(true);
	b_actions[2]->setCheckable(true);
	b_actions[3]->setCheckable(true);
	b_actions[3]->setChecked(true);
	b_actions[4]->setCheckable(true);
	b_actions[5]->setCheckable(true);
	//////EO2D Graph
	
	/////3DGraph
	QWidget *tridim = new QWidget(p);
	QVBoxLayout *t_layo = new QVBoxLayout(tridim);
	t_exp = new QExpressionEdit(tridim);
	grafic3d = new Q3DGraph(tridim);
	
	tridim->setLayout(t_layo);
	tabs->addTab(tridim, i18n("&3D Graph"));
	t_layo->addWidget(grafic3d);
	t_layo->addWidget(t_exp);
	
	connect(t_exp,  SIGNAL(returnPressed()), this, SLOT(afegeix()));
	connect(grafic3d, SIGNAL(status(const QString &)), this, SLOT(changeStatusBar(const QString &)));
	
	////////menu
	QMenu *t_menu = menuBar()->addMenu(i18n("3D &Graph"));
	QAction* t_actions[5];
	t_actions[0] = t_menu->addAction(i18n("&Transparency"), this, SLOT(toggleTransparency()));
	t_actions[1] = t_menu->addAction(i18n("&Save"), this, SLOT(save3DGraph()));
	t_menu->addSeparator()->setText(i18n("Type"));
	t_actions[2] = t_menu->addAction(i18n("Dots"), this, SLOT(set_dots()));
	t_actions[3] = t_menu->addAction(i18n("Lines"), this, SLOT(set_lines()));
	t_actions[4] = t_menu->addAction(i18n("Solid"), this, SLOT(set_solid()));
	
	QActionGroup *t_type = new QActionGroup(t_menu);
	t_type->addAction(t_actions[2]);
	t_type->addAction(t_actions[3]);
	t_type->addAction(t_actions[4]);
	
	t_actions[0]->setCheckable(true);
	t_actions[2]->setCheckable(true);
	t_actions[3]->setCheckable(true);
	t_actions[4]->setCheckable(true);
	t_actions[4]->setChecked(true);
	////////////
	//////EO3D Graph
	
	tabChanged(0);
}

void QAlgebra::afegeix()
{
	grafic3d->setFunc(t_exp->text());
}

void QAlgebra::new_func()
{
	KFunctionEdit e(this) ;
	e.setColor(QColor(0xf2,0,0));
	if(e.exec() == QDialog::Accepted) {
		grafic->addFunction(function(e.text(), e.color(), false, e.isMathML()));
		grafic->setSelected(e.text());
		
		QPixmap ico(15, 15);
		ico.fill(e.color());
		
		QTreeWidgetItem *item = new QTreeWidgetItem(b_funcs);
		item->setFlags(Qt::ItemIsSelectable| Qt::ItemIsUserCheckable| Qt::ItemIsEnabled| Qt::ItemIsTristate);
		item->setIcon(0, ico);
		item->setText(1, e.text());
		item->setTextColor(1, e.color());
		item->setCheckState(0, Qt::Checked);
		grafic->setFocus();
	}
}

void QAlgebra::edit_func(const QModelIndex &)
{
	KFunctionEdit e(this);
	e.setColor(QColor(0xf2,0,0));
	e.setText(b_funcs->currentItem()->text(1));
	if(e.exec() == QDialog::Accepted) {
		grafic->editFunction(b_funcs->currentItem()->text(1), function(e.text(), e.color(), false, e.isMathML()));
		grafic->setSelected(e.text());
		QPixmap ico(15, 15);
		ico.fill(e.color());
		
		b_funcs->currentItem()->setIcon(0, ico);
		b_funcs->currentItem()->setText(1, e.text());
		b_funcs->currentItem()->setTextColor(1, e.color());
		grafic->setFocus();
	}
}

void QAlgebra::edit_var(const QModelIndex &)
{
	KVarEdit e(this, false);
	e.setAnalitza(c_results->analitza());
	e.setVar(c_variables->currentItem()->text(0));
	
	if(e.exec() == QDialog::Accepted)
		c_results->analitza()->m_vars->modify(c_variables->currentItem()->text(0), e.val());
	c_variables->updateVariables();
}

void QAlgebra::operate()
{
	if(!c_exp->text().isEmpty()){
		c_exp->setCorrect(c_results->addOperation(c_exp->text(), c_exp->isMathML()));
		c_exp->selectAll();
	}
}

void QAlgebra::insert(QListWidgetItem * item)
{
	c_exp->insertPlainText(item->toolTip());
	c_exp->setFocus();
}

void QAlgebra::changeStatusBar(const QString& msg)
{
	statusBar()->showMessage(msg);
}

void QAlgebra::loadScript()
{
	QString path = QFileDialog::getOpenFileName(this, i18n("Choose a script"), NULL, "Script (*.kal)");
	if(!path.isEmpty())
		c_results->loadScript(path);
}

void QAlgebra::saveLog()
{
	QString path = QFileDialog::getSaveFileName(this, NULL, NULL, "Text File (*)");
	if(!path.isEmpty())
		c_results->saveLog(path);
}


void QAlgebra::set_res_low()	{ grafic->setResolution(416); }
void QAlgebra::set_res_std()	{ grafic->setResolution(832); }
void QAlgebra::set_res_fine()	{ grafic->setResolution(1664);}
void QAlgebra::set_res_vfine()	{ grafic->setResolution(3328);}

void QAlgebra::set_dots()	{ grafic3d->setMethod(Q3DGraph::Dots);  }
void QAlgebra::set_lines()	{ grafic3d->setMethod(Q3DGraph::Lines); }
void QAlgebra::set_solid()	{ grafic3d->setMethod(Q3DGraph::Solid); }

void QAlgebra::toggleTransparency()
{
	grafic3d->setTransparency(!grafic3d->transparency());
}

void QAlgebra::save3DGraph()
{
	QString path = QFileDialog::getSaveFileName(this, NULL, NULL, i18n("Image File (*.png)"));
	if(!path.isEmpty())
		grafic3d->toPixmap().save(path, "PNG");
}

void QAlgebra::toggleSquares()
{
	grafic->setSquares(!grafic->squares());
}

void QAlgebra::saveGraph()
{
	QString path = QFileDialog::getSaveFileName(this, NULL, NULL, i18n("Image File (*.png);;Vector image file (*.svg)"));
	if(!path.isEmpty())
		grafic->toImage(path);
}

void QAlgebra::canvi(QTreeWidgetItem *current, QTreeWidgetItem *)
{
	grafic->setSelected(current->text(1));
}

void QAlgebra::different(QTreeWidgetItem * item, int)
{
	QColor c(item->icon(0).pixmap(1,1).toImage().pixel(0,0));
	grafic->setShown(function(item->text(1), c), item->checkState(0) == Qt::Checked);
}

void QAlgebra::tabChanged(int n)
{
	c_dock_vars->hide();
	b_dock_funcs->hide();
	switch(n) {
		case 0:
			c_dock_vars->show();
			c_dock_vars->raise();
			c_exp->setFocus();
			break;
		case 1:
			b_dock_funcs->show();
			b_dock_funcs->raise();
			b_add->setFocus();
			break;
		case 2:
			t_exp->setFocus();
			break;
		default:
			break;
	}
	changeStatusBar(i18n("Ready"));
}