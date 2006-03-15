
#ifndef _KALGEBRA_H_
#define _KALGEBRA_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qlineedit.h>
#include <qlabel.h>
#include <qfile.h>
#include <qlabel.h>
#include <qpoint.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qvbox.h>
#include <qtextedit.h>
#include <qlayout.h>

#include <kmainwindow.h>
#include <kpushbutton.h>
#include <klocale.h>
#include <kpopupmenu.h>
#include <kmenubar.h>
#include <khtml_part.h>
#include <klistview.h>
#include <kaboutdialog.h>
#include <kfiledialog.h>
#include <kmainwindow.h>
#include <kurl.h>
#include <kparts/browserextension.h>
#include <ktabwidget.h>
#include <dcopclient.h>
#include <kstandarddirs.h>

#include "kfunctionedit.h"
#include "kvaredit.h"
#include "function.h"
#include "analitza.h"
#include "qgraph.h"
#include "q3dgraph.h"
#include "kalgebraiface.h"
#include "qexpressionedit.h"

#define G2D_RES_LOW	0
#define G2D_RES_STD	1
#define G2D_RES_FINE	2
#define G2D_RES_VFINE	3
#define G2D_TOGGSQUARES	4
#define G3D_TRANS	5
#define G3D_TYPE_DOTTED	6
#define G3D_TYPE_LINES	7
#define G3D_TYPE_SOLID	8

#define G_POINTS	0
#define G_LINES		1
#define G_SOLID		2

	/**
	* @short KAlgebra Main Window
	* @author ,,, <aleixpol@gmail.com>
	* @version 0.4.1
	*/

class KAlgebra : public KMainWindow, virtual public KAlgebraIface
{
    Q_OBJECT
public:
	KAlgebra();
	virtual ~KAlgebra();
	static QString treu_tags(QString in);
	
	//DCOP implementations
	void calculate(QString operation);
	void plot2D(QString operation);
	void add2D(QString operation);
	QStringList list2D();
	void remove2D(int n);
	void plot3D(QString operation);

private:
	QTabWidget *pestanya;
	KMenuBar *menu;
	
	//tab consola
	QExpressionEdit *operacio;
	QExpressionEdit *operacioMML;
	KTabWidget *tabOperacio;
	void opera_gen(QString);
	QAlgebraHighlighter *m_alg_high;
	
	KHTMLPart *log;
	KListView *varlist;
	QString hist;
	bool ultim_error;
	Analitza a;
	void update_varlist();
	
	//tab grafic2D
	QGraph *grafic;
	KTabWidget *tabFuncio2d;
	KListView *func2dlist;
	KPopupMenu *res;
	KPopupMenu *g2d;
	void g2d_res_check(int);
	
	//tab grafic3D
	QExpressionEdit *funcio3d;
	QExpressionEdit *funcio3dMML;
	Q3DGraph *grafic3d;
	KTabWidget *tabFuncio3d;
	bool transparencia;
	KPopupMenu *g3d;
	KPopupMenu *tipus3d;
	void g3d_type_check(int);
	
public slots:
	//Console
	void opera();
	void operaMML();
	void saveLog();
	void edit_var(QListViewItem *item, const QPoint &p,int);
	
	//graf2D
	void set_res_low();
	void set_res_std();
	void set_res_fine();
	void set_res_vfine();
	void slot_editat(QListViewItem *);
	void new_func();
	void imatge2d();
	void dibuixaMML();
	void edit_func(QListViewItem *item, const QPoint &p,int);
	void slot_togglesquares();
	
	//graf3D
	void slot_3dpoints();
	void slot_3dlines();
	void slot_3dsolid();
	void slot_transparencia();
	void slot_getpixmap();
	void dibuixa3d();
	void dibuixa3dMML();
};

#endif // _KALGEBRA_H_
