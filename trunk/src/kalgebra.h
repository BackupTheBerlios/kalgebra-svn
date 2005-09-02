

#ifndef _KALGEBRA_H_
#define _KALGEBRA_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kmainwindow.h>
#include <kpushbutton.h>
#include <klocale.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qfile.h>
#include <kpopupmenu.h>
#include <kmenubar.h>
#include <khtml_part.h>
#include <klistview.h>
#include <kaboutdialog.h>
#include <kfiledialog.h>
#include <kmainwindow.h>
#include <kurl.h>
#include <kparts/browserextension.h>
#include <qlabel.h>
#include <qpoint.h>
#include <ktabwidget.h>
#include <qpainter.h>

#include "kfunctionedit.h"
#include "analitza.h"
#include "qgraph.h"
#include "q3dgraph.h"

#define G2D_RES_16	0
#define G2D_RES_32	1
#define G2D_RES_64	2
#define G2D_RES_128	3
#define G3D_TRANS	4
#define G3D_TYPE_DOTTED	5
#define G3D_TYPE_LINES	6
#define G3D_TYPE_SOLID	7

class QLineEdit;
class KHTMLPart;

/**
 * @short KAlgebra Main Window
 * @author ,,, <aleixpol@gmail.com>
 * @version 0.2
 */
class KAlgebra : public KMainWindow
{
    Q_OBJECT
public:
	KAlgebra();
	virtual ~KAlgebra();
	static QString treu_tags(QString in);

private:
	QTabWidget *pestanya;
	KMenuBar *menu;
	
	//tab consola
	QLineEdit *operacio;
	QLineEdit *operacioMML;
	KTabWidget *tabOperacio;
	void opera_gen(bool);
	
	KHTMLPart *log;
	KListView *varlist;
	QString hist;
	bool ultim_error;
	Analitza a;
	void update_varlist();
	
	//tab grafic2D
	QLineEdit *funcio;
	QLineEdit *funcioMML;
	QGraph *grafic;
	KTabWidget *tabFuncio2d;
	KListView *func2dlist;
	KPopupMenu *res;
	void g2d_res_check(int);
	void draw();
	
	//tab grafic3D
	QLineEdit *funcio3d;
	QLineEdit *funcio3dMML;
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
	
	//graf2D
	void set_res_16();
	void set_res_32();
	void set_res_64();
	void set_res_128();
	void slot_editat(QListViewItem *);
	void new_func();
	void imatge2d();
/*	void dibuixa();*/
	void dibuixaMML();
	void edit_func(QListViewItem *item, const QPoint &p,int);
// 	void list_edit(QListViewItem*);
// 	void list_delete(QListViewItem*);
	
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
