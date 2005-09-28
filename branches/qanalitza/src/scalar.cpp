#include "scalar.h"

Scalar::~Scalar(){}

QDomElement Scalar::value(QDomDocument d){
	QDomElement e;
	if(m_boolean){
		if(m_value==0.0) e=d.createElement("false"); else e=d.createElement("true");
	} else {
		e=d.createElement("cn");
		e.appendChild(d.createTextNode(QString("%1").arg(m_value, 0, 'g', 16)));
	}
	return e;
}

void Scalar::setValue(const QDomElement& val){
	m_value=0.0;
	bool err;
// 	cout << "<" << val.tagName() << ">" << val.text() << "</" << val.tagName() << ">" << endl;
	/*if(val.tagName()=="apply")
		m_value = setValue(evalua(val));
	else*/ if(val.tagName() == "cn"){ // a es un nombre
		if(val.attribute("type","real") == "real"){
			m_value= val.text().stripWhiteSpace().toDouble(&err); //TODO: Base on double not implemented
		} else if(val.attribute("type") == "e-notation"){
			//TODO: Not implemented
		} else if(val.attribute("type") == "integer"){
			int aux = val.text().stripWhiteSpace().toInt(&err, val.attribute("base", "10").toInt(NULL, 10));
			m_value = (double) aux;
		}
		else if(val.attribute("type") == "rational"){
			//TODO: Not implemented
		}
		else if(val.attribute("type") == "complex-cartesian"){
			//TODO: Not implemented
		}
		else if(val.attribute("type") == "complex-polar"){
			//TODO: Not implemented
		}
		else if(val.attribute("type") == "constant"){
			if(val.text() == "&pi;"){
				m_value = 3.141592653f;
			} else if (val.text() == "&ee;" || val.text() == "&ExponentialE;"){
				m_value = 2.718281828f;
			}  else if (val.text() == "&ImagniaryI;"){
				//TODO: Not implemented
			} else if (val.text() == "&gamma;"){
				m_value = 0.5772156649f;
			} else if (val.text() == "&infin;"){
				//TODO: Not implemented
			} else if (val.text() == "&NaN;"){
				//TODO: Not implemented
			} else if (val.text() == "&true;"){
				m_value = 1.;
				m_boolean = true;
			} else if (val.text() == "&false;"){
				m_value = 0.;
				m_boolean = true;
			}
		}
	} //else if(val.tagName() == "ci") //si a es variable
		//setValue(vars->value(val.text()));
	else if(val.tagName()=="true"){
		m_value = 1.0;
		m_boolean = true;
	}else if(val.tagName()=="false"){
		m_value = 0.0;
		m_boolean = true;
	}else if(val.tagName()=="pi")
		m_value = 3.141592653f;
	else if(val.tagName()=="exponentiale")
		m_value = 2.718281828f;
	else if(val.tagName()=="eulergamma")
		m_value = 0.5772156649f;
// 	printf("\n---->%f\n", m_value);
}
