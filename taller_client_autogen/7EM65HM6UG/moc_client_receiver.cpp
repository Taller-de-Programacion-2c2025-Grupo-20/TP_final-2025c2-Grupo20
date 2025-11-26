/****************************************************************************
** Meta object code from reading C++ file 'client_receiver.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../client/client_receiver.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client_receiver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_ClientReceiver_t {
    uint offsetsAndSizes[14];
    char stringdata0[15];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[10];
    char stringdata5[12];
    char stringdata6[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ClientReceiver_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ClientReceiver_t qt_meta_stringdata_ClientReceiver = {
    {
        QT_MOC_LITERAL(0, 14),  // "ClientReceiver"
        QT_MOC_LITERAL(15, 12),  // "loginSuccess"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 7),  // "uint8_t"
        QT_MOC_LITERAL(37, 9),  // "player_id"
        QT_MOC_LITERAL(47, 11),  // "loginFailed"
        QT_MOC_LITERAL(59, 11)   // "gameStarted"
    },
    "ClientReceiver",
    "loginSuccess",
    "",
    "uint8_t",
    "player_id",
    "loginFailed",
    "gameStarted"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ClientReceiver[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,
       5,    0,   35,    2, 0x06,    3 /* Public */,
       6,    0,   36,    2, 0x06,    4 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ClientReceiver::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ClientReceiver.offsetsAndSizes,
    qt_meta_data_ClientReceiver,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ClientReceiver_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ClientReceiver, std::true_type>,
        // method 'loginSuccess'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint8_t, std::false_type>,
        // method 'loginFailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'gameStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ClientReceiver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClientReceiver *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->loginSuccess((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1]))); break;
        case 1: _t->loginFailed(); break;
        case 2: _t->gameStarted(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ClientReceiver::*)(uint8_t );
            if (_t _q_method = &ClientReceiver::loginSuccess; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ClientReceiver::*)();
            if (_t _q_method = &ClientReceiver::loginFailed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ClientReceiver::*)();
            if (_t _q_method = &ClientReceiver::gameStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *ClientReceiver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClientReceiver::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClientReceiver.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Thread"))
        return static_cast< Thread*>(this);
    return QObject::qt_metacast(_clname);
}

int ClientReceiver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ClientReceiver::loginSuccess(uint8_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ClientReceiver::loginFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ClientReceiver::gameStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
