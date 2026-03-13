/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include "enno_encode.h"
#include "common.h"

typedef enum
{
    EN_SCENE_LIST = 0,
	EN_SCENE_ADD,
	EN_SCENE_REMOVE,
	EN_SCENE_ON,
	EN_SCENE_OFF,
	EN_SCENE_MODIFY,
	EN_SCENE_DEL_DEV,
	EN_SCENE_ROOM_SET,
	EN_SCENE_DEVS_SET,
	EN_SCENE_FREE_SET,
	EN_SCENE_FREE_CREATE,
	EN_SCENE_METAL_ON,
	EN_SCENE_METAL_OFF,
	EN_SCENE_CMD_MAX,
}ENMU_SCENE_CMD;

typedef enum
{
	EN_DEVICE_CTL = 0,
	EN_SCENE_TURN_ON,
	EN_SCENE_TURN_OFF,
	EN_DEVICE_CTRL_MAX
}EN_DEVICE_CTRL_CMD;

typedef enum
{
    DEV_DALI_LIGHT            = 100,
    DEV_DALI_GROUP            = 101,
    DEV_DALI_BOARDCAST        = 102,
    DEV_RS485_HEATER_COOLER   = 103,
    DEV_RS485_SHADES          = 104,
    DEV_RS485_AIR_PURIFIER    = 105,
    DEV_DMX_LIGHT             = 106,
    DEV_KNX_SWITCH            = 107,
    DEV_P2					  = 108,
	DEV_RS485_AIR_COND        = 109,    
    DEV_RESERVE               = 110,
    DEV_P2_TEMP_SENSOR        = 111,    
    DEV_P2_HUM_SENSOR         = 112,     
    DEV_P2_AIR_QUALITY_SENSOR = 113,
    DEV_P2_LIGHT_SENSOR       = 114,   
    DEV_KNX_LIGHT_SWITCH      = 115,
    DEV_KNX_CONTACT_SENSOR    = 116,
    DEV_KNX_MOTION_SENSOR     = 117,
    DEV_KNX_ALARM             = 118,      
    DEV_KNX_OUTLET            = 119,
    DEV_KNX_DOOR              = 120, 
    DEV_KNX_WINDOW            = 121,    

	DEV_CO2_SENSOR            = 122,
	DEV_LEAK_SENSOR           = 123,
	DEV_TV_SET                = 124,
	DEV_FLOOR_HEAT            = 125,

	DEV_RS485_FIRE_PLACE	  = 126,
	DEV_RS485_POWER_MANAGER   = 127,

    DEV_RS485_GW_LETTIN       = 129,
	DEV_RS485_GW_LIGHT_PLAYER = 130,

    DEV_TYPE_MAX              = 131,  
    
    DEV_RS485_HEATER_COOLER_INTERNAL=1030,
	DEV_RS485_HEATER_COOLER_ABROAD  =1031,
	DEV_RS485_HEATER_FLOOR          = 1032,
}DEV_TYPE_DEF;

typedef enum
{
    DEV_HOME = 110,   // 110
    DEV_ROOM,   //111
    DEV_SCENE,  //112
}EN_DEV_TYPE;


typedef struct ST_DeviceItemtag
{
	QString m_Id;
	QString m_name;
	QString m_mode;
	QString value;
	QString speed;
	int on;
	int image_id;
	int scene_id;
	int device_id;
	int device_type;
	int current_pos;
	int target_pos;
	int mod;
	uint32 dim_value;
	int factory_model;
	int addr;
}ST_DeviceItem;

//! [0]
class BIL_SHARE TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

public:
    QList<TreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem *m_parentItem;
};
//! [0]

#endif // TREEITEM_H
