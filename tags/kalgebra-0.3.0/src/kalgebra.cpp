#include "kalgebra.h"	

#define G_POINTS 0
#define G_LINES 1
#define G_SOLID 2

KAlgebra::KAlgebra(): KMainWindow( 0, "KAlgebra" ) {
//	setXMLFile("kalgebraui.rc");
	ultim_error = true;
	
	this->setMinimumSize(809,500);
	pestanya = new KTabWidget(this, "tab Principal");
	pestanya->setFocusPolicy(QWidget::NoFocus);
	menu = new KMenuBar(this);
	
	//tab consola/////////////////
	QVBox * consola = new QVBox (pestanya);
	QHBox * state = new QHBox (consola);
	state->setSpacing(2);
	log =new KHTMLPart(state);
	log->widget()->setFocusPolicy(QWidget::NoFocus);
	varlist = new KListView(state);
	varlist->setFocusPolicy(QWidget::NoFocus); //WEO!
	varlist->setMaximumWidth(222);
	varlist->addColumn(i18n("Name"),60);
	varlist->addColumn(i18n("Value"));
	operacio = new QLineEdit (consola);
	operacio->setText("");
	operacio->setFocus();
	operacioMML = new QLineEdit (consola);
	operacioMML->setText("<math><apply><plus /><cn>2</cn><cn>2</cn></apply></math>");
	pestanya->addTab(consola, i18n("Console"));
	connect(operacio, SIGNAL(returnPressed()), this, SLOT(opera()));
	connect(operacioMML, SIGNAL(returnPressed()), this, SLOT(operaMML()));
	
	tabOperacio = new KTabWidget(consola, "tab Entrada Consola");
	tabOperacio->addTab(operacio, i18n("Operation"));
	tabOperacio->addTab(operacioMML, i18n("MathML Operation"));
	tabOperacio->setMaximumHeight(80);
	
	KPopupMenu *menu_consola = new KPopupMenu(this);
	menu_consola->insertItem(i18n("Save Log"), this, SLOT(saveLog()));
	menu->insertItem(i18n("Console"), menu_consola);
	consola->setTabOrder(operacio, tabOperacio);
	//tab consola/////////////////
	
	//tab grafic2D/////////////////
	QVBox *dibuix = new QVBox (pestanya);
	QHBox *hgraf2d = new QHBox (dibuix);
	hgraf2d->setSpacing(2);
	grafic = new QGraph(hgraf2d);
	QVBox *funcs2d = new QVBox (hgraf2d);
	funcs2d->setMaximumWidth(222);
	func2dlist = new KListView(funcs2d);
// 	func2dlist->setDefaultRenameAction ( KListView::Reject);
	KPushButton *afegeix = new KPushButton(i18n("Add function"), funcs2d);
	func2dlist->addColumn(i18n("Function"));
	func2dlist->addColumn(i18n("Color"));
/*	func2dlist->setItemsRenameable(true);*/
	pestanya->addTab(dibuix, i18n("2D Graph"));
	
	connect(func2dlist, SIGNAL(itemRenamed ( QListViewItem *) ), this, SLOT(slot_editat(QListViewItem *)));
	connect(func2dlist, SIGNAL(clicked( QListViewItem *) ), this, SLOT(slot_editat(QListViewItem*)));
	connect(afegeix, SIGNAL(clicked()), this, SLOT(new_func()));
	connect(func2dlist, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)), this, SLOT(edit_func(QListViewItem*, const QPoint&, int )));
	
	res = new KPopupMenu(this);
		res->insertItem("16", this, SLOT(set_res_16()),0,G2D_RES_16);
		res->insertItem("32", this, SLOT(set_res_32()),0,G2D_RES_32);
		res->insertItem("64", this, SLOT(set_res_64()),0,G2D_RES_64);
		res->insertItem("128", this, SLOT(set_res_128()),0,G2D_RES_128);
	set_res_64();
		
	KPopupMenu *g2d = new KPopupMenu(this);
		g2d->insertItem(i18n("Save Picture"), this, SLOT(imatge2d()));
		g2d->insertItem(i18n("Resolution (points/unit)"), res);
	menu->insertItem(i18n("&Graph"), g2d);
	//tab grafic2D/////////////////
	
	//tab grafic3D/////////////////
	QVBox *dibuix3d = new QVBox (pestanya);
	grafic3d = new Q3DGraph(dibuix3d);
	
	tabFuncio3d = new KTabWidget(dibuix3d, "tab Entrada Grafic3D");
	funcio3d = new QLineEdit (tabFuncio3d);
	funcio3d->setText("sin(x)+sin(y)");
	funcio3dMML = new QLineEdit (dibuix3d);
	funcio3dMML->setText("<math><apply><plus/><apply><sin/><ci>x</ci></apply><apply><sin/><ci>y</ci></apply></apply></math>");
	pestanya->addTab(dibuix3d, i18n("3D Graph"));
	connect(funcio3d , SIGNAL( returnPressed() ), this, SLOT(dibuixa3d()) );
	connect(funcio3dMML , SIGNAL( returnPressed() ), this, SLOT(dibuixa3dMML()) );
	
	tabFuncio3d->addTab(funcio3d, "f(x,y) function");
	tabFuncio3d->addTab(funcio3dMML, "MathML f(x,y) function");
	tabFuncio3d->setMaximumHeight(80);
	
	//menu3d
	transparencia=false;
	tipus3d = new KPopupMenu(this);
		tipus3d->insertItem(i18n("Dots"), this, SLOT(slot_3dpoints()),0,G3D_TYPE_DOTTED);
		tipus3d->insertItem(i18n("Lines"), this, SLOT(slot_3dlines()),0,G3D_TYPE_LINES);
		tipus3d->insertItem(i18n("Solid"), this, SLOT(slot_3dsolid()),0,G3D_TYPE_SOLID);
	slot_3dsolid();
		
	g3d = new KPopupMenu(this);
		g3d->insertItem(i18n("Save Picture"), this, SLOT(slot_getpixmap()));
		g3d->insertItem(i18n("Transparency"), this, SLOT(slot_transparencia()),0,G3D_TRANS);
		g3d->insertItem(i18n("Graph Type"), tipus3d);
	menu->insertItem(i18n("G&raph 3D"), g3d);
	//tab grafic3D/////////////////
	
	KPopupMenu *help = helpMenu();
	menuBar()->insertItem( i18n("&Help"), help );
	
	setCentralWidget( pestanya );
	update_varlist();
}

KAlgebra::~KAlgebra(){}

void KAlgebra::opera(){
	opera_gen(false);
}

void KAlgebra::dibuixa3d(){
	grafic3d->setFunc(funcio3d->text());
}

void KAlgebra::operaMML(){
	opera_gen(true);
}

void KAlgebra::opera_gen(bool MML){
	QFile file( "/tmp/kalgebra_log.xml" );
// 	cout << MML << endl;
	
	if ( file.open( IO_WriteOnly ) ) {
		QTextStream stream(&file);
		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
		stream << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\n<head>\n\t<title> :) </title>\n";
		stream << "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"http://math.nist.gov/~BMiller/mathml-css/style/mathml.css\" />\n";
		stream << "<style type=\"text/css\">\n";
		stream << "\t.error { border-style: solid; border-width: 1px; border-color: #ff3b21; background-color: #ffe9c4; padding:7px; }\n";
		stream << "\t.last  { border-style: solid; border-width: 1px; border-color: #2020ff; background-color: #e0e0ff; padding:7px; }\n";
		stream << "\t.before { text-align:right; }\n";
		stream << "</style></head>\n<body>\n";
		
		if(!ultim_error)
			hist = "<p class=\"before\">" + hist;
		
		int err_no=0;
		
		if(MML)
			err_no = a.setTextMML(operacioMML->text());
		else{
			err_no = a.setText(operacio->text());
			operacioMML->setText(a.textMML());
		}
// 		qDebug("%d", a.elem.toDocument().toString());
		if(err_no!=0){
			hist = "<p class=\"error\"> ERROR: "+ a.err +" </p>" + hist;
			ultim_error = true;
		} else {
			stream << "<p class=\"last\">\n";
			QDomElement b = a.Calcula();
			
			if (b.tagName() == "cn" || b.tagName() == "ci") {
				hist = a.toString() + " = " + b.text().stripWhiteSpace() + "</p>\n" + hist;
				a.vars.modifica("ans", b);
			} else if(a.elem.toElement().tagName()=="declare"){
				hist = a.toString() + hist;
			} else if(Analitza::isNum(b.tagName()))
				hist = a.toString() + " = " + b.tagName() + "</p>\n" + hist;
			
			ultim_error = false;
		}
		
		stream << hist;
		stream << "</body></html>\n";
		file.close();
		//qDebug(operacio->text().ascii());
	} else
		qDebug("malament");
	operacio->setText("");
	log->openURL("/tmp/kalgebra_log.xml");
	update_varlist();
}

void KAlgebra::saveLog(){
	QString path = KFileDialog::getSaveFileName( QString::null, "*.xml", this );
	if(path=="")
		return;
	QFile file(path);
	
	if ( file.open( IO_WriteOnly ) ) {
		QTextStream stream(&file);
		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
		stream << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\n<head>\n\t<title> KAlgebra Log </title>\n";
		stream << "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"http://math.nist.gov/~BMiller/mathml-css/style/mathml.css\" />\n";
		stream << "<style type=\"text/css\">\n";
		stream << ".error { border-style: solid; border-width: 1px; border-color: #ff3b21; background-color: #ffe9c4; padding:7px; }\n";
		stream << ".last  { border-style: solid; border-width: 1px; border-color: #2020ff; background-color: #e0e0ff; padding:7px; }\n";
		stream << ".before { text-align:right; }\n";
		stream << "</style></head><body>\n";
		stream << "<p class=\"before\">" +hist;
		stream << "</body></html>\n";
		file.close();
	}
}

void KAlgebra::update_varlist(){
	varlist->clear();
	QStringList qsl = a.vars.getNoms();
	for ( QStringList::Iterator it = qsl.begin(); it != qsl.end(); ++it ) {
// 		QListViewItem *klvi = new QListViewItem( varlist, *it, QString("%1").arg(a.toNum(a.vars.getValue(*it))));
		QListViewItem *klvi = new QListViewItem( varlist, *it, treu_tags(a.str(a.vars.value(*it))).stripWhiteSpace());
		varlist->insertItem(klvi);
	}
}

void KAlgebra::dibuixaMML(){
// 	grafic->setFuncMML(funcioMML->text());
}

void KAlgebra::dibuixa3dMML(){
	grafic3d->setFuncMML(funcio3dMML->text());
}

void KAlgebra::imatge2d(){
	grafic->toImage(KFileDialog::getSaveFileName( QString::null, "*.svg", this ));
}

/*void KAlgebra::imatge3dMML(){
	grafic3d->outputFuncMML(funcio3dMML->text(), path);
}*/

void KAlgebra::set_res_16() { grafic->setResolution(16); g2d_res_check(G2D_RES_16); }
void KAlgebra::set_res_32() { grafic->setResolution(32); g2d_res_check(G2D_RES_32); }
void KAlgebra::set_res_64() { grafic->setResolution(64); g2d_res_check(G2D_RES_64); }
void KAlgebra::set_res_128(){ grafic->setResolution(128); g2d_res_check(G2D_RES_128); }

void KAlgebra::g2d_res_check(int id){
	for(int i=G2D_RES_16;i<=G2D_RES_128; i++)
		res->setItemChecked(i, id==i);
}

void KAlgebra::g3d_type_check(int id){
	for(int i=G3D_TYPE_DOTTED;i<=G3D_TYPE_SOLID; i++)
		tipus3d->setItemChecked(i, id==i);
}

void KAlgebra::slot_3dpoints(){ grafic3d->setMethod(G_POINTS); g3d_type_check(G3D_TYPE_DOTTED); }
void KAlgebra::slot_3dlines() { grafic3d->setMethod(G_LINES); g3d_type_check(G3D_TYPE_LINES); }
void KAlgebra::slot_3dsolid() { grafic3d->setMethod(G_SOLID); g3d_type_check(G3D_TYPE_SOLID); }

void KAlgebra::slot_transparencia() {
	g3d->setItemChecked(G3D_TRANS, transparencia = !transparencia);
	grafic3d->setTraslucency(transparencia);
}

void KAlgebra::slot_getpixmap(){
	QString path = KFileDialog::getSaveFileName( QString::null, "*.png", this );
	grafic3d->toPixmap().save(path, "PNG");
}

void KAlgebra::slot_editat(QListViewItem *){
	draw();
}

void KAlgebra::draw(){
	QStringList a;
	QValueList<QColor> colors;
	
	QListViewItemIterator it(func2dlist, QListViewItemIterator::Checked );
	while ( it.current() ) {
		a << it.current()->text(0);

		colors << it.current()->pixmap(1)->convertToImage().pixel(2,2);
		++it;
	}
	
	grafic->setFunc(a,colors);
}

void KAlgebra::new_func(){
	KFunctionEdit *e = new KFunctionEdit(this) ;
	if(e->exec() == QDialog::Accepted ) {
		QPixmap p(15,15);
		p.fill(e->color());
		QCheckListItem *klvi = new QCheckListItem( func2dlist, e->text(), QCheckListItem::CheckBox);
		klvi->setPixmap(1, p);
		klvi->setState(QCheckListItem::On);
		func2dlist->insertItem(klvi);
		draw();
	}
	delete e;
}

void KAlgebra::edit_func(QListViewItem *item, const QPoint &,int) {
	KFunctionEdit *e = new KFunctionEdit(this) ;
	e->setText(item->text(0));
	e->setColor(item->pixmap(1)->convertToImage().pixel(2,2));
	if(e->exec() == QDialog::Accepted ) {
		QPixmap p(15,15);
		p.fill(e->color());
		item->setText(0, e->text());
		item->setPixmap(1, p);
		func2dlist->insertItem(item);
		draw();
	}
	delete e;
}

QString KAlgebra::treu_tags(QString in){
	bool tag=false;
	QString out;
	for(unsigned int i=0; i<in.length(); i++){
		if(in[i]=='<')
			tag=true;
		else if(in[i]=='>')
			tag=false;
		else if(!tag)
			out += in[i];
	}
	return out;
}

#include "kalgebra.moc"
