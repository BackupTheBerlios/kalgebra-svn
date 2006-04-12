#include "kvaredit.h"

KVarEdit::KVarEdit(QString var, QString exp, Variables *v, QWidget *parent, const char *name, bool modal, WFlags) : KDialogBase (parent, name, modal, i18n("Add/Edit a variable"), Ok|Cancel, Ok, false) {
	QWidget *page = new QWidget( this ); //Akets no cal k ls toquis em sembla, no passa res
	setMainWidget(page);
	/*page->setFixedWidth(400);
	page->setFixedHeight(300);
	setFixedWidth(400);*/
	setMinimumWidth(400);
	
	QVBoxLayout *topLayout = new QVBoxLayout(page, 0, spacingHint());
	
	m_exp = new QExpressionEdit(page);
	topLayout->addWidget(m_exp);
	connect(m_exp, SIGNAL(modificationChanged( bool )), this, SLOT(edit(bool)));
	
	m_valid = new QLabel(page);
	topLayout->addWidget(m_valid);
	
	QButtonGroup *buttons = new QButtonGroup(page);
	buttons->setColumnLayout(0, Qt::Vertical);
	QVBoxLayout *buttonsLayout = new QVBoxLayout( buttons->layout() );
	buttonsLayout->setAlignment( Qt::AlignTop );
	
	m_opt_calc = new QRadioButton(i18n("Calculate the expression"), buttons);
	m_opt_exp  = new QRadioButton(i18n("Save the expression"), buttons);
	buttonsLayout->addWidget(m_opt_calc);
	buttonsLayout->addWidget(m_opt_exp);
	m_opt_calc->setChecked(true);
	
	topLayout->addWidget(buttons);
	
	setVar(var);
	setExpression(exp);
	
	setCaption(i18n("Edit '%1' value").arg(var));
	
	vars=v;
	edit(true);
}

KVarEdit::~KVarEdit(){}

void KVarEdit::setExpression(QString newExp)	{ m_exp->setText(newExp); }

void KVarEdit::setVar(QString newVar)		{ qDebug("Kiko desperta: %s", newVar.ascii()); }

//bool KVarEdit::saveCalc()			{ return m_opt_calc->isChecked(); }

QDomElement KVarEdit::val() {
	Analitza a;
	QDomElement ret;
	
	if(m_exp->isMathML())
		a.setTextMML(m_exp->text());
	else
		a.setText(m_exp->text());
	
	if(m_opt_calc->isChecked()) {
		QDomDocument doc;
		ret=doc.createElement("cn");
		ret.appendChild(doc.createTextNode(QString("%1").arg(a.Calcula(), 0, 'g', 16)));
	} else {
		print_dom(a.elem);
		ret = a.elem.toElement();
	}
	return ret;
}

void KVarEdit::edit(bool){
	int err_num;
	double val;
		
	if(vars==NULL) return;
	
	Analitza a;
	a.setVars(*vars);
	
	if(m_exp->isMathML()) {
		err_num= a.setTextMML(m_exp->text());
		m_exp->setEnabled(false);
	} else {
		err_num=a.setText(m_exp->text());
		m_exp->setEnabled(true);
	}
	
	if(err_num==0) {
		if(a.lambda().count()>0) {
			a.vars.modifica(a.lambda()[0], 0.);
			m_valid->setText(i18n("<qt text='#090'><b>f(%1)=%2</b></qt>").arg(a.lambda()[0]).arg(Analitza::treu_tags(a.toString())));
		}
		
		val = a.Calcula();
		if(a.err != "")
			err_num=-1;
	}
	
	if(err_num){
		m_valid->setText(i18n("<qt text='red'><b>WRONG</b></qt>"));
		QToolTip::add(m_valid, a.err);
		m_correct=false;
	} else {
		m_valid->setText(i18n("<qt text='#090'><b>Result: %1</b></qt>").arg(val));
		QToolTip::remove(m_valid);
		m_correct=true;
	}
	
	m_exp->setModified(false);
	this->enableButtonOK(m_correct);
}

#include "kvaredit.moc"
