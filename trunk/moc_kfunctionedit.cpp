/****************************************************************************
** Meta object code from reading C++ file 'kfunctionedit.h'
**
** Created: Tue Oct 17 01:11:07 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "kfunctionedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kfunctionedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_ColorCombo[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_ColorCombo[] = {
    "ColorCombo\0"
};

const QMetaObject ColorCombo::staticMetaObject = {
    { &QComboBox::staticMetaObject, qt_meta_stringdata_ColorCombo,
      qt_meta_data_ColorCombo, 0 }
};

const QMetaObject *ColorCombo::metaObject() const
{
    return &staticMetaObject;
}

void *ColorCombo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ColorCombo))
	return static_cast<void*>(const_cast<ColorCombo*>(this));
    return QComboBox::qt_metacast(_clname);
}

int ColorCombo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QComboBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_KFunctionEdit[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      24,   14,   14,   14, 0x08,
      31,   14,   14,   14, 0x08,
      36,   14,   14,   14, 0x08,
      53,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KFunctionEdit[] = {
    "KFunctionEdit\0\0accept()\0edit()\0ok()\0colorChange(int)\0clear()\0"
};

const QMetaObject KFunctionEdit::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_KFunctionEdit,
      qt_meta_data_KFunctionEdit, 0 }
};

const QMetaObject *KFunctionEdit::metaObject() const
{
    return &staticMetaObject;
}

void *KFunctionEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KFunctionEdit))
	return static_cast<void*>(const_cast<KFunctionEdit*>(this));
    return QWidget::qt_metacast(_clname);
}

int KFunctionEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: accept(); break;
        case 1: edit(); break;
        case 2: ok(); break;
        case 3: colorChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: clear(); break;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void KFunctionEdit::accept()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
