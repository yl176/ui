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

/*
    treemodel.cpp

    Provides a simple tree model to show how to create and use hierarchical
    models.
*/

#include "treeitem.h"
#include "treemodel.h"
#include <stdio.h>
#include <QDebug>
#include <QStringList>
#include <unistd.h>
//#include <QJsonDocument>
//#include <QJsonArray>
//#include <QJsonObject>
//#include <QTextCodec>


//! [0]
TreeModel::TreeModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
#if 0
    QList<QVariant> rootData;
    rootData << "Title" << "Summary";
    rootItem = new TreeItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
#endif
	//qDebug()<<"start readjs"<<endl;
	//createjs();
	#ifndef X86
	read_scene_show();
	readjs();
	#endif
}
//! [0]

//! [1]
TreeModel::~TreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}
//! [2]

//! [3]
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}
//! [3]

//! [4]
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}
//! [5]

//! [6]
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
//! [7]

//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
//! [8]
#ifndef X86
void TreeModel::load_boot_mode_status(uint32 *p_status)
{
	int nbytes = 1;
	char tmp[32];
	char *path = BOOT_MODE_FILE;
	QFile file(BOOT_MODE_FILE);

	memset(tmp, 0, 32);
	
	if (!file.open(QFile::ReadOnly))
	{
	    qDebug()<<"TreeModel fileopen fail";
	    return;
	}
	nbytes = file.read(tmp,10);

	if (!nbytes)
	{
        *p_status = 0;
		file.close();
		return;
	}

	*p_status = atoi(tmp);

    //qDebug()<<"TreeModel boot_mode="<<*p_status;
	
	file.close();
	return;
}

int TreeModel::get_scene_show(QString scene_name_in)
{
    int i = 0;
	
	qDebug()<<"name_in="<<scene_name_in;
    for (i = 0; i < scene_num; i++)
    {
        qDebug()<<"stScenes[i].s_name="<<stScenes[i].s_name;
		if (QString::compare(scene_name_in,stScenes[i].s_name) == 0 )
        {
            ENNO_LOG_ERR("find scene, show is=%d\n", stScenes[i].show);
            return stScenes[i].show;
        }
    }

    ENNO_LOG_ERR("not find scene\n");
	return 1;
}

void TreeModel::read_scene_show()
{
    char pFile[128];
	char cmd[128];
	char file_name[128];
	int i;
	int ret;
	int cnt = 0;

    scene_num = 0;
	memset(file_name, 0, 128);
	strcpy(file_name, SCENE_SHOW_CONF);

	/*if (access(BOOT_MODE_FILE, F_OK) == 0)
	{
	    load_boot_mode_status(&boot_mode);
		//qDebug()<<"load boot_mode file ok,boot_mode"<<boot_mode;
		if (boot_mode)
		{
		    memset(file_name, 0, 128);
		    snprintf(file_name, 128, "%s", SCENE_ANDROID_CONF);
		}
	}*/

	ENNO_LOG_ERR("read file_name=%s\n", file_name);
		
	if (access(file_name, F_OK) != 0)
	{
	    ENNO_LOG_ERR("access file_name fail\n", file_name);
		return;
	}
	else
	{
	    strcpy(pFile, file_name);
	}

	QFile file1(file_name);
	
	file1.open(QIODevice::ReadOnly | QIODevice::Text);
	
	QByteArray fileData = file1.readAll();
    QString fileStr = QString::fromUtf8(fileData);
	
	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(fileStr.toUtf8(), &jsonError);
	
	ENNO_LOG_ERR("\n");
	if(jsonError.error != QJsonParseError::NoError)
	{
		syslog(LOG_INFO|LOG_LOCAL1,"read json file error, jsonError.error=%d\n", jsonError.error);
		ENNO_LOG_ERR("\n");
		return;
	    //strcpy(pFile, SCENE_ROOM_DEF_CONF);
	}
	else
	{
	    snprintf(cmd, 128, "cp %s %s ", pFile, SCENE_ROOM_DEF_CONF);
		//qDebug()<<"cmd=["<<cmd<<"]";
		system(cmd);
		usleep(20000);
		system("sync");
	}
	QJsonObject object_home = jsonDoc.object();  // level_1 home
	ENNO_LOG_ERR("\n");

	if (object_home.contains("scenes")) 
	{  // 包含指定的 homeName
		QJsonValue valueS = object_home.value("scenes");  // 获取指定 key 对应的 value
	    if (valueS.isArray()) 
		{
		    QJsonArray scenes_array = valueS.toArray(); 
			int nSize = scenes_array.size();
			
			ENNO_LOG_ERR("scenes size=%d\n", nSize);
			for (i = 0; i < nSize; i++)
			{
				QJsonValue value = scenes_array.at(i);
				if (value.isObject())
				{
					ENNO_LOG_ERR("\n");
					QJsonObject object_scene = value.toObject();
					if (object_scene.contains("name")) 
					{  // 包含指定的 homeName
						QJsonValue value = object_scene.value("name");  // 获取指定 key 对应的 value
						if (value.isString()) 
						{  // 判断 value 是否为字符串
							stScenes[cnt].s_name = value.toString();  // 将 value 转化为字符串
						}
					}

					if (object_scene.contains("show")) 
					{  // 包含指定的 homeName
						QJsonValue value = object_scene.value("show");  // 获取指定 key 对应的 value
						if (value.isDouble()) 
						{  // 判断 value 是否为字符串
							long intResult = value.toDouble();  // 将 value 转化为字符串
							ENNO_LOG_ERR("intResult=%d\n", intResult);
							stScenes[cnt].show = (intResult == 0) ? 0 : 1;
						}
						else
						{
						    ENNO_LOG_ERR("value is not double\n");
						}
					}

					if (stScenes[cnt].show != 0 && stScenes[cnt].show != 1)
					{
					    stScenes[cnt].show = 1;
					}
					cnt++;
				}
			}
	 	}
	}
#if 0
	QFile file(pFile);

	QByteArray byteArray;

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
        return;
	}
	 while (!file.atEnd()) {
        QByteArray line = file.readLine();
		
		byteArray.append(line);
    }
	 #endif
	//parsejs(byteArray);
    scene_num = cnt;
	ENNO_LOG_ERR("scene_num=%d\n", scene_num);
	//关闭文件
	file1.close();
}


void TreeModel::readjs()
{
    char pFile[128];
	char cmd[128];
	char file_name[128];
	int iRet = 0;

	memset(file_name, 0, 128);
	strcpy(file_name, SCENE_ROOM_CONF);

	/*if (access(BOOT_MODE_FILE, F_OK) == 0)
	{
	    load_boot_mode_status(&boot_mode);
		//qDebug()<<"load boot_mode file ok,boot_mode"<<boot_mode;
		if (boot_mode)
		{
		    memset(file_name, 0, 128);
		    snprintf(file_name, 128, "%s", SCENE_ANDROID_CONF);
		}
	}*/

	qDebug()<<"file name="<<file_name;
		
	if (access(file_name, F_OK) != 0)
	{
	    ENNO_LOG_ERR("access file_name fail\n", file_name);
		strcpy(pFile, SCENE_ROOM_DEF_CONF);
	}
	else
	{
	    strcpy(pFile, file_name);
	}

	
	QFile file1(file_name);
    
	
	file1.open(QIODevice::ReadOnly | QIODevice::Text);
	
	QByteArray fileData = file1.readAll();
    QString fileStr = QString::fromUtf8(fileData);
	
	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(fileStr.toUtf8(), &jsonError);
	
	if(jsonError.error != QJsonParseError::NoError)
	{
		syslog(LOG_INFO|LOG_LOCAL1,"read json file error, jsonError.error=%d\n", jsonError.error);
	    strcpy(pFile, SCENE_ROOM_DEF_CONF);
	}
	else
	{
	    snprintf(cmd, 128, "cp %s %s ", pFile, SCENE_ROOM_DEF_CONF);
		//qDebug()<<"cmd=["<<cmd<<"]";
		system(cmd);
		usleep(20000);
		system("sync");
	}

	QFile file(pFile);
#if 0
	QJsonArray devArray;
	QJsonArray sceneArray;
	QJsonArray roomArray;
	QJsonObject json_home;
	QJsonObject json_room;
	QJsonObject json_scene;
	QJsonObject json_dev;
	json_dev.insert("devName", "floor");
	json_dev.insert("id", "1234");
	json_dev.insert("type", "104");
	json_dev.insert("mode", "3");
	json_dev.insert("speed", "1");
	
	devArray.append(json_dev);
	
	json_scene.insert("sceneName", "早安");
    json_scene.insert("devList", QJsonValue(devArray));

    sceneArray.append(json_scene);
	
    json_room.insert("roomId", "234");
    json_room.insert("roomName", "room");
    json_room.insert("sceneArray", QJsonValue(sceneArray));

    roomArray.append(json_room);
    json_home.insert("homeName", "涓囬噷鍒");
	json_home.insert("homeId", "83DA0987-9B38-56AC-945C-8198A02C5FA3");
	json_home.insert("roomArray", QJsonValue(roomArray));
	// 构建 JSON 文档
	QJsonDocument document;
	document.setObject(json_home);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	QString strJson(byteArray);

	qDebug() << strJson;
#endif
	QByteArray byteArray;

	//file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
	    //qDebug()<<"[qterror]there is no scene file\n";
        return;
	}
	//qDebug()<<"read file"<<endl;
	 while (!file.atEnd()) {
        QByteArray line = file.readLine();
		
		//qDebug()<<"parsejs:"<<line;
		byteArray.append(line);
    }
//	qDebug()<<"parsejs:"<<byteArray;
	iRet = parsejs(byteArray);  //if parse fail, then load default set
    if (iRet)
	{
#if 1
		QJsonArray devArray;
		QJsonArray sceneArray;
		QJsonArray roomArray;
		QJsonObject json_home;
		QJsonObject json_room;
		QJsonObject json_scene;
		QJsonObject json_dev;
		json_dev.insert("devName", "floor");
		json_dev.insert("id", "1234");
		json_dev.insert("type", "104");
		json_dev.insert("mode", "3");
		json_dev.insert("speed", "1");
		
		devArray.append(json_dev);
		
		json_scene.insert("sceneName", "默认");
	    json_scene.insert("devList", QJsonValue(devArray));

	    sceneArray.append(json_scene);
		
	    json_room.insert("roomId", "234");
	    json_room.insert("roomName", "默认");
	    json_room.insert("sceneArray", QJsonValue(sceneArray));

	    roomArray.append(json_room);
	    json_home.insert("name", "默认家庭");//"涓囬噷鍒");
		json_home.insert("id", "83DA0987-9B38-56AC-945C-8198A02C5FA3");
		json_home.insert("type", "HouseRoomUpdate");
		json_home.insert("roomArray", QJsonValue(roomArray));
		// 构建 JSON 文档
		QJsonDocument document;
		document.setObject(json_home);
		QByteArray byteArray = document.toJson(QJsonDocument::Compact);
		QString strJson(byteArray);

		qDebug() << strJson;

		parsejs(byteArray);
#endif
	}
	//关闭文件
	file.close();
}

int TreeModel::parse_dev(QJsonArray dev_array, TreeItem *parent)
{	
	int nSize = dev_array.size();
	QList<TreeItem*> parents;
    parents << parent;
	//qDebug()<<"nSize="<<nSize<<endl;
	
	for (int i = 0; i < nSize; i++)
	{
		QList<QVariant> columnData;
	    QJsonValue value = dev_array.at(i);
		if (value.isObject())
		{
			QJsonObject object_dev = value.toObject();
			
			if (object_dev.contains("id"))
			{
				QJsonValue value = object_dev.value("id");
				if (value.isString()) 
				{
					QString dSerialid = value.toString();
					//qDebug() << "[devList]id : " << dSerialid;
					columnData<<dSerialid;
				}
			}
			
			if (object_dev.contains("name"))
			{
	            QJsonValue value = object_dev.value("name");
	            if (value.isString()) 
				{
				    QString devName = value.toString();  
	                //qDebug() << "[devList]name : " << devName;
					columnData<<devName;		
				}
			}
			else
			{
			    columnData<<"no";
			}
			
			if (object_dev.contains("type"))
			{
	            QJsonValue value = object_dev.value("type");
	            if (value.isString()) 
				{
				    QString dTpye = value.toString();
	                //qDebug() << "[devList]type : " << dTpye;
					columnData<<dTpye;
				}
			}
			
			if (object_dev.contains("val"))
			{
	            QJsonValue value = object_dev.value("val");
	            if (value.isString()) 
				{
				    QString dTpye = value.toString();
	                //qDebug() << "[devList]value : " << dTpye;
					columnData<<dTpye;
				}
			}

			if (object_dev.contains("spd"))
			{
	            QJsonValue value = object_dev.value("spd");
	            if (value.isString()) 
				{
				    QString dTpye = value.toString();
	                //qDebug() << "[devList]spd : " << dTpye;
					columnData<<dTpye;
					parents.last()->appendChild(new TreeItem(columnData, parents.last()));
				}
			}
		}
	}

	return 0;
}
extern uint32 g_sid_index[MAX_SCENE];

int TreeModel::parse_scene(QJsonArray scene_array, TreeItem *parent)
{
    int i;
	int ret;
	QList<TreeItem*> parents;
    parents << parent;
    int nSize = scene_array.size();
	static int s_scene_id = 0;
	
	if (s_scene_id == 0)
    {
        memset(g_sid_index, 0xff, MAX_SCENE);
    }
	for (i = 0; i < nSize; i++)
	{
		QJsonValue value = scene_array.at(i);
		QList<QVariant> columnData;
		if (value.isObject())
		{
		    QJsonObject object_scene = value.toObject();
		
            if (object_scene.contains("name")) 
			{  
				QJsonValue value = object_scene.value("name");	
				if (value.isString()) 
				{  				    
					QString devMode = QString::number(DEV_SCENE, 10);
					QString strName = value.toString(); 

					char*  ch;
					QByteArray ba = strName.toLocal8Bit(); // must
					ch = ba.data();
#if 0
					QString str = "中文";
					
					qDebug()<<"before_scene_nam="<<QString::fromLocal8Bit("中文");
					//qDebug()<<"before_scene_nam1="<<strName.toStdString().c_str();
					//printf();
					qDebug()<<"the_scene_name="<<strName.toStdString().c_str();
#endif
                    #if 0
                    if (object_scene.contains("show"))
                    {
                        QJsonValue value_show = object_scene.value("show");	
						if (value_show.isDouble()) 
						{
						    long intResult = value_show.toDouble();
							if (intResult == 0)
							{
							    s_scene_id++;
								continue;
							}
						}
                    }
					#endif

					if (!get_scene_show(strName))
					{
					    qDebug()<<"scene not show="<<strName;
					    s_scene_id++;
					    continue;
					}
					
					if (strlen(ch) >= 5) //free_,radar_
					{
					    if (!memcmp(ch, "cus_", 4))
					    {
					        s_scene_id++;
					        continue;
					    }
						else if (!memcmp(ch, "radar_", 6))
						{
						    s_scene_id++;
						    continue;
						}
						else if (!memcmp(ch, "cus", 3))
						{
						    s_scene_id++;
						    continue;
						}
					}
				}
			}
		
			if (object_scene.contains("id")) 
			{  
				QJsonValue value = object_scene.value("id");	
				if (value.isString()) 
				{  
					//QString str = value.toString();  
					//qDebug() << "[sceneArray]sceneId : " << str;
					//columnData<<str;
					
				}
				QString str;
				str = QString("%1").arg(s_scene_id);
		        columnData<<str;
				s_scene_id++;
			}
			else
			{
			   QString str;
			   str = QString("%1").arg(s_scene_id);
			   columnData<<str;
			   s_scene_id++;
			}

			if (object_scene.contains("name")) 
			{  
				QJsonValue value = object_scene.value("name");	
				if (value.isString()) 
				{  				    
					QString devMode = QString::number(DEV_SCENE, 10);
					//QString strName = value.toString();  
					//qDebug() << "[sceneArray]name : " << strName;
					QString strName = value.toString();

					/*char*  ch;
					QByteArray ba = strName.toLocal8Bit(); // must
					ch = ba.data();

                    printf("treemode(%d)={", ba.size());
					for (int k = 0; k < strName.length(); k++)
					{
					    printf("0x%x ", strName.data()[k], );
					}
					printf("}\r\n");*/
					columnData<<strName;
					columnData<<devMode;
				}
			}
			else
			{
			    QString devMode = QString::number(DEV_SCENE, 10);
			    QString strName("auto");  
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				columnData<<strName;
				columnData<<devMode;
			}

			if (object_scene.contains("imId")) 
			{  
				QJsonValue value = object_scene.value("imId");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[sceneArray]imageId : " << str;
					columnData<<str;
					columnData<<"spd";
					parents.last()->appendChild(new TreeItem(columnData, parents.last()));
				}
			}
            else
            {
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				QString str("1");
				columnData<<str;
				columnData<<"spd";
            }
			
			if (object_scene.contains("devs"))				//level_4 devList
			{
			    continue;   //no parse devs
				QJsonValue value = object_scene.value("devs");
				if (value.isArray())
				{
					QJsonArray dev_array = value.toArray();
					if (parents.last()->childCount() > 0) 
					{
	                    parents << parents.last()->child(parents.last()->childCount()-1);
					}
					ret = parse_dev(dev_array, parents.last());
					if (ret)
					{
					    qDebug() << "parse_dev error ret=%d\n" << ret;
					}
					parents.pop_back();
				}	
			}
	    }
	}

	return 0;
}

int TreeModel::parse_curtain(QJsonArray curtain_array, TreeItem *parent)
{
    int i;
	int ret;
	QList<TreeItem*> parents;
    parents << parent;
    int nSize = curtain_array.size();
	
	for (i = 0; i < nSize; i++)
	{
		QJsonValue value = curtain_array.at(i);
		QList<QVariant> columnData;
		if (value.isObject())
		{
			QJsonObject object_scene = value.toObject();
			if (object_scene.contains("id")) 
			{  
				QJsonValue value = object_scene.value("id");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[curtainArray]id : " << str;
					columnData<<str;
				}
			}
			else
			{
			    QString str("1111");
				qDebug() << "[auto][curtainArray]id : " << str;
				columnData<<str;
			}

			if (object_scene.contains("name")) 
			{  
				QJsonValue value = object_scene.value("name");	
				if (value.isString()) 
				{  				    
					QString devMode = QString::number(DEV_SCENE, 10);
					QString strName = value.toString();  
					//qDebug() << "[curtainArray]devName : " << strName;
					columnData<<strName;
				}
			}
			else
			{
				QString str("userdef");
				qDebug() << "[auto][curtainArray]devName : " << str;
				columnData<<str;
			}

			if (object_scene.contains("type")) 
			{  
				QJsonValue value = object_scene.value("type"); 
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[curtainArray]devType : " << str;
					columnData<<str;
				}
			}
			else
			{
				QString devMode = QString::number(DEV_RS485_SHADES, 10);
				qDebug() << "[auto][curtainArray]devType : " << devMode;
				columnData<<devMode;
			}

			if (object_scene.contains("val")) 
			{  
				QJsonValue value = object_scene.value("val");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[curtainArray]value : " << str;
					columnData<<str;
				}
			}
			else
			{
				QString str("0");
				qDebug() << "[auto][curtainArray]value : " << str;
				columnData<<str;
			}
			
            if (object_scene.contains("pow")) 
			{  
				QJsonValue value = object_scene.value("pow");
				if (value.isDouble()) 
				{  
					//QString str(value.toDouble());
					long intResult = value.toDouble();
					QString str=QString::number(intResult,10);
					//qDebug() << "[curtain]pow : " << str;
					columnData<<str;
					parents.last()->appendChild(new TreeItem(columnData, parents.last()));
				}
				else
				{
			       qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				}
			}
			else
			{
				QString str("0");				
				//qDebug() << "[auto]pow : " << str<<",length="<<columnData.length();
				columnData<<str;
				parents.last()->appendChild(new TreeItem(columnData, parents.last()));
			}
	    }
	}

	return 0;
}

int TreeModel::parse_lights(QJsonArray light_array, TreeItem *parent)
{
    int i;
	int ret;
	QList<TreeItem*> parents;
    parents << parent;
    int nSize = light_array.size();
	
	for (i = 0; i < nSize; i++)
	{
		QJsonValue value = light_array.at(i);
		QList<QVariant> columnData;
		if (value.isObject())
		{
			QJsonObject object_scene = value.toObject();
			if (object_scene.contains("id")) 
			{  
				QJsonValue value = object_scene.value("id");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[lightArray]id : " << str;
					columnData<<str;
				}
			}
			else
			{
			    QString str("1111");
				//qDebug() << "[auto][lightArray]id : " << str;
				columnData<<str;
			}

			if (object_scene.contains("name")) 
			{  
				QJsonValue value = object_scene.value("name");	
				if (value.isString()) 
				{  				    
					QString devMode = QString::number(DEV_SCENE, 10);
					QString strName = value.toString();  
					//qDebug() << "[curtainArray]devName : " << strName;
					columnData<<strName;
				}
			}
			else
			{
				QString str("userdef");
				//qDebug() << "[auto][lightArray]devName : " << str;
				columnData<<str;
			}

			if (object_scene.contains("type")) 
			{  
				QJsonValue value = object_scene.value("type"); 
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[lightArray]devType : " << str;
					columnData<<str;
				}
			}
			else
			{
				QString devMode = QString::number(DEV_DALI_LIGHT, 10);
				//qDebug() << "[auto][lightArray]devType : " << devMode;
				columnData<<devMode;
			}

			if (object_scene.contains("val")) 
			{  
				QJsonValue value = object_scene.value("val");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[lightArray]value : " << str;
					columnData<<str;
				}
			}
			else
			{
				QString str("0");
				//qDebug() << "[auto]lightArray]value : " << str;
				columnData<<str;
			}
			
            if (object_scene.contains("pow")) 
			{  
				QJsonValue value = object_scene.value("pow");
				if (value.isDouble()) 
				{  
					//QString str(value.toDouble());
					long intResult = value.toDouble();
					QString str=QString::number(intResult,10);
					//qDebug() << "[light]pow : " << str;
					columnData<<str;
					parents.last()->appendChild(new TreeItem(columnData, parents.last()));
				}
				else
				{
			       qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				}
			}
			else
			{
				QString str("0");				
				//qDebug() << "[auto]pow : " << str<<",length="<<columnData.length();
				columnData<<str;
				parents.last()->appendChild(new TreeItem(columnData, parents.last()));
			}
	    }
	}

	return 0;
}

int TreeModel::parse_ac(QJsonArray ac_array, TreeItem *parent)
{
    int i;
	int ret;
	QList<TreeItem*> parents;
    parents << parent;
    int nSize = ac_array.size();
	
	for (i = 0; i < nSize; i++)
	{
		QJsonValue value = ac_array.at(i);
		QList<QVariant> columnData;
		if (value.isObject())
		{
			QJsonObject object_scene = value.toObject();
			if (object_scene.contains("id")) 
			{  
				QJsonValue value = object_scene.value("id");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					qDebug() << "[acArray]id : " << str;
					columnData<<str;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				continue;
			}

			if (object_scene.contains("name")) 
			{  
				QJsonValue value = object_scene.value("name");	
				if (value.isString()) 
				{  				    
					QString strName = value.toString();  
					//qDebug() << "[acArray]name : " << strName;
					columnData<<strName;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				continue;
			}

			if (object_scene.contains("type")) 
			{  
				QJsonValue value = object_scene.value("type"); 
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[acArray]type : " << str;
					columnData<<str;
				}
			}
			else
			{
				//qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				continue;
			}

			if (object_scene.contains("val")) 
			{  
				QJsonValue value = object_scene.value("val");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[acArray]val : " << str;
					columnData<<str;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				QString str("25");  
					//qDebug() << "[acArray]val : " << str;
				columnData<<str;
				//continue;
			}
			
            if (object_scene.contains("pow")) 
			{  
				QJsonValue value = object_scene.value("pow");	
				if (value.isDouble()) 
				{  
					//QString str(value.toDouble());
					long intResult = value.toDouble();
					QString str=QString::number(intResult,10);
					//qDebug() << "[acArray]pow : " << str;
					columnData<<str;
				}
				else
				{
			       //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				//continue;
				QString str("0");  
					//qDebug() << "[acArray]val : " << str;
				columnData<<str;
			}

			if (object_scene.contains("spd")) 
			{  
				QJsonValue value = object_scene.value("spd");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[acArray]spd : " << str;
					columnData<<str;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				//continue;
				QString str("35");
				columnData<<str;
			}

			if (object_scene.contains("mod")) 
			{  
				QJsonValue value = object_scene.value("mod");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[acArray]mode : " << str<<", length"<<columnData.length();
					columnData<<str;
					parents.last()->appendChild(new TreeItem(columnData, parents.last()));
				}
			}
			else
			{
				//qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				QString str("1");  
				//qDebug() << "[acArray]mode : " << str;
				if (columnData.length() > 0)
				{
					columnData<<str;
					//qDebug() << "[acArray]length="<<columnData.length();
					parents.last()->appendChild(new TreeItem(columnData, parents.last()));
				}
			}
	    }
	}

	return 0;
}

int TreeModel::parse_floor(QJsonArray floor_array, TreeItem *parent)
{
    int i;
	int ret;
	QList<TreeItem*> parents;
    parents << parent;
    int nSize = floor_array.size();
	
	for (i = 0; i < nSize; i++)
	{
		QJsonValue value = floor_array.at(i);
		QList<QVariant> columnData;
		if (value.isObject())
		{
			QJsonObject object_scene = value.toObject();
			if (object_scene.contains("id")) 
			{  
				QJsonValue value = object_scene.value("id");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[floorArray]id : " << str;
					columnData<<str;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				continue;
			}

			if (object_scene.contains("name")) 
			{  
				QJsonValue value = object_scene.value("name");	
				if (value.isString()) 
				{  				    
					QString strName = value.toString();  
					//qDebug() << "[floorArray]devName : " << strName;
					columnData<<strName;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				continue;
			}

			if (object_scene.contains("type")) 
			{  
				QJsonValue value = object_scene.value("type"); 
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[floorArray]devType : " << str;
					columnData<<str;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				continue;
				//QString str("125");
				//columnData<<str;
			}

			if (object_scene.contains("val")) 
			{  
				QJsonValue value = object_scene.value("val");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[floorArray]value : " << str;
					columnData<<str;
				}
			}
			else
			{
				//qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				QString str("24");  
					//qDebug() << "[acArray]val : " << str;
				columnData<<str;
			}
			
            if (object_scene.contains("pow")) 
			{  
				QJsonValue value = object_scene.value("pow");	
				if (value.isDouble()) 
				{  
					//QString str(value.toDouble());
					long intResult = value.toDouble();
					QString str=QString::number(intResult,10);
					//qDebug() << "[acArray]on : " << str;
					columnData<<str;
				}
				else
				{
			      // qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
			      QString str("1");
				  columnData<<str;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
			    QString str("1");
				columnData<<str;
				//continue;
			}

			if (object_scene.contains("spd")) 
			{  
				QJsonValue value = object_scene.value("spd");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[floorArray]speed : " << str;
					columnData<<str;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				//continue;
				QString str("35");
				columnData<<str;
			}

			if (object_scene.contains("mod")) 
			{  
				QJsonValue value = object_scene.value("mod");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[floorArray]mode : " << str;
					columnData<<str;
					parents.last()->appendChild(new TreeItem(columnData, parents.last()));
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				QString str("1");  
				//qDebug() << "[acArray]mode : " << str;
				
				if (columnData.length() > 0)
					{
				columnData<<str;
				parents.last()->appendChild(new TreeItem(columnData, parents.last()));
					}
			}
	    }
	}

	return 0;
}

int TreeModel::parse_purifier(QJsonArray purifier_array, TreeItem *parent)
{
    int i;
	int ret;
	QList<TreeItem*> parents;
    parents << parent;
    int nSize = purifier_array.size();
	
	for (i = 0; i < nSize; i++)
	{
		QJsonValue value = purifier_array.at(i);
		QList<QVariant> columnData;
		if (value.isObject())
		{
			QJsonObject object_scene = value.toObject();
			if (object_scene.contains("id")) 
			{  
				QJsonValue value = object_scene.value("id");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[purifierArray]id : " << str;
					columnData<<str;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				continue;
			}

			if (object_scene.contains("name")) 
			{  
				QJsonValue value = object_scene.value("name");	
				if (value.isString()) 
				{  				    
					QString strName = value.toString();  
					//qDebug() << "[purifierArray]devName : " << strName;
					columnData<<strName;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				continue;
			}

			if (object_scene.contains("type")) 
			{  
				QJsonValue value = object_scene.value("type"); 
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[purifierArray]devType : " << str;
					columnData<<str;
				}
			}
			else
			{
			   // qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				continue;
			}

			if (object_scene.contains("val")) 
			{  
				QJsonValue value = object_scene.value("val");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[purifierArray]value : " << str;
					columnData<<str;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				//continue;
				
			    QString str("0");
				//qDebug() << "[auto][purifierArray]val : " << str;
				columnData<<str;
			}
			
            if (object_scene.contains("pow")) 
			{  
				QJsonValue value = object_scene.value("pow");	
				if (value.isDouble()) 
				{  
					//QString str(value.toDouble());
					long intResult = value.toDouble();
					QString str=QString::number(intResult,10);
					//qDebug() << "[acArray]on : " << str;
					columnData<<str;
				}
				else
				{
			      // qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				QString str("1");
				columnData<<str;
			}

			if (object_scene.contains("spd")) 
			{  
				QJsonValue value = object_scene.value("spd");	
				if (value.isString()) 
				{  
					QString str = value.toString();  
					//qDebug() << "[purifierArray]speed : " << str;
					columnData<<str;
					parents.last()->appendChild(new TreeItem(columnData, parents.last()));
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				//continue;
				QString str("35");
				
				if (columnData.length() > 0)
				{
					columnData<<str;
					parents.last()->appendChild(new TreeItem(columnData, parents.last()));
				}
			}
	    }
	}

	return 0;
}

int TreeModel::parse_room(QJsonArray room_array, TreeItem *parent)
{
    int i;
	int ret;
	int nSize = room_array.size();
	QList<TreeItem*> parents;
    parents << parent;
	//qDebug()<<"parse_room nSize="<<nSize;
	
	for (i = 0; i < nSize; i++)
	{
		QList<QVariant> columnData;
		QJsonValue value = room_array.at(i);
		if (value.isObject())
		{
			QJsonObject object_room = value.toObject();
			
			if (object_room.contains("id")) 
			{
				QJsonValue value = object_room.value("id");
				if (value.isString()) 
				{
					QString strFrom = value.toVariant().toString();
					//qDebug() << "[roomArray]roomId : " << strFrom;
					columnData << strFrom;
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
			    QString strFrom("1111");
				columnData << strFrom;
			}

			if (object_room.contains("name")) 
			{  // 包含指定的 homeName
				QJsonValue value = object_room.value("name");  // 获取指定 key 对应的 value
				if (value.isString()) 
				{  // 判断 value 是否为字符串
					QString devMode = QString::number(DEV_ROOM, 10); 
					QString strName = value.toString();  // 将 value 转化为字符串
					//qDebug() << "[roomArray]roomName : " << strName;
					columnData<<strName;
					columnData<<devMode;
					columnData<<"value";
					columnData<<"speed";
					//add room to house child
					if (object_room.contains("scenes") || object_room.contains("curtains")
						|| object_room.contains("airs") || object_room.contains("freshs")
						|| object_room.contains("floors") || object_room.contains("lights"))
					{
					    //qDebug() <<"room add"; 
						parents.last()->appendChild(new TreeItem(columnData, parents.last()));
					}
					else
					{
					    qDebug()<<"not add room";
					    continue;
					}
				}
			}
			else
			{
			   // qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
				return 0;
			}
			
			if (object_room.contains("scenes"))
			{
				QJsonValue value = object_room.value("scenes");   //level_3 scene
				
				if (value.isArray())
				{
					QJsonArray array_scene = value.toArray();
					if (parents.last()->childCount() > 0) 
					{
	                    parents << parents.last()->child(parents.last()->childCount()-1);
					}
					ret = parse_scene(array_scene, parents.last());	
					if (ret)
					{
					   // qDebug() << "parse_scene error ret=%d\n" << ret;
					}
					parents.pop_back();
				}
			}
			else
			{
			    //qDebug() <<"func="<<__func__<<","<<__LINE__<<". [qt_json err]"<<endl;
			}
#if 1
			if (object_room.contains("curtains"))
			{
				QJsonValue value = object_room.value("curtains");   //level_3 scene
				
				if (value.isArray())
				{
					QJsonArray array_curtain = value.toArray();
					if (parents.last()->childCount() > 0) 
					{
	                    parents << parents.last()->child(parents.last()->childCount()-1);
					}
					ret = parse_curtain(array_curtain, parents.last());	
					if (ret)
					{
					    qDebug() << "parse_curtain error ret=%d\n" << ret;
					}
					parents.pop_back();
				}
			}
#endif
			if (object_room.contains("airs"))
			{
				QJsonValue value = object_room.value("airs");   //level_3 scene
				
				if (value.isArray())
				{
					QJsonArray array_ac = value.toArray();
					if (parents.last()->childCount() > 0) 
					{
	                    parents << parents.last()->child(parents.last()->childCount()-1);
					}
					ret = parse_ac(array_ac, parents.last());	
					if (ret)
					{
					    //qDebug() << "parse_curtain error ret=%d\n" << ret;
					}
					parents.pop_back();
				}
			}
#if 1
			if (object_room.contains("lights"))
			{
				QJsonValue value = object_room.value("lights");   //level_3 scene
				
				if (value.isArray())
				{
					QJsonArray array_lights = value.toArray();
					if (parents.last()->childCount() > 0) 
					{
	                    parents << parents.last()->child(parents.last()->childCount()-1);
					}
					ret = parse_lights(array_lights, parents.last());	
					if (ret)
					{
					    //qDebug() << "parse_curtain error ret=%d\n" << ret;
					}
					parents.pop_back();
				}
			}
#endif
#if 1
			if (object_room.contains("freshs"))
			{
				QJsonValue value = object_room.value("freshs");   //level_3 scene
				
				if (value.isArray())
				{
					QJsonArray array_purifier = value.toArray();
					if (parents.last()->childCount() > 0) 
					{
	                    parents << parents.last()->child(parents.last()->childCount()-1);
					}
					ret = parse_purifier(array_purifier, parents.last());	
					if (ret)
					{
					    //qDebug() << "parse_curtain error ret=%d\n" << ret;
					}
					parents.pop_back();
				}
			}

			if (object_room.contains("floors"))
			{
				QJsonValue value = object_room.value("floors");   //level_3 scene
				
				if (value.isArray())
				{
					QJsonArray array_floor = value.toArray();
					if (parents.last()->childCount() > 0) 
					{
	                    parents << parents.last()->child(parents.last()->childCount()-1);
					}
					ret = parse_floor(array_floor, parents.last());	
					if (ret)
					{
					    //qDebug() << "parse_curtain error ret=%d\n" << ret;
					}
					parents.pop_back();
				}
			}
			#endif
		}
	}
	//qDebug()<<"room_parse in="<<i;
    return 0;
}
void TreeModel::createjs()
{
    QFile file("/home/user/ui/devjs");
	QJsonArray devArray;
	QJsonArray sceneArray;
	QJsonArray roomArray;
	QJsonObject json_home;
	QJsonObject json_room;
	QJsonObject json_scene;
	QJsonObject json_dev;
	json_dev.insert("devName", "floor");
	json_dev.insert("serialid", "1234");
	json_dev.insert("mode", "104");
	json_dev.insert("value", "3");
	json_dev.insert("speed", "1");
	
	devArray.append(json_dev);
	
	json_scene.insert("sceneName", "welcome");
	json_scene.insert("sceneId", "83DA0987-9B38-56AC-945C-8198A02C5F00");
	json_scene.insert("imageId", "1");
    json_scene.insert("devList", QJsonValue(devArray));

    sceneArray.append(json_scene);
	
    json_room.insert("roomId", "234");
    json_room.insert("roomName", "room");
    json_room.insert("sceneArray", QJsonValue(sceneArray));

    roomArray.append(json_room);
    json_home.insert("homeName", "涓囬噷鍒");
	json_home.insert("homeId", "83DA0987-9B38-56AC-945C-8198A02C5FA3");
	json_home.insert("roomArray", QJsonValue(roomArray));
	// 构建 JSON 文档
	QJsonDocument document;
	document.setObject(json_home);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	QString strJson(byteArray);

	//qDebug() << strJson;

	file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
	file.write(byteArray);
	//关闭文件
	file.close();
}

int TreeModel::parsejs(QByteArray byteArray)
{
	//parse
	int ret;
	QList<QVariant> rootData;
    QList<TreeItem*> parents;
	
	QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);

//	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<".";
	
//	qDebug()<<"in parsejs:"<<byteArray;

    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
	{  // 解析未发生错误
	    if (doucment.isObject())
		{ // JSON 文档为对象
	        QJsonObject object_home = doucment.object();  // level_1 home

	        if (object_home.contains("id")) 
			{
	            QJsonValue value = object_home.value("id");
	            if (value.isString()) 
				{
	                QString strFrom = value.toVariant().toString();
	               // qDebug() << "homeId : " << strFrom;
					rootData << strFrom;
	            }
				else
				{
					ENNO_LOG_ERR("[qt_json err]homeId parse err");
					return -1;
				}
	        }
			else
			{
				ENNO_LOG_ERR("[qt_json err]homeId err\n");
				return -1;
			}

			if (object_home.contains("name")) 
			{  // 包含指定的 homeName
	            QJsonValue value = object_home.value("name");  // 获取指定 key 对应的 value
	            if (value.isString()) 
				{  // 判断 value 是否为字符串
	                QString strName = value.toString();  // 将 value 转化为字符串
					QString devMode = QString::number(DEV_HOME, 10);           
	               // qDebug() << "homeName : " << strName;
					rootData << strName;
					rootData << devMode;
					rootData << "value";
					rootData << "speed";
					rootItem = new TreeItem(rootData);
					parents << rootItem;
	            }
	        }
			else
			{
				ENNO_LOG_ERR("[qt_json err]parse_room name error ret=%d\n");
			}
			
	        if (object_home.contains("rooms")) 
			{
	            QJsonValue value = object_home.value("rooms");  // level_2 rome
				if (value.isArray()) 
				{
				    QJsonArray room_array = value.toArray(); 
					ret = parse_room(room_array, parents.last()); 
					if (ret)
					{
					    qDebug() << "parse_room error ret=%d\n" << ret;
					}
				}
				else
				{
					ENNO_LOG_ERR("[qt_json err]parse_room error ret=%d\n", ret);
				}
	        }
	    }

		return 0;
	}
	else if (doucment.isNull())
	{
		ENNO_LOG_ERR("[qt_json err]doucment.isNull\n");
		return -1;
	}
	else if (jsonError.error != QJsonParseError::NoError)
	{
		ENNO_LOG_ERR("[qt_json err]jsonError.error=%d\n", jsonError.error);
		return -1;
	}

	return -1;
}
#endif
void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
	QString str;
	QString p_str;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
					p_str = (parents.last())->m_itemData[0].value<QString>();
					//qDebug() <<"pop_back pstr\n"<<p_str; 
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(columnData, parents.last()));
			
			//printf("strlist\n");
			
			for(int i = 0; i < columnData.count(); i++) 
			{
			    p_str = (parents.last())->m_itemData[0].value<QString>();
			    if (columnData[i].canConvert<QStringList>())
			    str = columnData[i].value<QString>();
                //qDebug()<<"add str["<<i + 1<<"] : "<<str<<","<<p_str<<endl; 
			}
			//printf("strlist end\n");
			
        }

        ++number;
    }
}
