/****************************************************************************
** Meta object code from reading C++ file 'clientwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../clientwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ClientWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   14,   13,   13, 0x05,
      48,   37,   13,   13, 0x05,
      91,   77,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     114,   14,   13,   13, 0x0a,
     132,   37,   13,   13, 0x0a,
     163,   77,   13,   13, 0x0a,
     188,   13,   13,   13, 0x0a,
     217,   13,   13,   13, 0x0a,
     249,   13,   13,   13, 0x0a,
     279,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ClientWindow[] = {
    "ClientWindow\0\0socket\0sigConnect(int)\0"
    "socket,msg\0sigMessage(int,Net::Message)\0"
    "socket,remote\0sigDisconnect(int,int)\0"
    "slot_connect(int)\0slot_message(int,Net::Message)\0"
    "slot_disconnect(int,int)\0"
    "on_actionConnect_triggered()\0"
    "on_actionDisconnect_triggered()\0"
    "on_actionSettings_triggered()\0"
    "on_pushButton_clicked()\0"
};

void ClientWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ClientWindow *_t = static_cast<ClientWindow *>(_o);
        switch (_id) {
        case 0: _t->sigConnect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sigMessage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Net::Message(*)>(_a[2]))); break;
        case 2: _t->sigDisconnect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->slot_connect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slot_message((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Net::Message(*)>(_a[2]))); break;
        case 5: _t->slot_disconnect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->on_actionConnect_triggered(); break;
        case 7: _t->on_actionDisconnect_triggered(); break;
        case 8: _t->on_actionSettings_triggered(); break;
        case 9: _t->on_pushButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ClientWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ClientWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ClientWindow,
      qt_meta_data_ClientWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ClientWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ClientWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ClientWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClientWindow))
        return static_cast<void*>(const_cast< ClientWindow*>(this));
    if (!strcmp(_clname, "Net::Host"))
        return static_cast< Net::Host*>(const_cast< ClientWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ClientWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ClientWindow::sigConnect(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ClientWindow::sigMessage(int _t1, Net::Message _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ClientWindow::sigDisconnect(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
