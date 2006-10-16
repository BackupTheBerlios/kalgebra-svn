#ifndef QDOUBLEPOINT_H
#define QDOUBLEPOINT_H

/**
@author Aleix Pol i Gonzalez
*/


class QDoublePoint {
public:
	QDoublePoint() { m_x=0.0; m_y=0.0; }
	QDoublePoint(const QDoublePoint &p) { m_x = p.x(); m_y=p.y(); }
	QDoublePoint(double newX,double newY) { m_x=newX; m_y=newY; }
// 	QDoublePoint(QPoint p) { m_x=p.x(); m_y=p.y(); }
	~QDoublePoint() {}
	void setX(double newX) { m_x=newX; }
	void setY(double newY) { m_y=newY; }
	void setXY(double newX,double newY) { m_x=newX; m_y=newY; }
	void setXY(int newX, int newY) { m_x=newX; m_y=newY; }
	double x() const { return m_x; }
	double y() const { return m_y; }
private:
	double m_x;
	double m_y;
};

#endif
