/****************************************************************************
** Meta object code from reading C++ file 'qgraph.h'
**
** Created: Tue Oct 17 01:11:12 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qgraph.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgraph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QGraph[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      12,    8,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QGraph[] = {
    "QGraph\0\0msg\0status(QString)\0forceRepaint()\0"
};

const QMetaObject QGraph::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QGraph,
      qt_meta_data_QGraph, 0 }
};

const QMetaObject *QGraph::metaObject() const
{
    return &staticMetaObject;
}

void *QGraph::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QGraph))
	return static_cast<void*>(const_cast<QGraph*>(this));
    return QWidget::qt_metacast(_clname);
}

int QGraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: status((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: forceRepaint(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QGraph::status(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}