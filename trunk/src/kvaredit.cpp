#include "kvaredit.h"

KVarEdit::KVarEdit(QWidget *parent, const char *name, bool modal, WFlags) : KDialogBase (parent, name, modal, i18n("Add/Edit a function"), Ok|Cancel, Ok, false) {
	QWidget *page = new QWidget( this ); //Akets no cal k ls toquis em sembla, no passa res
	setMainWidget(page);
	page->setFixedWidth(400);
	page->setFixedHeight(300);
	setFixedWidth(400);
	setFixedHeight(300);
	
	QVBoxLayout *topLayout = new QVBoxLayout( page, 0, spacingHint() ); //topLayout serà el Layout principal, ho necessites per colocar les coses de manera logica
	
	m_exp = new QLineEdit( "2+2", page, "expression"); //Creem m_exp
	topLayout->addWidget(m_exp); //afegim m_exp al layout
	connect(m_exp, SIGNAL(textChanged ( const QString & )), this, SLOT(edit())); //Ho connectem al SLOT edit()
	
	//Aqui posa els optionbox, tho deixo a tu jejeje
	
}

KVarEdit::~KVarEdit(){}

void KVarEdit::edit(){
	//Quan hi ha un canvi a m_exp es crida aixo
}

#include "kvaredit.moc"
