#include "variables.h"

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////                                                   /////////////////////
/////////////                   Classe Variables                /////////////////////
/////////////                                                   /////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

Variables::Variables(){
// 	afegeix("x", 33.0f);
	modifica("true", 1.0f);
	modifica("false", 0.0f);
	modifica("pi", 3.141592653f);
	modifica("e", 2.718281828f);
	modifica("euler", 0.5772156649f);
	modifica("º", 3.141592653f/180.);
}

inline void Variables::afegeix(QString id, QDomElement val){
	modifica(id,val);
}

void Variables::treu(QString id){
	QValueList<struct VARIABLE>::iterator it;
	struct VARIABLE aux;
	it = vars.begin() ;
	for(; it != vars.end(); ++it){ 
		aux = *it;
		if(aux.nom == id){
			vars.remove(it);
			break;
		}
	}
}

void Variables::modifica(QString id, double new_val){
	QString z = QString( "<cn> %1 </cn>" ).arg(new_val, 0, 'g', 16);
	QDomDocument a;
	QString err = "<cn>" + z + "</cn>";
	a.setContent(err);
	modifica(id,a.documentElement());
}

void Variables::modifica(QString id, QDomElement new_val){
	struct VARIABLE aux;
	
	QValueList<struct VARIABLE>::iterator it;
	for(it = vars.begin(); it != vars.end(); ++it){ 
		aux = *it;
		if(aux.nom == id){
			vars.remove(it);
			break;
		}
	}
	aux.nom=id;
	aux.valor=new_val;
	vars.append(aux);
}
QDomElement Variables::value(QString id){
	struct VARIABLE aux;
	QValueList<struct VARIABLE>::iterator it;
	
	for(it = vars.begin(); it != vars.end(); ++it){ 
		aux = *it;
		if(aux.nom.stripWhiteSpace() == id.stripWhiteSpace()){
// 			cout << "-->" << aux.valor.toDocument().toString().ascii() << endl;
			return aux.valor;
		}
	}
	//error
	QDomDocument a; QString err("<cn \"constant\"> &NaN </cn>");
	a.setContent(err);
	return a.documentElement();
}

QStringList Variables::getNoms(){
	struct VARIABLE aux;
	QStringList out;
	QValueList<struct VARIABLE>::iterator it;
	for(it = vars.begin(); it != vars.end(); ++it){ 
		aux = *it;
		out << aux.nom;
	}
	return out;
}

bool Variables::isvar(QDomElement e){
	if(e.tagName()=="ci"){
// 		struct VARIABLE aux;
		QStringList out;
		QValueList<struct VARIABLE>::iterator it;
		for(it = vars.begin(); it != vars.end(); ++it){ 
			if((*it).nom=="")
				return true;
		}
	}
	return false;
}
