#include "kfunctionedit.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

KFunctionEdit::KFunctionEdit(QWidget *parent, bool modal, Qt::WFlags) :
		QDialog (parent), m_correct(false)
{
	this->setWindowTitle(i18n("Add/Edit a function"));
	this->setModal(modal);
	buttonBox = new QDialogButtonBox(this);
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	this->setFixedWidth(400);
	this->setFixedHeight(350);
	
	QVBoxLayout *topLayout = new QVBoxLayout(this);
	topLayout->setMargin(10);
	topLayout->setSpacing(5);
	m_func = new QExpressionEdit(this);
	m_func->setAns("x");
	
	connect(m_func, SIGNAL(textChanged()), this, SLOT(edit()));
	connect(m_func, SIGNAL(returnPressed()), this, SLOT(ok()));
	
	m_color = new ColorCombo(this);
	m_color->setColor(QColor(0,150,0));
	connect(m_color, SIGNAL(currentIndexChanged(int)), this, SLOT(colorChange(int)));
	
	m_valid = new QLabel(this);
	
	m_graph = new QGraph(this);
	m_graph->setViewPort(QRect(QPoint(-9, 5), QPoint(9, -5)));
	m_graph->setResolution(400);
	m_graph->setFocusPolicy(Qt::NoFocus);
	m_graph->addFunction(function(m_func->text(), m_color->color(), true));
	m_graph->setMouseTracking(false);
	m_graph->setFramed(true);
	m_graph->setReadOnly(true);
	
	topLayout->addWidget(m_func);
	topLayout->addWidget(m_color);
	topLayout->addWidget(m_valid);
	topLayout->addWidget(m_graph);
	topLayout->addWidget(buttonBox);
	
	m_func->setFocus();
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

KFunctionEdit::~KFunctionEdit(){}

void KFunctionEdit::setText(const QString &newText) {
	m_func->setText(newText);
	m_func->document()->setModified(true);
}

void KFunctionEdit::setColor(const QColor &newColor)
{
	m_color->setColor(newColor);
	m_graph->editFunction(0)->setColor(newColor);
	m_graph->forceRepaint();
}

void KFunctionEdit::colorChange(int)
{
	setColor(m_color->color());
}

void KFunctionEdit::edit()	//Let's see if the exp is correct
{
	Analitza *a = new Analitza;
	bool err_num;
	
	QString funct = m_func->text();
	
	if(!m_func->isMathML()) {
		QExp e(funct);
		e.parse();
		funct = e.mathML();
	}
	
	a->setTextMML(funct);
	
	if(a->isCorrect()) {
		QString var = a->bvarList().count()==0 ? "x" : a->bvarList()[0];
		a->m_vars->modify(var, 0.);
		m_valid->setText(QString("<b style='color:#090'>f:=%1->%2</b>").arg(var).arg(a->toString()));
		a->calculate();
	}
	
	if(!a->isCorrect()){
		m_valid->setText(i18n("<b style='color:red'>WRONG</b>"));
		m_valid->setToolTip(a->m_err.join("\n"));
		m_correct=false;
	} else {
		m_valid->setToolTip(QString::null);
		m_graph->editFunction(0, function(m_func->toPlainText(), m_color->color(), true));
		m_correct=true;
	}
	
	m_func->setCorrect(m_correct);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_correct);
	
	delete a;
}

void KFunctionEdit::ok(){
	if(m_correct)
		accept();
}

///////////////////////////////////////

ColorCombo::ColorCombo(QWidget* parent) : QComboBox(parent)
{
	setIconSize(QSize(width()*10, height()));
	
	setColor(Qt::green);
	setColor(Qt::black);
	setColor(Qt::blue);
	setColor(Qt::red);
	setColor(QColor(0,0,2,0x7f));
}

void ColorCombo::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	QSize s = event->size();
	s-=QSize(30, 7);
	setIconSize(s);
}

void ColorCombo::setColor(const QColor &color)
{
	int pos = findData(color.name());
	if(pos==-1) {
		QPixmap p(iconSize());
		p.fill(color);
		addItem(p, QString::null, color.name());
		setCurrentIndex(count()-1);
	} else {
		setCurrentIndex(pos);
	}
}

QColor ColorCombo::color() const
{
	return QColor(itemData(currentIndex()).toString());
}
