/****************************************************************************
** Meta object code from reading C++ file 'algebra.h'
**
** Created: Tue Oct 17 01:11:04 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "algebra.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'algebra.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QAlgebra[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      25,   20,    9,    9, 0x0a,
      50,    9,    9,    9, 0x0a,
      63,    9,    9,    9, 0x0a,
      73,    9,    9,    9, 0x0a,
      83,    9,    9,    9, 0x0a,
      94,    9,    9,    9, 0x0a,
     117,    9,    9,    9, 0x0a,
     148,  139,    9,    9, 0x0a,
     189,    9,    9,    9, 0x0a,
     205,    9,    9,    9, 0x0a,
     219,    9,    9,    9, 0x0a,
     233,    9,    9,    9, 0x0a,
     248,    9,    9,    9, 0x0a,
     276,  264,    9,    9, 0x0a,
     308,    9,    9,    9, 0x0a,
     320,    9,    9,    9, 0x0a,
     335,    9,    9,    9, 0x0a,
     360,    9,    9,    9, 0x0a,
     376,    9,    9,    9, 0x0a,
     387,    9,    9,    9, 0x0a,
     399,    9,    9,    9, 0x0a,
     411,    9,    9,    9, 0x0a,
     432,    9,    9,    9, 0x0a,
     446,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QAlgebra[] = {
    "QAlgebra\0\0operate()\0item\0insert(QListWidgetItem*)\0loadScript()\0"
    "saveLog()\0afegeix()\0new_func()\0edit_func(QModelIndex)\0"
    "edit_var(QModelIndex)\0item,ant\0"
    "canvi(QTreeWidgetItem*,QTreeWidgetItem*)\0toggleSquares()\0"
    "set_res_low()\0set_res_std()\0set_res_fine()\0set_res_vfine()\0"
    "item,column\0different(QTreeWidgetItem*,int)\0saveGraph()\0"
    "functools(int)\0changeStatusBar(QString)\0tabChanged(int)\0set_dots()\0"
    "set_lines()\0set_solid()\0toggleTransparency()\0save3DGraph()\0about()\0"
};

const QMetaObject QAlgebra::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QAlgebra,
      qt_meta_data_QAlgebra, 0 }
};

const QMetaObject *QAlgebra::metaObject() const
{
    return &staticMetaObject;
}

void *QAlgebra::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAlgebra))
	return static_cast<void*>(const_cast<QAlgebra*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QAlgebra::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: operate(); break;
        case 1: insert((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 2: loadScript(); break;
        case 3: saveLog(); break;
        case 4: afegeix(); break;
        case 5: new_func(); break;
        case 6: edit_func((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 7: edit_var((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 8: canvi((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 9: toggleSquares(); break;
        case 10: set_res_low(); break;
        case 11: set_res_std(); break;
        case 12: set_res_fine(); break;
        case 13: set_res_vfine(); break;
        case 14: different((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: saveGraph(); break;
        case 16: functools((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: changeStatusBar((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: tabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: set_dots(); break;
        case 20: set_lines(); break;
        case 21: set_solid(); break;
        case 22: toggleTransparency(); break;
        case 23: save3DGraph(); break;
        case 24: about(); break;
        }
        _id -= 25;
    }
    return _id;
}
