#include "debug.h"

void treubug::print_dom(QDomNode in, int ind){
	QString a="";
	for(int i=0; i<ind; i++)
		a+="--------";
	qDebug("%s%s(%s)", a.ascii(), in.toElement().tagName().ascii(), in.toElement().text().ascii());
	if(in.childNodes().length()==1)
		return;
	for(unsigned int i=0 ; i<in.childNodes().length(); i++){
		print_dom(in.childNodes().item(i), ind+1);
	}
}
