/****************************************************************************
** Meta object code from reading C++ file 'console.h'
**
** Created: Tue Oct 17 01:11:06 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "console.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'console.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Console[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      13,    9,    8,    8, 0x05,
      29,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      54,   44,   39,    8, 0x0a,
      84,   81,   39,    8, 0x0a,
     111,  106,   39,    8, 0x0a,
     131,  106,   39,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Console[] = {
    "Console\0\0msg\0status(QString)\0changed()\0bool\0op,mathml\0"
    "addOperation(QString,bool)\0op\0addOperation(QString)\0path\0"
    "loadScript(QString)\0saveLog(QString)\0"
};

const QMetaObject Console::staticMetaObject = {
    { &QListWidget::staticMetaObject, qt_meta_stringdata_Console,
      qt_meta_data_Console, 0 }
};

const QMetaObject *Console::metaObject() const
{
    return &staticMetaObject;
}

void *Console::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Console))
	return static_cast<void*>(const_cast<Console*>(this));
    return QListWidget::qt_metacast(_clname);
}

int Console::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: status((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: changed(); break;
        case 2: { bool _r = addOperation((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = addOperation((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = loadScript((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = saveLog((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Console::status(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Console::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_VariableView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_VariableView[] = {
    "VariableView\0\0updateVariables()\0"
};

const QMetaObject VariableView::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_VariableView,
      qt_meta_data_VariableView, 0 }
};

const QMetaObject *VariableView::metaObject() const
{
    return &staticMetaObject;
}

void *VariableView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VariableView))
	return static_cast<void*>(const_cast<VariableView*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int VariableView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateVariables(); break;
        }
        _id -= 1;
    }
    return _id;
}
