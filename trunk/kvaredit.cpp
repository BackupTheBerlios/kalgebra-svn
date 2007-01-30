#include "analitza.h"
#include "kvaredit.h"

#include <QPushButton>
#include <QLabel>

KVarEdit::KVarEdit(QWidget *parent, bool modal) :
	QDialog (parent), vars(NULL), m_correct(false), m_var("x")
{
	this->setWindowTitle(i18n("Add/Edit a variable"));
	this->setModal(modal);
	this->setMinimumSize(200, 200);
	
	buttonBox = new QDialogButtonBox(this);
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	
	QVBoxLayout *topLayout = new QVBoxLayout(this);
	
	m_exp = new QExpressionEdit(this);
	connect(m_exp, SIGNAL(textChanged()), this, SLOT(edit()));
	connect(m_exp, SIGNAL(returnPressed()), this, SLOT(ok()));
	
	m_valid = new QLabel(this);
	
	QGroupBox *buttons = new QGroupBox(i18n("Mode"));
	
	m_opt_exp  = new QRadioButton(i18n("Save the expression"));
	m_opt_calc = new QRadioButton(i18n("Calculate the expression"));
	m_opt_calc->setChecked(true);
	
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_opt_calc);
	vbox->addWidget(m_opt_exp);
	buttons->setLayout(vbox);
	
	topLayout->addWidget(m_exp);
	topLayout->addWidget(m_valid);
	topLayout->addWidget(buttons);
	topLayout->addWidget(buttonBox);
	
	m_exp->setFocus();
}

KVarEdit::~KVarEdit(){}

void KVarEdit::setVar(const QString& newVar)
{
	m_var=newVar;
	this->setWindowTitle(i18n("Edit '%1' value").arg(newVar));
	if(vars==NULL)
		m_exp->setText("not available");
	else {
		if(m_exp->isMathML())
			m_exp->setText(vars->value(newVar)->toMathML());
		else
			m_exp->setText(vars->value(newVar)->toString());
	}
}

Object* KVarEdit::val()
{
	Analitza a;
	Object* ret;
	
	QString expres = m_exp->text();
	if(!m_exp->isMathML()) {
		QExp e(expres);
		e.parse();
		expres = e.mathML();
	}
	
	a.setTextMML(expres);
	
	if(m_opt_calc->isChecked())
		ret = new Cn(a.calculate());
	else
		ret = Analitza::objectCopy(a.m_tree);
	
	return ret;
}

void KVarEdit::edit()
{
	double val;
	Analitza *a = new Analitza;
	QString funct = m_exp->text();
	
	if(m_exp->text().isEmpty()) {
		m_exp->setCorrect(true);
		m_valid->setText(QString::null);
		return;
	}
	
	if(!m_exp->isMathML()) {
		QExp e(funct);
		e.parse();
		funct = e.mathML();
		if(e.error().isEmpty())
			a->setTextMML(funct);
		else
			a->m_err << i18n("From parser:") << e.error();
	} else
		a->setTextMML(funct);
	
	if(a->isCorrect()) {
		if(a->bvarList().count()>0)
			m_valid->setText(i18n("%1:=%2").arg(m_var).arg(m_exp->text()));
		else {
			val = a->calculate().value();
			
			if(!a->isCorrect()) {
				m_valid->setText(i18n("<b style='color:red'>WRONG</b>"));
				m_valid->setToolTip(a->m_err.join("\n"));
				m_correct=false;
			} else {
				m_valid->setText(i18n("<b style='color:#090'>%1 := %2</b>").arg(m_var).arg(val));
				m_valid->setToolTip(QString::null);
				m_correct=true;
			}
		}
	}
	
	m_exp->setCorrect(m_correct);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_correct);
}

void KVarEdit::ok(){
	if(m_correct)
		accept();
}
