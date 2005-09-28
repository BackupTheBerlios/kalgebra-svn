#ifndef VARIABLES_H
#define VARIABLES_H
#include <qvaluelist.h>
#include <qstringlist.h>
#include <qdom.h>

/**
@author Aleix Pol,,,
*/
struct VARIABLE{
	QString nom;
	QDomElement valor;
};

class Variables{
	private:
		QValueList<struct VARIABLE> vars;
	public:
		Variables();
		void afegeix(QString id, QDomElement val);
		void treu(QString id);
		void modifica(QString id, QDomElement new_val);
		void modifica(QString id, double new_val);
		QDomElement value(QString id);
		QStringList getNoms();
		bool isvar(QDomElement e);
};

#endif
