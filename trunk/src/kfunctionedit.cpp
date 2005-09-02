#include "kfunctionedit.h"

KFunctionEdit::KFunctionEdit(QWidget *parent, const char *name, bool modal, WFlags) : KDialogBase (parent, name, modal, i18n("Add/Edit a function"), Ok|Apply|Cancel, Ok, false) {
	QWidget *page = new QWidget( this );
	setMainWidget(page);
	setMaximumWidth(300);
	setMinimumWidth(300);
	setMaximumHeight(400);
	setMinimumHeight(400);
	
	QVBoxLayout *topLayout = new QVBoxLayout( page, 0, spacingHint() );

	QLabel *label = new QLabel(i18n("f(x)="), page);
	topLayout->addWidget( label );

	m_func = new KLineEdit( "x^sin(x)", page, "lineedit" );
	connect(m_func, SIGNAL(textChanged ( const QString & )), this, SLOT(edit()));
	topLayout->addWidget(m_func);

	m_color = new KColorCombo(page);
	m_color->setColor(QColor(0,150,0));
// 	m_color->setColor(QColor(255,0,0));
	topLayout->addWidget(m_color);
	
	m_valid = new QLabel("<qt text='red'>asdasda<b>GOGOGO</b></qt>", page);
	topLayout->addWidget(m_valid);
	
	m_graph = new QGraph(page);
	m_graph->setResolution(5);
	m_graph->setMouseTracking(false);
	topLayout->addWidget(m_graph);
	
	edit();
	m_func->setFocus();
	topLayout->addStretch(10);
}

KFunctionEdit::~KFunctionEdit(){}

void KFunctionEdit::edit(){
	//Let's see if the exp is correct
	QExp a(m_func->text());
	a.parse();
	if(a.error() != ""){
		m_valid->setText(i18n("<qt text='red'><b>WRONG</b></qt>"));
	} else {
		m_valid->setText(i18n("<qt text='green'>GOOD</qt>"));
		QValueList<QColor> col;
		col << m_color->color();
		m_graph->setFunc(a.mathML(), col);
	}
//	mmlexp=a.mathML();
}


#include "kfunctionedit.moc"
