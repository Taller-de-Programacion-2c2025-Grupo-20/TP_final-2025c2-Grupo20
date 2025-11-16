/****************************************************************************
** Meta object code from reading C++ file 'lobbyscreen.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../client/Pantallas/lobbyscreen.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lobbyscreen.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_LobbyScreen_t {
    uint offsetsAndSizes[16];
    char stringdata0[12];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[7];
    char stringdata5[17];
    char stringdata6[23];
    char stringdata7[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_LobbyScreen_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_LobbyScreen_t qt_meta_stringdata_LobbyScreen = {
    {
        QT_MOC_LITERAL(0, 11),  // "LobbyScreen"
        QT_MOC_LITERAL(12, 9),  // "startGame"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 7),  // "Client*"
        QT_MOC_LITERAL(31, 6),  // "client"
        QT_MOC_LITERAL(38, 16),  // "updateLobbyState"
        QT_MOC_LITERAL(55, 22),  // "on_readyButton_clicked"
        QT_MOC_LITERAL(78, 22)   // "on_startButton_clicked"
    },
    "LobbyScreen",
    "startGame",
    "",
    "Client*",
    "client",
    "updateLobbyState",
    "on_readyButton_clicked",
    "on_startButton_clicked"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_LobbyScreen[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   41,    2, 0x08,    3 /* Private */,
       6,    0,   42,    2, 0x08,    4 /* Private */,
       7,    0,   43,    2, 0x08,    5 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject LobbyScreen::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_LobbyScreen.offsetsAndSizes,
    qt_meta_data_LobbyScreen,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_LobbyScreen_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LobbyScreen, std::true_type>,
        // method 'startGame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Client *, std::false_type>,
        // method 'updateLobbyState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_readyButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_startButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void LobbyScreen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LobbyScreen *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->startGame((*reinterpret_cast< std::add_pointer_t<Client*>>(_a[1]))); break;
        case 1: _t->updateLobbyState(); break;
        case 2: _t->on_readyButton_clicked(); break;
        case 3: _t->on_startButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LobbyScreen::*)(Client * );
            if (_t _q_method = &LobbyScreen::startGame; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *LobbyScreen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LobbyScreen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LobbyScreen.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LobbyScreen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void LobbyScreen::startGame(Client * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
