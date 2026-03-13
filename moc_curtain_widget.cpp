/****************************************************************************
** Meta object code from reading C++ file 'curtain_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "curtain_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'curtain_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CurtainWidget_t {
    QByteArrayData data[15];
    char stringdata0[212];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CurtainWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CurtainWidget_t qt_meta_stringdata_CurtainWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CurtainWidget"
QT_MOC_LITERAL(1, 14, 8), // "turnIcon"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 12), // "current_icon"
QT_MOC_LITERAL(4, 37, 10), // "onTimerOut"
QT_MOC_LITERAL(5, 48, 10), // "PressShade"
QT_MOC_LITERAL(6, 59, 15), // "shade_pos_check"
QT_MOC_LITERAL(7, 75, 13), // "setLightValue"
QT_MOC_LITERAL(8, 89, 17), // "lightdim_to_shade"
QT_MOC_LITERAL(9, 107, 17), // "shade_to_lightdim"
QT_MOC_LITERAL(10, 125, 16), // "shade_turn_onoff"
QT_MOC_LITERAL(11, 142, 15), // "shade_main_back"
QT_MOC_LITERAL(12, 158, 17), // "curtaion_move_end"
QT_MOC_LITERAL(13, 176, 17), // "valueChanged_slot"
QT_MOC_LITERAL(14, 194, 17) // "animationFinished"

    },
    "CurtainWidget\0turnIcon\0\0current_icon\0"
    "onTimerOut\0PressShade\0shade_pos_check\0"
    "setLightValue\0lightdim_to_shade\0"
    "shade_to_lightdim\0shade_turn_onoff\0"
    "shade_main_back\0curtaion_move_end\0"
    "valueChanged_slot\0animationFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CurtainWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x0a /* Public */,
       4,    0,   77,    2, 0x0a /* Public */,
       5,    0,   78,    2, 0x0a /* Public */,
       6,    0,   79,    2, 0x0a /* Public */,
       7,    1,   80,    2, 0x0a /* Public */,
       8,    0,   83,    2, 0x0a /* Public */,
       9,    0,   84,    2, 0x0a /* Public */,
      10,    0,   85,    2, 0x0a /* Public */,
      11,    0,   86,    2, 0x0a /* Public */,
      12,    0,   87,    2, 0x0a /* Public */,
      13,    1,   88,    2, 0x08 /* Private */,
      14,    0,   91,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,

       0        // eod
};

void CurtainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CurtainWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->turnIcon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->onTimerOut(); break;
        case 2: _t->PressShade(); break;
        case 3: _t->shade_pos_check(); break;
        case 4: _t->setLightValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->lightdim_to_shade(); break;
        case 6: _t->shade_to_lightdim(); break;
        case 7: _t->shade_turn_onoff(); break;
        case 8: _t->shade_main_back(); break;
        case 9: _t->curtaion_move_end(); break;
        case 10: _t->valueChanged_slot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 11: _t->animationFinished(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CurtainWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_CurtainWidget.data,
    qt_meta_data_CurtainWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CurtainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CurtainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CurtainWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CurtainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
