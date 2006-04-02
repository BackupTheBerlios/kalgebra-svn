#include "kfunctionedit.h"

KFunctionEdit::KFunctionEdit(QWidget *parent, const char *name, bool modal, WFlags) : KDialogBase (parent, name, modal, i18n("Add/Edit a function"), Ok|Cancel, Ok, false) {
	QWidget *page = new QWidget( this );
	setMainWidget(page);
	page->setFixedWidth(400);
	page->setFixedHeight(300);
	setFixedWidth(400);
	setFixedHeight(300);
	
	QVBoxLayout *topLayout = new QVBoxLayout( page, 0, spacingHint() );
	
	m_func = new QExpressionEdit(page);
	m_func->setText("x**sin(x)");
	
	connect(m_func, SIGNAL(modificationChanged( bool )), this, SLOT(edit(bool)));
	connect(m_func, SIGNAL(returnPressed()), this, SLOT(ok()));
	topLayout->addWidget(m_func);
	
	m_color = new KColorCombo(page);
	m_color->setColor(QColor(0,150,0));
	topLayout->addWidget(m_color);
	
	m_valid = new QLabel("<qt text='red'></qt>", page);
	topLayout->addWidget(m_valid);
	
	QHBox *raro = new QHBox(page);
	raro->setFrameStyle( QFrame::Box | QFrame::Plain );
	raro->setLineWidth(1);
	topLayout->addWidget(raro);
	
	m_graph = new QGraph(raro);
	m_graph->setViewPort(QRect::QRect(QPoint::QPoint(-9, 5), QPoint::QPoint(9, -5)));
	m_graph->setResolution(400);
	m_graph->setMouseTracking(false);
	m_graph->setFocusPolicy(NoFocus);
	m_graph->addFunction(function(m_func->text(), m_color->color(), true));
	
	edit(false);
	m_func->setFocus();
	m_correct=true;
}

KFunctionEdit::~KFunctionEdit(){}

void KFunctionEdit::edit(bool){	//Let's see if the exp is correct
	Analitza a;
	int err_num;
	
	if(m_func->isMathML()) {
		err_num= a.setTextMML(m_func->text());
		m_func->setEnabled(false);
	} else {
		err_num=a.setText(m_func->text());
		m_func->setEnabled(true);
	}
	
	if(err_num==0) {
		if(a.lambda().count()>0) {
			a.vars.modifica(a.lambda()[0], 0.);
			m_valid->setText(i18n("<qt text='#090'><b>f(%1)=%2</b></qt>").arg(a.lambda()[0]).arg(Analitza::treu_tags(a.toString())));
		} else {
			a.vars.modifica("x", 0.);
			m_valid->setText(i18n("<qt text='#090'><b>f(x)=%1</b></qt>").arg(Analitza::treu_tags(a.toString())));
		}
		a.Calcula();
		if(a.err != "")
			err_num=-1;
	}
	
	if(err_num){
		m_valid->setText(i18n("<qt text='red'><b>WRONG</b></qt>"));
		QToolTip::add(m_valid, a.err);
		m_correct=false;
	} else {
		QToolTip::remove(m_valid);
		m_graph->editFunction(0, function(m_func->text(), m_color->color(), true));
		m_correct=true;
	}
	m_func->setModified(false);
	this->enableButtonOK(m_correct);
}

#include "kfunctionedit.moc"
