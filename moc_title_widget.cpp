/****************************************************************************
** Meta object code from reading C++ file 'title_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "title_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'title_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HouseButton_t {
    QByteArrayData data[3];
    char stringdata0[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HouseButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HouseButton_t qt_meta_stringdata_HouseButton = {
    {
QT_MOC_LITERAL(0, 0, 11), // "HouseButton"
QT_MOC_LITERAL(1, 12, 11), // "pressButton"
QT_MOC_LITERAL(2, 24, 0) // ""

    },
    "HouseButton\0pressButton\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HouseButton[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void HouseButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HouseButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pressButton(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HouseButton::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HouseButton::pressButton)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject HouseButton::staticMetaObject = { {
    &QPushButton::staticMetaObject,
    qt_meta_stringdata_HouseButton.data,
    qt_meta_data_HouseButton,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HouseButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HouseButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HouseButton.stringdata0))
        return static_cast<void*>(this);
    return QPushButton::qt_metacast(_clname);
}

int HouseButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void HouseButton::pressButton()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_TitleWidget_t {
    QByteArrayData data[33];
    char stringdata0[534];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TitleWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TitleWidget_t qt_meta_stringdata_TitleWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "TitleWidget"
QT_MOC_LITERAL(1, 12, 8), // "turnPage"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 12), // "current_page"
QT_MOC_LITERAL(4, 35, 9), // "last_page"
QT_MOC_LITERAL(5, 45, 15), // "switch_to_house"
QT_MOC_LITERAL(6, 61, 12), // "current_room"
QT_MOC_LITERAL(7, 74, 12), // "current_name"
QT_MOC_LITERAL(8, 87, 18), // "press_button_check"
QT_MOC_LITERAL(9, 106, 15), // "switch_to_scene"
QT_MOC_LITERAL(10, 122, 21), // "switch_to_scene_first"
QT_MOC_LITERAL(11, 144, 16), // "scene_show_delay"
QT_MOC_LITERAL(12, 161, 17), // "bottomAniFinished"
QT_MOC_LITERAL(13, 179, 17), // "light_button_slot"
QT_MOC_LITERAL(14, 197, 19), // "curtain_button_slot"
QT_MOC_LITERAL(15, 217, 19), // "content_button_slot"
QT_MOC_LITERAL(16, 237, 18), // "switch_button_slot"
QT_MOC_LITERAL(17, 256, 17), // "press_button_slot"
QT_MOC_LITERAL(18, 274, 19), // "switch_button1_slot"
QT_MOC_LITERAL(19, 294, 8), // "turnIcon"
QT_MOC_LITERAL(20, 303, 12), // "current_icon"
QT_MOC_LITERAL(21, 316, 15), // "scene_button_on"
QT_MOC_LITERAL(22, 332, 16), // "scene_button_off"
QT_MOC_LITERAL(23, 349, 20), // "hvac2scene_button_on"
QT_MOC_LITERAL(24, 370, 23), // "scene2curtain_button_on"
QT_MOC_LITERAL(25, 394, 22), // "hvac2curtain_button_on"
QT_MOC_LITERAL(26, 417, 18), // "curtain_button_off"
QT_MOC_LITERAL(27, 436, 14), // "hvac_button_on"
QT_MOC_LITERAL(28, 451, 20), // "scene2hvac_button_on"
QT_MOC_LITERAL(29, 472, 15), // "hvac_button_off"
QT_MOC_LITERAL(30, 488, 18), // "group_winsFinished"
QT_MOC_LITERAL(31, 507, 12), // "TimerTimeOut"
QT_MOC_LITERAL(32, 520, 13) // "set_room_name"

    },
    "TitleWidget\0turnPage\0\0current_page\0"
    "last_page\0switch_to_house\0current_room\0"
    "current_name\0press_button_check\0"
    "switch_to_scene\0switch_to_scene_first\0"
    "scene_show_delay\0bottomAniFinished\0"
    "light_button_slot\0curtain_button_slot\0"
    "content_button_slot\0switch_button_slot\0"
    "press_button_slot\0switch_button1_slot\0"
    "turnIcon\0current_icon\0scene_button_on\0"
    "scene_button_off\0hvac2scene_button_on\0"
    "scene2curtain_button_on\0hvac2curtain_button_on\0"
    "curtain_button_off\0hvac_button_on\0"
    "scene2hvac_button_on\0hvac_button_off\0"
    "group_winsFinished\0TimerTimeOut\0"
    "set_room_name"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TitleWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  149,    2, 0x06 /* Public */,
       5,    2,  154,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  159,    2, 0x0a /* Public */,
       9,    0,  160,    2, 0x0a /* Public */,
      10,    0,  161,    2, 0x0a /* Public */,
      11,    0,  162,    2, 0x0a /* Public */,
      12,    0,  163,    2, 0x0a /* Public */,
      13,    0,  164,    2, 0x0a /* Public */,
      14,    0,  165,    2, 0x0a /* Public */,
      15,    0,  166,    2, 0x0a /* Public */,
      16,    0,  167,    2, 0x0a /* Public */,
      17,    0,  168,    2, 0x0a /* Public */,
      18,    0,  169,    2, 0x0a /* Public */,
       1,    2,  170,    2, 0x0a /* Public */,
      19,    1,  175,    2, 0x0a /* Public */,
      21,    0,  178,    2, 0x0a /* Public */,
      22,    0,  179,    2, 0x0a /* Public */,
      23,    0,  180,    2, 0x0a /* Public */,
      24,    0,  181,    2, 0x0a /* Public */,
      25,    0,  182,    2, 0x0a /* Public */,
      26,    0,  183,    2, 0x0a /* Public */,
      27,    0,  184,    2, 0x0a /* Public */,
      28,    0,  185,    2, 0x0a /* Public */,
      29,    0,  186,    2, 0x0a /* Public */,
      30,    0,  187,    2, 0x0a /* Public */,
      31,    0,  188,    2, 0x0a /* Public */,
      32,    0,  189,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    6,    7,

 // slots: parameters
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
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,   20,
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

void TitleWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TitleWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->turnPage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->switch_to_house((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->press_button_check(); break;
        case 3: _t->switch_to_scene(); break;
        case 4: _t->switch_to_scene_first(); break;
        case 5: _t->scene_show_delay(); break;
        case 6: _t->bottomAniFinished(); break;
        case 7: _t->light_button_slot(); break;
        case 8: _t->curtain_button_slot(); break;
        case 9: _t->content_button_slot(); break;
        case 10: _t->switch_button_slot(); break;
        case 11: _t->press_button_slot(); break;
        case 12: _t->switch_button1_slot(); break;
        case 13: _t->turnPage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->turnIcon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->scene_button_on(); break;
        case 16: _t->scene_button_off(); break;
        case 17: _t->hvac2scene_button_on(); break;
        case 18: _t->scene2curtain_button_on(); break;
        case 19: _t->hvac2curtain_button_on(); break;
        case 20: _t->curtain_button_off(); break;
        case 21: _t->hvac_button_on(); break;
        case 22: _t->scene2hvac_button_on(); break;
        case 23: _t->hvac_button_off(); break;
        case 24: _t->group_winsFinished(); break;
        case 25: _t->TimerTimeOut(); break;
        case 26: _t->set_room_name(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TitleWidget::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TitleWidget::turnPage)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TitleWidget::*)(int , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TitleWidget::switch_to_house)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TitleWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_TitleWidget.data,
    qt_meta_data_TitleWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TitleWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TitleWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TitleWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TitleWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 27)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 27;
    }
    return _id;
}

// SIGNAL 0
void TitleWidget::turnPage(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TitleWidget::switch_to_house(int _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
