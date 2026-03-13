/****************************************************************************
** Meta object code from reading C++ file 'set_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "set_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'set_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetWidget_t {
    QByteArrayData data[6];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetWidget_t qt_meta_stringdata_SetWidget = {
    {
QT_MOC_LITERAL(0, 0, 9), // "SetWidget"
QT_MOC_LITERAL(1, 10, 14), // "changeLanguage"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4), // "flag"
QT_MOC_LITERAL(4, 31, 7), // "setfunc"
QT_MOC_LITERAL(5, 39, 12) // "current_icon"

    },
    "SetWidget\0changeLanguage\0\0flag\0setfunc\0"
    "current_icon"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   27,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void SetWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setfunc((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SetWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetWidget::changeLanguage)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SetWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_SetWidget.data,
    qt_meta_data_SetWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SetWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SetWidget::changeLanguage(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_SetAboutWidget_t {
    QByteArrayData data[37];
    char stringdata0[673];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetAboutWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetAboutWidget_t qt_meta_stringdata_SetAboutWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SetAboutWidget"
QT_MOC_LITERAL(1, 15, 14), // "changeLanguage"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 4), // "flag"
QT_MOC_LITERAL(4, 36, 10), // "retMainwid"
QT_MOC_LITERAL(5, 47, 13), // "showSetupCode"
QT_MOC_LITERAL(6, 61, 7), // "setfunc"
QT_MOC_LITERAL(7, 69, 12), // "current_icon"
QT_MOC_LITERAL(8, 82, 14), // "chineseClicked"
QT_MOC_LITERAL(9, 97, 14), // "englishClicked"
QT_MOC_LITERAL(10, 112, 11), // "trcnClicked"
QT_MOC_LITERAL(11, 124, 10), // "wacClicked"
QT_MOC_LITERAL(12, 135, 16), // "netrebootClicked"
QT_MOC_LITERAL(13, 152, 13), // "netfixClicked"
QT_MOC_LITERAL(14, 166, 18), // "dali_1_all_Clicked"
QT_MOC_LITERAL(15, 185, 18), // "dali_1_new_Clicked"
QT_MOC_LITERAL(16, 204, 18), // "dali_2_all_Clicked"
QT_MOC_LITERAL(17, 223, 18), // "dali_2_new_Clicked"
QT_MOC_LITERAL(18, 242, 22), // "identify_start_Clicked"
QT_MOC_LITERAL(19, 265, 21), // "identify_stop_Clicked"
QT_MOC_LITERAL(20, 287, 20), // "identify_pre_Clicked"
QT_MOC_LITERAL(21, 308, 21), // "identify_next_Clicked"
QT_MOC_LITERAL(22, 330, 28), // "dali_reconfiguration_Clicked"
QT_MOC_LITERAL(23, 359, 40), // "dali_1_addr_reconfiguration_s..."
QT_MOC_LITERAL(24, 400, 42), // "dali_1_addr_reconfiguration_c..."
QT_MOC_LITERAL(25, 443, 40), // "dali_2_addr_reconfiguration_s..."
QT_MOC_LITERAL(26, 484, 42), // "dali_2_addr_reconfiguration_c..."
QT_MOC_LITERAL(27, 527, 21), // "facreset_sure_Clicked"
QT_MOC_LITERAL(28, 549, 23), // "facreset_cancel_Clicked"
QT_MOC_LITERAL(29, 573, 14), // "programClicked"
QT_MOC_LITERAL(30, 588, 10), // "logClicked"
QT_MOC_LITERAL(31, 599, 10), // "iosClicked"
QT_MOC_LITERAL(32, 610, 10), // "andClicked"
QT_MOC_LITERAL(33, 621, 14), // "setcodeClicked"
QT_MOC_LITERAL(34, 636, 10), // "facClicked"
QT_MOC_LITERAL(35, 647, 10), // "retClicked"
QT_MOC_LITERAL(36, 658, 14) // "onTimerTaskSet"

    },
    "SetAboutWidget\0changeLanguage\0\0flag\0"
    "retMainwid\0showSetupCode\0setfunc\0"
    "current_icon\0chineseClicked\0englishClicked\0"
    "trcnClicked\0wacClicked\0netrebootClicked\0"
    "netfixClicked\0dali_1_all_Clicked\0"
    "dali_1_new_Clicked\0dali_2_all_Clicked\0"
    "dali_2_new_Clicked\0identify_start_Clicked\0"
    "identify_stop_Clicked\0identify_pre_Clicked\0"
    "identify_next_Clicked\0"
    "dali_reconfiguration_Clicked\0"
    "dali_1_addr_reconfiguration_sure_Clicked\0"
    "dali_1_addr_reconfiguration_cancel_Clicked\0"
    "dali_2_addr_reconfiguration_sure_Clicked\0"
    "dali_2_addr_reconfiguration_cancel_Clicked\0"
    "facreset_sure_Clicked\0facreset_cancel_Clicked\0"
    "programClicked\0logClicked\0iosClicked\0"
    "andClicked\0setcodeClicked\0facClicked\0"
    "retClicked\0onTimerTaskSet"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetAboutWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  179,    2, 0x06 /* Public */,
       4,    0,  182,    2, 0x06 /* Public */,
       5,    0,  183,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,  184,    2, 0x08 /* Private */,
       8,    0,  187,    2, 0x08 /* Private */,
       9,    0,  188,    2, 0x08 /* Private */,
      10,    0,  189,    2, 0x08 /* Private */,
      11,    0,  190,    2, 0x08 /* Private */,
      12,    0,  191,    2, 0x08 /* Private */,
      13,    0,  192,    2, 0x08 /* Private */,
      14,    0,  193,    2, 0x08 /* Private */,
      15,    0,  194,    2, 0x08 /* Private */,
      16,    0,  195,    2, 0x08 /* Private */,
      17,    0,  196,    2, 0x08 /* Private */,
      18,    0,  197,    2, 0x08 /* Private */,
      19,    0,  198,    2, 0x08 /* Private */,
      20,    0,  199,    2, 0x08 /* Private */,
      21,    0,  200,    2, 0x08 /* Private */,
      22,    0,  201,    2, 0x08 /* Private */,
      23,    0,  202,    2, 0x08 /* Private */,
      24,    0,  203,    2, 0x08 /* Private */,
      25,    0,  204,    2, 0x08 /* Private */,
      26,    0,  205,    2, 0x08 /* Private */,
      27,    0,  206,    2, 0x08 /* Private */,
      28,    0,  207,    2, 0x08 /* Private */,
      29,    0,  208,    2, 0x08 /* Private */,
      30,    0,  209,    2, 0x08 /* Private */,
      31,    0,  210,    2, 0x08 /* Private */,
      32,    0,  211,    2, 0x08 /* Private */,
      33,    0,  212,    2, 0x08 /* Private */,
      34,    0,  213,    2, 0x08 /* Private */,
      35,    0,  214,    2, 0x08 /* Private */,
      36,    0,  215,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SetAboutWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetAboutWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->retMainwid(); break;
        case 2: _t->showSetupCode(); break;
        case 3: _t->setfunc((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->chineseClicked(); break;
        case 5: _t->englishClicked(); break;
        case 6: _t->trcnClicked(); break;
        case 7: _t->wacClicked(); break;
        case 8: _t->netrebootClicked(); break;
        case 9: _t->netfixClicked(); break;
        case 10: _t->dali_1_all_Clicked(); break;
        case 11: _t->dali_1_new_Clicked(); break;
        case 12: _t->dali_2_all_Clicked(); break;
        case 13: _t->dali_2_new_Clicked(); break;
        case 14: _t->identify_start_Clicked(); break;
        case 15: _t->identify_stop_Clicked(); break;
        case 16: _t->identify_pre_Clicked(); break;
        case 17: _t->identify_next_Clicked(); break;
        case 18: _t->dali_reconfiguration_Clicked(); break;
        case 19: _t->dali_1_addr_reconfiguration_sure_Clicked(); break;
        case 20: _t->dali_1_addr_reconfiguration_cancel_Clicked(); break;
        case 21: _t->dali_2_addr_reconfiguration_sure_Clicked(); break;
        case 22: _t->dali_2_addr_reconfiguration_cancel_Clicked(); break;
        case 23: _t->facreset_sure_Clicked(); break;
        case 24: _t->facreset_cancel_Clicked(); break;
        case 25: _t->programClicked(); break;
        case 26: _t->logClicked(); break;
        case 27: _t->iosClicked(); break;
        case 28: _t->andClicked(); break;
        case 29: _t->setcodeClicked(); break;
        case 30: _t->facClicked(); break;
        case 31: _t->retClicked(); break;
        case 32: _t->onTimerTaskSet(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SetAboutWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetAboutWidget::changeLanguage)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SetAboutWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetAboutWidget::retMainwid)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SetAboutWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetAboutWidget::showSetupCode)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SetAboutWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_SetAboutWidget.data,
    qt_meta_data_SetAboutWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetAboutWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetAboutWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetAboutWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SetAboutWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 33)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void SetAboutWidget::changeLanguage(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SetAboutWidget::retMainwid()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SetAboutWidget::showSetupCode()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
