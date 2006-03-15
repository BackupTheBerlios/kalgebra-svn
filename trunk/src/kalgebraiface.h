#ifndef KALGEBRAIFACE_H
#define KALGEBRAIFACE_H

#include <dcopobject.h>
#include <qstringlist.h>

/**
@author Aleix Pol i Gonzalez
*/
class KAlgebraIface : virtual public DCOPObject
{
	K_DCOP
k_dcop:
	virtual void calculate(QString operation) = 0;
	virtual void plot2D(QString operation) = 0;
	virtual void add2D(QString operation) = 0;
	virtual QStringList list2D() = 0;
	virtual void remove2D(int n) =0;
	virtual void plot3D(QString operation) = 0;
};

#endif
