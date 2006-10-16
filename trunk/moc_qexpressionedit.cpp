/****************************************************************************
** Meta object code from reading C++ file 'qexpressionedit.h'
**
** Created: Tue Oct 17 01:11:11 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qexpressionedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qexpressionedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QExpressionEdit[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      39,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   16,   16,   16, 0x0a,
      65,   16,   16,   16, 0x0a,
      77,   16,   16,   16, 0x0a,
      94,   16,   16,   16, 0x0a,
     113,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QExpressionEdit[] = {
    "QExpressionEdit\0\0signalHelper(QString)\0returnPressed()\0returnP()\0"
    "cursorMov()\0ajudant(QString)\0completed(QString)\0updateCompleter()\0"
};

const QMetaObject QExpressionEdit::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_QExpressionEdit,
      qt_meta_data_QExpressionEdit, 0 }
};

const QMetaObject *QExpressionEdit::metaObject() const
{
    return &staticMetaObject;
}

void *QExpressionEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QExpressionEdit))
	return static_cast<void*>(const_cast<QExpressionEdit*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int QExpressionEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalHelper((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: returnPressed(); break;
        case 2: returnP(); break;
        case 3: cursorMov(); break;
        case 4: ajudant((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: completed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: updateCompleter(); break;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QExpressionEdit::signalHelper(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QExpressionEdit::returnPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
