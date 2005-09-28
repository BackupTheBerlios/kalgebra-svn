

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "analitza.h"
#include <qdatetime.h>
#include "debug.h"


int main(){
	QTime t;
	Analitza a;
	char *s;
	s= (char*) malloc(500);
	while(true){
		s=gets(s);
		a.setText(s);
		treubug::print_dom(a.elem);
		qDebug(".....||||||||.....");
		t.restart();
		treubug::print_dom(a.Calcula());
		qDebug( "                                     Lasts: %d ms", t.elapsed() );
	}
}
