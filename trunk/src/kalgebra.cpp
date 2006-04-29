#include "kalgebra.h"

KAlgebra::KAlgebra(): DCOPObject ("KAlgebraIface") , KMainWindow(0, "KAlgebra") {
// 	setXMLFile("kalgebraui.rc");
	this->setMinimumSize(809,500);
	
	m_status = new QLabel(0, this->statusBar());
	m_status->setMinimumWidth(809);
	m_status->setFixedHeight(18);
	m_status->setAlignment(AlignTop);
	this->statusBar()->addWidget(m_status);
	this->statusBar()->show();
	
	pestanya = new KTabWidget(this, "tab Principal");
	pestanya->setFocusPolicy(QWidget::NoFocus);
//	pestanya->setTabPosition(QTabWidget::Bottom); //I wonder how, I wonder why...
	connect(pestanya, SIGNAL(currentChanged(QWidget*)), this, SLOT(tabChanges(QWidget*)));
	
	//tab consola/////////////////
	consola = new QWidget(this);
	QVBoxLayout *cons_layout = new QVBoxLayout(consola);
	QSplitter *log_split = new QSplitter(consola);
	log =new KHTMLPart(log_split);
	log->setOnlyLocalReferences(false);
	log->openURL("http://kalgebra.berlios.de");
	log->widget()->setFocusPolicy(QWidget::NoFocus);
	log->widget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	varlist = new KListView(log_split);
	varlist->setFocusPolicy(QWidget::NoFocus); //FIXME: WEO!
	varlist->setFixedWidth(250);
	varlist->addColumn(i18n("Name"),60);
	varlist->addColumn(i18n("Value"),-1);
	varlist->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	varlist->setResizeMode(QListView::LastColumn);
	operacio = new QExpressionEdit(consola, 0, Autodetect);
	operacio->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	operacio->setText("");
	operacio->setFocus();
	
	pestanya->addTab(consola, i18n("Console"));
	cons_layout->addWidget(log_split);
	cons_layout->setSpacing(5);
	cons_layout->addWidget(operacio);
	
	connect(operacio, SIGNAL(returnPressed()), this, SLOT(opera()));
	connect(operacio, SIGNAL(signalHelper(const QString&)), this, SLOT(changeStatusBar(const QString&)));
	
	connect(varlist, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)), this, SLOT(edit_var(QListViewItem*, const QPoint&, int ))); //I'm disconnecting it until it is developed
	
	KPopupMenu *menu_consola = new KPopupMenu(this);
	KAction *actSaveLog = new KAction(i18n("Save Log"), "savelog", KStdAccel::shortcut(KStdAccel::Save), this, SLOT(saveLog()), actionCollection(), "save");
	actSaveLog->plug(menu_consola);
	
	this->menuBar()->insertItem(i18n("Console"), menu_consola);
	//tab consola/////////////////
	
	//tab grafic2D/////////////////
	hgraf2d = new QSplitter(this);
	
	grafic = new QGraph(hgraf2d);
	QWidget *graphtab = new QWidget(hgraf2d);
	QVBoxLayout *funcs2d = new QVBoxLayout(graphtab);
	graphtab->setMaximumWidth(250);
	func2dlist = new KListView(graphtab);
	func2dlist->addColumn(i18n("Function"));
	func2dlist->addColumn(i18n("Color"));
	func2dlist->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	func2dlist->setResizeMode(QListView::LastColumn);
	addfunc = new KPushButton(i18n("Add function"), graphtab);
	
	funcs2d->addWidget(func2dlist);
	funcs2d->addWidget(addfunc);
	pestanya->addTab(hgraf2d, i18n("2D Graph"));
	
	connect(func2dlist, SIGNAL(clicked( QListViewItem *) ), this, SLOT(slot_editat(QListViewItem*)));
	connect(func2dlist, SIGNAL(spacePressed( QListViewItem *) ), this, SLOT(slot_editat(QListViewItem*)));
	connect(func2dlist, SIGNAL(selectionChanged( QListViewItem *) ), this, SLOT(slot_editat(QListViewItem*)));
	connect(func2dlist, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)), this, SLOT(edit_func(QListViewItem*, const QPoint&, int )));
	connect(addfunc, SIGNAL(clicked()), this, SLOT(new_func()));
	
	res = new KPopupMenu(this);
		res->insertItem(i18n("Poor"), this, SLOT(set_res_low()),0,G2D_RES_LOW);
		res->insertItem(i18n("Standard"), this, SLOT(set_res_std()),0,G2D_RES_STD);
		res->insertItem(i18n("Fine"), this, SLOT(set_res_fine()),0,G2D_RES_FINE);
		res->insertItem(i18n("Very Fine"), this, SLOT(set_res_vfine()),0,G2D_RES_VFINE);
	set_res_std();
		
	g2d = new KPopupMenu(this);
		g2d->insertItem(i18n("Save Picture"), this, SLOT(imatge2d()));
		g2d->insertItem(i18n("Toggle Squares"), this, SLOT(slot_togglesquares()),0,G2D_TOGGSQUARES);
		g2d->insertItem(i18n("Precision"), res);
	this->menuBar()->insertItem(i18n("&Graph"), g2d);
	//tab grafic2D/////////////////
	
	//tab grafic3D/////////////////
	dibuix3d=new QWidget(this);
	QVBoxLayout *layout3d = new QVBoxLayout(dibuix3d);
	grafic3d = new Q3DGraph(dibuix3d);
	
	funcio3d = new QExpressionEdit(dibuix3d,0, Autodetect);
	funcio3d->setText("sin(x)*sin(y)"); //NOTE: La huevera, by Miquel Grau
	pestanya->addTab(dibuix3d, i18n("3D Graph"));
	connect(funcio3d, SIGNAL(returnPressed()), this, SLOT(dibuixa3d()) );
	connect(funcio3d, SIGNAL(signalHelper(const QString&)), this, SLOT(changeStatusBar(const QString&)));
	
	layout3d->setSpacing(5);
	layout3d->addWidget(grafic3d);
	layout3d->addWidget(funcio3d);
	
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
	
	this->menuBar()->insertItem(i18n("G&raph 3D"), g3d);
	//tab grafic3D/////////////////
	
	KPopupMenu *help = helpMenu();
	this->menuBar()->insertItem( i18n("&Help"), help );
	
	ultim_error = true;
	setCentralWidget( pestanya );
	update_varlist();
}

KAlgebra::~KAlgebra(){}

void KAlgebra::dibuixa3d(){
	if(funcio3d->isMathML())
		grafic3d->setFuncMML(funcio3d->text());
	else
		grafic3d->setFunc(funcio3d->text());
}

void KAlgebra::opera(){
	if(!operacio->text().isEmpty())
		opera_gen(operacio->text());
}

void KAlgebra::opera_gen(QString op){
	double b=0.;
	QFile file( "/tmp/kalgebra_log.xml" );
	
	if ( file.open( IO_WriteOnly ) ) {
		QTextStream stream(&file);
		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
		stream << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\n<head>\n\t<title> :) </title>\n";
		//stream << "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"http://math.nist.gov/~BMiller/mathml-css/style/mathml.css\" />\n";
		stream << "<style type=\"text/css\">\n";
		stream << "\t.error { border-style: solid; border-width: 1px; border-color: #ff3b21; background-color: #ffe9c4; padding:7px; }\n";
		stream << "\t.last  { border-style: solid; border-width: 1px; border-color: #2020ff; background-color: #e0e0ff; padding:7px; }\n";
		stream << "\t.before { text-align:right; }\n";
		stream << "\t.op  { font-weight: bold; }\n";
		stream << "\t.sep { font-weight: bold; }\n";
		stream << "\t.num { color: #0000C4; }\n";
		stream << "\t.var { color: #640000; }\n";
		stream << "\t.func{ color: #003600; }\n";
		stream << "</style>\n</head>\n<body>\n";
		
		if(!ultim_error)
			hist.prepend("<p class=\"before\">");
		
		int err_no=0;
		
		if(op[0]=='<')
			err_no = a.setTextMML(op);
		else {
			err_no = a.setText(op);
// 			operacioMML->setText(a.textMML());
		}
		
		if(err_no==0)
			b = a.Calcula();
		
// 		kdDebug() << a.err << "::::::::::" << err_no << endl;
		//print_dom(a.elem);
		
		if(!a.err.isEmpty()){
			hist.prepend(QString("<p class=\"error\"> <b>ERROR:</b> %1 </p>").arg(a.err));
			ultim_error = true;
		} else {
			stream << "<p class=\"last\">\n";
			
			if(a.elem.firstChild().toElement().tagName()=="declare" || a.elem.firstChild().toElement().tagName()=="lambda") {
				hist.prepend(a.toString());
			} else {
				hist.prepend(QString("%1 <span class='sep'>=</span> <span class='num'>%1</span></p>\n").arg(a.toString()).arg(b, 0, 'g', 12));
				a.vars.modifica("ans", b);
			}
			
			ultim_error = false;
		}
		
		stream << hist << "</body></html>\n";
		file.close();
	}
	operacio->setText("");
	
	log->openURL("/tmp/kalgebra_log.xml");
	update_varlist();
}

void KAlgebra::saveLog(){
	QString path = KFileDialog::getSaveFileName(QString::null, "*.xml", this);
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
		stream << ".op { text-weight: bold; }\n";
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
		QListViewItem *klvi = new QListViewItem( varlist, *it, Analitza::treu_tags(a.str(a.vars.value(*it))).stripWhiteSpace());
		varlist->insertItem(klvi);
	}
}

void KAlgebra::imatge2d(){
	QString path = KFileDialog::getSaveFileName(QString::null, i18n("*.svg|SVG Image (*.svg)\n*.png|PNG Image (*.png)"), this);
	grafic->toImage(path);
}

/*void KAlgebra::imatge3dMML(){
	grafic3d->outputFuncMML(funcio3dMML->text(), path);
}*/

void KAlgebra::set_res_low() { grafic->setResolution(416); g2d_res_check(G2D_RES_LOW); }
void KAlgebra::set_res_std() { grafic->setResolution(832); g2d_res_check(G2D_RES_STD); }
void KAlgebra::set_res_fine() { grafic->setResolution(1664); g2d_res_check(G2D_RES_FINE); }
void KAlgebra::set_res_vfine(){ grafic->setResolution(3328); g2d_res_check(G2D_RES_VFINE); }

void KAlgebra::g2d_res_check(int id){
	for(int i=G2D_RES_LOW;i<=G2D_RES_VFINE; i++)
		res->setItemChecked(i, id==i);
}

void KAlgebra::g3d_type_check(int id){
	for(int i=G3D_TYPE_DOTTED;i<=G3D_TYPE_SOLID; i++)
		tipus3d->setItemChecked(i, id==i);
}

void KAlgebra::slot_3dpoints(){ grafic3d->setMethod(G_POINTS); g3d_type_check(G3D_TYPE_DOTTED); }
void KAlgebra::slot_3dlines() { grafic3d->setMethod(G_LINES);  g3d_type_check(G3D_TYPE_LINES);  }
void KAlgebra::slot_3dsolid() { grafic3d->setMethod(G_SOLID);  g3d_type_check(G3D_TYPE_SOLID);  }

void KAlgebra::slot_transparencia() {
	g3d->setItemChecked(G3D_TRANS, transparencia = !transparencia);
	grafic3d->setTraslucency(transparencia);
}

void KAlgebra::slot_getpixmap(){
	QString path = KFileDialog::getSaveFileName( QString::null, "*.png", this );
	if(!path.isEmpty())
		grafic3d->toPixmap().save(path, "PNG");
}

void KAlgebra::slot_editat(QListViewItem *it){
	if(it != NULL){
		grafic->setSelected(it->text(0));
		
		QListViewItemIterator it(func2dlist, QListViewItemIterator::NotChecked); //Looks ridiculous but I can't do better :S
		while ( it.current() ) {
			grafic->setShown(it.current()->text(0), false);
			++it;
		}
		
		it=QListViewItemIterator(func2dlist, QListViewItemIterator::Checked);
		while ( it.current() ) {
			grafic->setShown(it.current()->text(0), true);
			++it;
		}
	} else {
		grafic->setSelected("none");
	}
}

void KAlgebra::new_func(){
	KFunctionEdit *e = new KFunctionEdit(this) ;
	if(e->exec() == QDialog::Accepted && e->isCorrect()) {
		grafic->unselect();
		QPixmap p(15,15);
		p.fill(e->color());
		QCheckListItem *klvi = new QCheckListItem( func2dlist, e->text(), QCheckListItem::CheckBox);
		klvi->setPixmap(1, p);
		klvi->setState(QCheckListItem::On);
		bool inserted=grafic->addFunction(function(e->text(), e->color(), true));
		if(inserted)
			func2dlist->insertItem(klvi);
	}
	delete e;
}

void KAlgebra::edit_func(QListViewItem *item, const QPoint &,int) {
	KFunctionEdit *e = new KFunctionEdit(this) ;
	e->setText(item->text(0));
	e->setColor(item->pixmap(1)->convertToImage().pixel(2,2));
	if(e->exec() == QDialog::Accepted) {
		grafic->unselect();
		QPixmap p(15,15);
		p.fill(e->color());
		grafic->editFunction(item->text(0), function(e->text(), e->color(), true));
		item->setText(0, e->text());
		item->setPixmap(1, p);
	}
	delete e;
}

void KAlgebra::edit_var(QListViewItem *item, const QPoint &,int){
	KVarEdit *e = new KVarEdit(item->text(0), item->text(1), &a.vars, this) ;
	if(e->exec() == QDialog::Accepted ) { //Si s'ha acceptat
		a.vars.modifica(item->text(0), e->val());
	}
	delete e;
	update_varlist();
}

void KAlgebra::slot_togglesquares(){
	grafic->setSquares(!grafic->squares());
	g2d->setItemChecked(G2D_TOGGSQUARES, grafic->squares());
}

void KAlgebra::changeStatusBar(const QString& text)
{
	m_status->setText(text);
// 	statusBar()->message(text);
}

void KAlgebra::tabChanges(QWidget *newWid)
{
	if(newWid==consola) {
		operacio->setFocus();
		operacio->setCursorPosition(0,operacio->text().length());
	} else if(newWid==hgraf2d)
		addfunc->setFocus();
	else if(newWid==dibuix3d) {
		funcio3d->setFocus();
		funcio3d->setCursorPosition(0,funcio3d->text().length());
	} else
		qDebug("the new x-files");
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////    DCOP interfaces     ////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void KAlgebra::operate(QString operation){
	opera_gen(operation);
}

void KAlgebra::plot2D(QString operation){
	func2dlist->clear();
	grafic->clear();
	add2D(operation);
}

void KAlgebra::add2D(QString operation){
	QPixmap p(15,15);
	p.fill(QColor(0,150,0));
	QCheckListItem *klvi = new QCheckListItem(func2dlist, operation, QCheckListItem::CheckBox);
	klvi->setPixmap(1, p);
	klvi->setState(QCheckListItem::On);
	func2dlist->insertItem(klvi);
	func2dlist->setSelected(klvi, true);
	grafic->setSelected(operation);
	grafic->addFunction(function(operation, QColor(0,150,0), false));
}

QStringList KAlgebra::list2D(){
	QStringList a;
	QListViewItemIterator it(func2dlist, QListViewItemIterator::Checked);
	while ( it.current() ) {
		a << it.current()->text(0);
		++it;
	}
	return a;
}

void KAlgebra::remove2D(int){} //TODO: not implemented

void KAlgebra::plot3D(QString operation)
{
	if(funcio3d->isMathML())
		grafic3d->setFuncMML(funcio3d->text());
	else
		grafic3d->setFunc(funcio3d->text());
}

void KAlgebra::save2D(QString path)
{
	if(!path.isEmpty())
		grafic->toImage(path);
}


void KAlgebra::save3D(KURL path)
{
	if(!path.isEmpty())
		grafic3d->save(path);
}

QString KAlgebra::calculate(QString op) //This could be a nice example about how does it all work
{
	Analitza a;
	double res;
	if(QExpressionEdit::isMathML(op))
		a.setTextMML(op);
	else
		a.setText(op);
	
	if(a.err.isEmpty())
		res=a.Calcula();
	
	if(!a.err.isEmpty())
		return i18n("Error: %1").arg(Analitza::treu_tags(a.err));
	else
		return QString("%1").arg(res, 0, 'g', 12);
}

#include "kalgebra.moc"
