/****************************************************************************
** Meta object code from reading C++ file 'roomlight_dim.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "roomlight_dim.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'roomlight_dim.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RoomLightDimWidget_t {
    QByteArrayData data[12];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RoomLightDimWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RoomLightDimWidget_t qt_meta_stringdata_RoomLightDimWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RoomLightDimWidget"
QT_MOC_LITERAL(1, 19, 17), // "closeRoomlightDim"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 10), // "onTimerOut"
QT_MOC_LITERAL(4, 49, 13), // "onTimerDimOut"
QT_MOC_LITERAL(5, 63, 21), // "single_send_light_dim"
QT_MOC_LITERAL(6, 85, 17), // "single_dim_paint0"
QT_MOC_LITERAL(7, 103, 17), // "single_dim_paint1"
QT_MOC_LITERAL(8, 121, 17), // "single_dim_paint2"
QT_MOC_LITERAL(9, 139, 17), // "single_dim_paint3"
QT_MOC_LITERAL(10, 157, 17), // "single_dim_paint4"
QT_MOC_LITERAL(11, 175, 14) // "changeLanguage"

    },
    "RoomLightDimWidget\0closeRoomlightDim\0"
    "\0onTimerOut\0onTimerDimOut\0"
    "single_send_light_dim\0single_dim_paint0\0"
    "single_dim_paint1\0single_dim_paint2\0"
    "single_dim_paint3\0single_dim_paint4\0"
    "changeLanguage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RoomLightDimWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   65,    2, 0x0a /* Public */,
       4,    0,   66,    2, 0x0a /* Public */,
       5,    0,   67,    2, 0x0a /* Public */,
       6,    0,   68,    2, 0x0a /* Public */,
       7,    0,   69,    2, 0x0a /* Public */,
       8,    0,   70,    2, 0x0a /* Public */,
       9,    0,   71,    2, 0x0a /* Public */,
      10,    0,   72,    2, 0x0a /* Public */,
      11,    0,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

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

       0        // eod
};

void RoomLightDimWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RoomLightDimWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closeRoomlightDim(); break;
        case 1: _t->onTimerOut(); break;
        case 2: _t->onTimerDimOut(); break;
        case 3: _t->single_send_light_dim(); break;
        case 4: _t->single_dim_paint0(); break;
        case 5: _t->single_dim_paint1(); break;
        case 6: _t->single_dim_paint2(); break;
        case 7: _t->single_dim_paint3(); break;
        case 8: _t->single_dim_paint4(); break;
        case 9: _t->changeLanguage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RoomLightDimWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RoomLightDimWidget::closeRoomlightDim)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject RoomLightDimWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_RoomLightDimWidget.data,
    qt_meta_data_RoomLightDimWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RoomLightDimWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RoomLightDimWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RoomLightDimWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RoomLightDimWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void RoomLightDimWidget::closeRoomlightDim()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
