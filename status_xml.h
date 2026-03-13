#ifndef _STATUS_XML_H
#define _STATUS_XML_H


#if defined(__linux)&&defined(__x86_64)
#define xmlFileName "/home/orchid/qt/qwid/status.xml"
#elif defined(_WIN32)
#define xmlFileName "C:\\work\\p2\\code\\config\\status.xml"
#else
#define xmlFileName "/home/user/ui/qwid/status.xml"
#endif


#include <QFile>
#include <QString>
#include <QList>


enum
{
    DEV_TYPE_CURTAIN,
    DEV_TYPE_HAVC
};

typedef struct __OutDoorValues
{
    unsigned int temperature;
    unsigned int aqi;
    unsigned int weather;
}OutDoorValues;


typedef struct __InDoorValues
{
    unsigned int temp;
    unsigned int hum;
    unsigned int voc;
}InDoorValues;


#define BIT_TEMP 0x01
#define BIT_AQI  0x02
#define BIT_WEATHER 0x04

#define BIT_TEMP_IN 0x01
#define BIT_HUM_IN  0x02
#define BIT_VOC_IN 0x04


typedef struct __DevsXml
{
    unsigned int roomId;
    unsigned int devType;
    unsigned int devId;
    unsigned int cid;
    unsigned int val;
}DevsXml;

typedef struct __LightXml
{
    unsigned int roomId;
    unsigned int lightStatus;
	unsigned int lightDim;
}LightXml;

typedef struct __SceneXml
{
    unsigned int roomId;
    unsigned int sceneId;
    unsigned int sceneStatus;
}SceneXml;

typedef QList<DevsXml> DevList;
typedef QList<DevsXml>::iterator It_DevList;
typedef QList<LightXml> LightStatusList;
typedef QList<SceneXml> SceneStatusList;

class CStatusXml
{
private:
   QFile file;
   int roomIndex;
   int isReadOut;
public:
   CStatusXml(const char * fileName);
   void updateOutDoor(int enumIn,unsigned int val);
   void updateInDoor(int enumIn,unsigned int val);
   void updateOpenDev(unsigned int val);
   void updateSetup(unsigned int val);
   void setRoomIndex(int roomI);
   void setReadOut(int flag);
   int  getReadOut(void);
   void updateDevInfo(int devType,unsigned int devId,unsigned int cid,unsigned int val);
   void updateScene(unsigned int scenceId,unsigned int val);
   void updateLight(unsigned int val);
   void updateLightDim(unsigned int val);
   void updateSsid(char *p_ap_name);
   void updateip(char *p_ip_name);
   void updatedb(char *p_db_name);
/*   void addSceneStatus(int roomIndex,const char * buff, int len);
   void addDev(int roomIndex,const char * buff, int len);
   void addSensor(const char * buff, int len);
   void addFwUpdate(const char * buff, int len);
   void addDimLight(const char * buff, int len);
   void addPannelInfo(const char * buff, int len);
   void addOpenDev(const char * buff, int len);
   void addProgram(const char * buff, int len);
   void updateLightStatus(int roomIndex,const char * buff, int len);
   void addNetStatus(const char * buff, int len);
   void updateNetSSID(const char * buff, int len);*/
   void init_status(void);
   void getAllItems(void);
   void getOutDoorValues(char* bitMapOut,OutDoorValues*pstOutDoorValue);
   void getInDoorValues(char* bitMapOut, InDoorValues*pstInDoorValue);
   void getOpenDev(unsigned int * bitMapOut);
   void getSetup(unsigned int * bitMapOut);
   void getSsid(char *pp_ap_name);
   void getip(char *pp_ip_name);
   void getdb(char *pp_db_name);
   void getAllDevs(DevList& devListOut,LightStatusList& lightListOut,SceneStatusList &listScenesOut);
         
};

extern CStatusXml * g_statusXmlHandle;


#endif
