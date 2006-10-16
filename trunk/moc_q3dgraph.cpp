/****************************************************************************
** Meta object code from reading C++ file 'q3dgraph.h'
**
** Created: Tue Oct 17 01:11:10 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "q3dgraph.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'q3dgraph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Q3DGraph[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      14,   10,    9,    9, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Q3DGraph[] = {
    "Q3DGraph\0\0msg\0status(QString)\0"
};

const QMetaObject Q3DGraph::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_Q3DGraph,
      qt_meta_data_Q3DGraph, 0 }
};

const QMetaObject *Q3DGraph::metaObject() const
{
    return &staticMetaObject;
}

void *Q3DGraph::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q3DGraph))
	return static_cast<void*>(const_cast<Q3DGraph*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int Q3DGraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: status((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Q3DGraph::status(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
