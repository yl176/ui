/****************************************************************************
** Meta object code from reading C++ file 'content_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "content_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'content_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SceneButton_t {
    QByteArrayData data[3];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SceneButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SceneButton_t qt_meta_stringdata_SceneButton = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SceneButton"
QT_MOC_LITERAL(1, 12, 9), // "pressIcon"
QT_MOC_LITERAL(2, 22, 0) // ""

    },
    "SceneButton\0pressIcon\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SceneButton[] = {

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

void SceneButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SceneButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pressIcon(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SceneButton::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SceneButton::pressIcon)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SceneButton::staticMetaObject = { {
    &QToolButton::staticMetaObject,
    qt_meta_stringdata_SceneButton.data,
    qt_meta_data_SceneButton,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SceneButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SceneButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SceneButton.stringdata0))
        return static_cast<void*>(this);
    return QToolButton::qt_metacast(_clname);
}

int SceneButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
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
void SceneButton::pressIcon()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_sceneWidget_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sceneWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sceneWidget_t qt_meta_stringdata_sceneWidget = {
    {
QT_MOC_LITERAL(0, 0, 11) // "sceneWidget"

    },
    "sceneWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sceneWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void sceneWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject sceneWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_sceneWidget.data,
    qt_meta_data_sceneWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *sceneWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sceneWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_sceneWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int sceneWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_ContentWidget_t {
    QByteArrayData data[20];
    char stringdata0[261];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ContentWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ContentWidget_t qt_meta_stringdata_ContentWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ContentWidget"
QT_MOC_LITERAL(1, 14, 10), // "onTimerOut"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 8), // "turnIcon"
QT_MOC_LITERAL(4, 35, 12), // "current_icon"
QT_MOC_LITERAL(5, 48, 9), // "PressIcon"
QT_MOC_LITERAL(6, 58, 10), // "onTimerExe"
QT_MOC_LITERAL(7, 69, 15), // "light_dim_check"
QT_MOC_LITERAL(8, 85, 15), // "scene_main_back"
QT_MOC_LITERAL(9, 101, 16), // "scene_turn_onoff"
QT_MOC_LITERAL(10, 118, 17), // "valueChanged_slot"
QT_MOC_LITERAL(11, 136, 17), // "animationFinished"
QT_MOC_LITERAL(12, 154, 4), // "next"
QT_MOC_LITERAL(13, 159, 7), // "forward"
QT_MOC_LITERAL(14, 167, 17), // "scene_to_lightdim"
QT_MOC_LITERAL(15, 185, 17), // "lightdim_to_scene"
QT_MOC_LITERAL(16, 203, 14), // "scene_move_end"
QT_MOC_LITERAL(17, 218, 13), // "buttonClicked"
QT_MOC_LITERAL(18, 232, 14), // "changeLanguage"
QT_MOC_LITERAL(19, 247, 13) // "setLightValue"

    },
    "ContentWidget\0onTimerOut\0\0turnIcon\0"
    "current_icon\0PressIcon\0onTimerExe\0"
    "light_dim_check\0scene_main_back\0"
    "scene_turn_onoff\0valueChanged_slot\0"
    "animationFinished\0next\0forward\0"
    "scene_to_lightdim\0lightdim_to_scene\0"
    "scene_move_end\0buttonClicked\0"
    "changeLanguage\0setLightValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ContentWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x0a /* Public */,
       3,    1,  100,    2, 0x0a /* Public */,
       5,    0,  103,    2, 0x0a /* Public */,
       6,    0,  104,    2, 0x0a /* Public */,
       7,    0,  105,    2, 0x0a /* Public */,
       8,    0,  106,    2, 0x0a /* Public */,
       9,    0,  107,    2, 0x0a /* Public */,
      10,    1,  108,    2, 0x0a /* Public */,
      11,    0,  111,    2, 0x0a /* Public */,
      12,    0,  112,    2, 0x0a /* Public */,
      13,    0,  113,    2, 0x0a /* Public */,
      14,    0,  114,    2, 0x0a /* Public */,
      15,    0,  115,    2, 0x0a /* Public */,
      16,    0,  116,    2, 0x0a /* Public */,
      17,    0,  117,    2, 0x08 /* Private */,
      18,    0,  118,    2, 0x08 /* Private */,
      19,    1,  119,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void ContentWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ContentWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTimerOut(); break;
        case 1: _t->turnIcon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->PressIcon(); break;
        case 3: _t->onTimerExe(); break;
        case 4: _t->light_dim_check(); break;
        case 5: _t->scene_main_back(); break;
        case 6: _t->scene_turn_onoff(); break;
        case 7: _t->valueChanged_slot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 8: _t->animationFinished(); break;
        case 9: _t->next(); break;
        case 10: _t->forward(); break;
        case 11: _t->scene_to_lightdim(); break;
        case 12: _t->lightdim_to_scene(); break;
        case 13: _t->scene_move_end(); break;
        case 14: _t->buttonClicked(); break;
        case 15: _t->changeLanguage(); break;
        case 16: _t->setLightValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ContentWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ContentWidget.data,
    qt_meta_data_ContentWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ContentWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ContentWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ContentWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ContentWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
