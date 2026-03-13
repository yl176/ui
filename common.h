#ifndef COMMON_H
#define COMMON_H

#include <time.h>
#include <stdio.h>
#include "util.h"
#include <syslog.h>

#include <QSettings>

#define ENNO_HILINK_AUTH 1

#ifndef uint32
typedef unsigned int    uint32;
#endif

#ifdef BIL_LIB        
# define BIL_SHARE Q_DECL_EXPORT        
#else        
# define BIL_SHARE Q_DECL_IMPORT        
#endif     

#if defined(__linux)&&defined(__x86_64)
#define PAIR_OK_FILE "/home/orchid/qt/config/paired_flag"
#elif defined(_WIN32)
#define PAIR_OK_FILE "C:\\work\\p2\\code\\config\\paired_flag"
#else
#define PAIR_OK_FILE "/home/user/ui/qwid/paired_flag"
#endif


#define AQI_HIDE_FILE "/home/user/conf/aqi_hide_flag"


#if defined(__linux)&&defined(__x86_64)
#define START_FLAG_FILE "/home/orchid/qt/config/flash_flag"
#elif defined(_WIN32)
#define START_FLAG_FILE "C:\\work\\p2\\code\\config\\flash_flag"
#else
#define START_FLAG_FILE "/home/user/ui/qwid/flash_flag"
#endif

#if defined(__linux)&&defined(__x86_64)
#define HOUSE_SET_FILE "/home/orchid/qt/config/house_flag"
#elif defined(_WIN32)
#define HOUSE_SET_FILE "C:\\work\\p2\\code\\config\\house_flag"
#else
#define HOUSE_SET_FILE "/home/user/ui/qwid/house_flag"
#endif

#if defined(__linux)&&defined(__x86_64)
#define LOCK_STATUS_FILE "/home/orchid/qt/config/lock_status"
#elif defined(_WIN32)
#define LOCK_STATUS_FILE "C:\\work\\p2\\code\\config\\lock_status"
#else
#define LOCK_STATUS_FILE "/home/user/conf/lock_status"
#endif

#define BOOT_MODE_FILE "/home/user/conf/boot_mode"
#define SCENE_ANDROID_CONF "/home/user/conf/pannos_android.conf"

#define CTL_METAL_BUTTON_SECTION "metal_scene"
#define CTL_METAL_FILE "/home/user/conf/ctl_metal.ini"
#define CFG_CHILD_LOCK_FILE "/home/user/conf/child_lock"


extern int g_language_zh;
extern int g_aqi_value;

#define OLED_PIX_X  320
#define OLED_PIX_Y  320

#define MAX_SCENE 256

#define DEF_RESET_TIME 35

//#define FPGA_TEST

#ifdef FPGA_TEST
#define TIMER_STANDBY_MS 2000
#define TIMER_SLEEP_MS   820000
#define TIMER_SET_MS     6000
#define TIMER_DIM_OUT_MS 400
#define TIMER_BAR_MS 200
#else
#define TIMER_STANDBY_MS 60000
#define TIMER_SLEEP_MS   120000
#define TIMER_SET_MS     8000
#define TIMER_DIM_OUT_MS 3000
#define TIMER_BAR_MS 1200
#endif

#define TIMER_LIGHT_OUT_MS 8000

#define TIME_LIGHT_CLICK_MS 500

#define TIMER_UPDATE_MS 5000
#define TIMER_RATAT_MS 50
#define TIMER_CHECK_MS 2000

#define TIMER_TICK_ONE 1000

#define TIMER_DIM_OUT_MS 5000

#define DEVICE_ID_LEN 24
#define DEVICE_VERSION_LEN 24
#define SSID_LEN 128
#define IP_LEN 64

#define TIME_OF_SCENE_SWITCH 200
#define TIME_OF_SCENE_DELAY 40

#define CTL_IS_BIT_SET(data, b)  (((data)>>(b))&0x1)

typedef struct
{
    uint32 device_id;
	uint32 device_type;
	uint32 cid;
	uint32 value;
}CTL_DEVICE_CONF;


typedef enum {
    LeftToRight,
    RightToLeft
}Direction;

typedef enum
{
    F_UI_SCENE_CTRL          = 0x12100001,
	F_UI_SCENE_DIM_VALUE     = 0x12100002,
    F_UI_SET_CONFIG          = 0x12100003,
    F_UI_DEV_CTL             = 0x12100004,
    F_UI_CONF_REQ            = 0x12100005,
    F_UI_FACTORY_REQ         = 0x12100006,
    F_UI_SET_BOOT_MODE       = 0x12100007,
    F_UI_WIFI_RESET_REQ      = 0x12100008,/*wifi reset request*/
    F_UI_DIM_START           = 0x12100009,
    F_UI_DIM_STOP            = 0x1210000A,
    F_UI_DIM_LIGHT           = 0x1210000B,
    F_UI_CUR_SWITCH          = 0x1210000C,
    F_UI_REBOOT_QT           = 0x1210000D,
    F_UI_METAL_BUTTON        = 0x1210000E,
	F_UI_GET_DEV_INFO_CMD    = 0x1210000F,
	F_UI_CYCLE_LIGHT_DIM     = 0x12100010,
	F_UI_MENU_CHANGE         = 0x12100011,
	F_UI_IS_WAKE_UP          = 0x12100012,
	F_UI_TOUCH_EVENT         = 0x12100013,
	F_UI_METAL_LIGHT_ONOFF   = 0x12100014,
	F_UI_METAL_BUTTON_PRESS  = 0x12100015,
    
    T_UI_UPDATE              = 0x10120001,
    T_UI_RESET               = 0x10120002,
    T_UI_CONF_UPDATE         = 0x10120003,
    T_UI_GET_BOOT_MODE       = 0x10120004,
    T_UI_ADD_OK              = 0x10120005,
    T_UI_BRIDGE_IDENTIFY     = 0x10120006,
    T_UI_BRIDGE_IDENTIFY_END = 0x10120007,
	T_UI_DISPLAY_PAIRID      = 0x10120008,
	T_UI_CLR_PAIRID          = 0x10120009,
	T_UI_SET_SCENE_DIM       = 0x1012000A,
	T_UI_SCENE_UPDATE        = 0x1012000B,
	T_UI_GET_SENSOR_VAL      = 0x1012000C,
	T_UI_FIRMWARE_UPDATE     = 0x1012000D,
	T_UI_DIM_LIGHT           = 0x1012000E,
    T_UI_GET_DEV_INFO        = 0x1012000F,
    T_UI_DEV_OPEN_NOTIRY     = 0x10120010,
    T_UI_ENTER_PROGRAM       = 0x10120011,
    T_UI_EXIT_PROGRAM        = 0x10120012,
    T_UI_METAL_BUTTON        = 0x10120013, 
    T_UI_NET_STATE           = 0x10120014,
    T_UI_DISPLAY_SSID        = 0x10120015,
    T_UI_SEND_LOCK_STATUS    = 0x10120016,
    T_UI_DALI_AUTO_ADDR_RESP = 0x10120017,
    T_UI_CYCLE_LIGHT_DIM     = 0x10120018,
    T_UI_SCENE_NAME_UPDATE   = 0x10120019,
    T_UI_METAL_BUTTON_PRESS  = 0x1012001A,
    T_UI_METAL_BUTTON_RELEASE  = 0x1012001B,


    T_UI_FLUTTER_CLR_PAIRID    = 0x1012001C,



	T_SYS_DEV_UPDATE = 0x10ff0001,
	
	F_UI_QT_WAKE_UP  = 0x12120001,
	F_UI_LIGHT_ON    = 0x12120005,
	F_UI_MSP_MSG	 = 0x12120006,
    F_UI_LIGHT_STATUS = 0x12120007,
    
	
	
}MSG_DEF_TYPE;

typedef enum
{
    AC_FAN_LOW,
    AC_FAN_MIDDLE,
    AC_FAN_HIGH,
    AC_FAN_AUTO,
}AC_FAN_SPEED_DEF;

//device char
typedef enum
{
    //dali light
    CHAR_LIGHT_ON = 100,
    CHAR_LIGHT_BRIGHTNESS,
    CHAR_LIGHT_ON_STATE,
    CHAR_LIGHT_BRIGHTNESS_STATE,
    CHAR_LIGHT_DIMMER,
    CHAR_LIGHT_COLOR_TEMP,    //
    CHAR_LIGHT_SATURATION,

    //heater cooler
    CHAR_AC_ACTIVE = 300,  //300
    CHAR_AC_CUR_TEMPERATURE,     //
    CHAR_AC_TARGET_HEATER_COOLER_STATE,  //
    CHAR_AC_CURRENT_HEATER_COOLER_STATE,  //
    CHAR_AC_TARGET_TEMPERATURE, //
    CHAR_AC_ROTATION_SPEED,  //
    CHAR_AC_ACTIVE_STATE,    
    CHAR_AC_TARGET_TEMPERATURE_STATE,
    CHAR_AC_ROTATION_SPEED_STATE,
    CHAR_AC_COOLING_TH_TEMP,  //
    CHAR_AC_HEATING_TH_TEMP,  //
    CHAR_AC_TEMP_UINT, //
    CHAR_AC_CURRENT_HEATING_COOLING_STATE,  //
    CHAR_AC_TARGET_HEATING_COOLING_STATE,  //
    CHAR_AC_FLOOR_HEAT_AUTO_MAN_CTRL,      //314

    //curtain
    CHAR_SHADE_ON = 400,    //
    CHAR_SHADE_TARGET_POSITION,
    CHAR_SHADE_CURRENT_POSITION,
    CHAR_SHADE_ON_STATE,   //
    CHAR_SHADE_HOLD_POSITION,
    CHAR_SHADE_POSITION_STATE,

	//fresh air
    CHAR_AP_ACTIVE = 500,  //
    CHAR_AP_ACTIVE_STATE,
    CHAR_AP_TARGET_AIR_PURIFIER_STATE,
    CHAR_AP_CYCLE_MODE,
    CHAR_AP_CURRENT_AIR_PURIFIER_STATE,
    CHAR_AP_CYCLE_MODE_STATE,
    CHAR_AP_ROTATION_SPEED,
    CHAR_AP_ROTATION_SPEED_STATE,
    CHAR_AP_2LV_SPEED,
    CHAR_AP_2LV_SPEED_STATE,
    CHAR_AP_3LV_LOW_SPEED,
    CHAR_AP_3LV_LOW_SPEED_STATE,
    CHAR_AP_3LV_MEDIUM_SPEED,
    CHAR_AP_3LV_MEDIUM_SPEED_STATE,
    CHAR_AP_3LV_HIGH_SPEED,  
    CHAR_AP_3LV_HIGH_SPEED_STATE,
    CHAR_AP_AIR_QUALITY,//
    CHAR_AP_PM25_DENSITY,
    CHAR_AP_VOC_DENSITY,
    CHAR_AP_CARBON_DIOXIDE_LEVEL,

	//dmx light
    CHAR_DMX_ON = 600,//
    CHAR_DMX_BRIGHTNESS, //
    CHAR_DMX_ON_STATE,   //
    CHAR_DMX_BRIGHTNESS_STATE, //
    CHAR_DMX_RGB, //
    CHAR_DMX_HUE,  //
    CHAR_DMX_SATURATION, //
    CHAR_DMX_UI_ON_OFF,
    CHAR_DMX_MODE,  //ali iot
    CHAR_DMX_COLOUR,//ali iot
    CHAR_DMX_COLOR_MODE,
    CHAR_DMX_COLOR_ARRAY_INFO,

    //KNX switch
    CHAR_SW_ON = 700,  //69
    CHAR_SW_ON_STATE,

    //P2 Intergrated
    CHAR_P2_TEMP_SENSOR = 800, //
    CHAR_P2_HUMIDITY_SENSOR,
    CHAR_P2_GAS_SENSOR,
    CHAR_P2_OCCUPANCY_SENSOR, //
    CHAR_P2_OUTDOOR_TEMP,
    CHAR_P2_OUTDOOR_AQI,
    CHAR_P2_OUTDOOR_WEATHER,
    CHAR_P2_OUTDOOR_PM25,
    CHAR_P2_METAL_BUTTON,  //
    CHAR_P2_METAL_BUTTON_FEEDBACK,
    CHAR_P2_DIM_KOB,
    CHAR_P2_DIM_KOB_STATE,
    CHAR_P2_METAL_BUTTON_SCENE_CONTROL,
    CHAR_P2_TARGET_SCENE,  //
    CHAR_P2_CURRENT_SCENE,
    CHAR_P2_SCENE_SERVER,
    CHAR_P2_SCENE_CONF,
    CHAR_P2_SCENE_CTRL,
    CHAR_P2_AIR_QUALITY_SENSOR,//
    CHAR_P2_VOC_SENSOR,
    CHAR_P2_LIGHT_SENSOR,

	CHAR_TYPE_NAME = 900,   
}EN_CHAR_TYPE_DEF;

typedef enum
{
	EN_SET_NETWORK_FIX,   //Network Fix", to arm
	EN_SET_DALI_ADDR,     //"Dali addr set" to arm
	EN_SET_FACTORY_RESET, //"Factory Reset" to arm
    EN_SET_ETS_MODE,     //"Boot Mode"  to arm
    EN_SET_SETUP_CODE,    //"Setup Code"
    EN_SET_LANGUAGE,      //"Language"
    EN_SET_ABOUT,         //"about"
    EN_SET_BOOT_MODE,
    EN_SET_WAC_RESET,
    EN_SET_NET_REBOOT,
    EN_SET_MAX_ITEM,
    EN_SET_IDENTIFY_CH1,
    EN_SET_IDENTIFY_CH2,
    EN_SET_IDENTIFY_STOP,
    EN_SET_DALI_RECONFIGURATION,
}EN_SET_ITEM_DEF;

typedef enum{
	DALI_CH1_ADDR_RESET_ALL=1,
	DALI_CH1_ADDR_SET_FOR_NEW,
	DALI_CH2_ADDR_RESET_ALL,
	DALI_CH2_ADDR_SET_FOR_NEW,

}DALI_SET_ADDR_DEF;

typedef struct
{
    int channel;    //CHANNEL_DEF
    int dev_id;     //
    int c_id;       //EN_CHAR_TYPE_DEF
    int dev_type;   //DEV_TYPE_DEF
    int value;
}CTL_MSG_CONF;

typedef enum
{
    EN_NET_UNCONNECT,
	EN_NET_UNPAIRED,
	EN_NET_PAIRED,
	EN_NET_CONNECTED,
	EN_NET_FIXED,
	EN_NET_MAX,
}EN_NET_STATE;

#define offsetof(t,m) ((size_t)&((t *)0)->m)

#define UI_TO_QT_HEAD_FLAG 0x5566a55a
#define UI_MAX_DATA_LENGTH 1024
//,public QThread

#define CHANGE_TO_LITTLE_ENDIAN(in)  ((((in)&0xff)<<24) \
	+ ((((in)>>8)&0xff)<<16) + ((((in)>>16)&0xff)<<8) + (((in)>>24)&0xff))


typedef struct ST_MSG_TYPEtag
{
    unsigned int head;
	unsigned int length;
	unsigned char data[UI_MAX_DATA_LENGTH];
}ST_MSG_TYPE;

#if defined(__linux)&&defined(__x86_64)
#define LANGUAGE_CONF "/home/orchid/qt/config/language.conf"
#elif defined(_WIN32)
#define LANGUAGE_CONF "C:\\work\\p2\\code\\config\\language.conf"
#else
#define LANGUAGE_CONF "/home/user/ui/qwid/language.conf"
#endif

#if defined(__linux)&&defined(__x86_64)
#define SCENE_ROOM_CONF "/home/orchid/qt/config/pannos_scene_room.conf"
#elif defined(_WIN32)
#define SCENE_ROOM_CONF "C:\\work\\p2\\code\\qwid\\pannos_scene_room.conf"
#else
#define SCENE_ROOM_CONF "/home/user/conf/pannos_scene_room.conf"
#endif

#define SCENE_SHOW_CONF "/home/user/conf/scene_show"
#define SCENE_SHOW_DEF_CONF "/home/user/conf/scene_show_def"


#define SCENE_ROOM_BAK_CONF "/home/user/conf/pannos_scene_room_bak.conf"
#define SCENE_ROOM_DEF_CONF "/home/user/conf/pannos_scene_room_def.conf"


extern pthread_mutex_t mutex_turnPage;
extern int g_upSreenTurning;
extern int g_BottomSreenTurning;

#define BIT_AC 0x01
#define BIT_FLOOR 0x02

void unlockTurnPageMutex(QWidget * widgetIn);

int needEnterLockStatus(void);

typedef enum
{
    EN_USER_ROOT      = 0,
	EN_USER_GUEST     = 1,
    EN_USER_NO_ACCESS = 2,
}EN_USER_RIGHT;

/**************/
#define ENNO_DEBUG_ERR  0x1
#define ENNO_DEBUG_WARN 0x2
#define ENNO_DEBUG_INFO 0x4

#define ENNO_DEBUG_LEVEL      0x1

extern int g_enno_debug_level;

#define ENNO_LOG_INFO(fmt,...) \
		do{\
			if (g_enno_debug_level&ENNO_DEBUG_INFO)\
			{\
				syslog(LOG_INFO|LOG_LOCAL1, fmt, ##__VA_ARGS__);\	
			}\
		}while(0)

#define ENNO_LOG_WARN(fmt,...) \
		do{\
			if (g_enno_debug_level&ENNO_DEBUG_WARN)\
			{\
			    printf("[enno_warn:func=%s,%d," fmt, __func__, __LINE__, ##__VA_ARGS__);\	 
			}\
			syslog(LOG_INFO|LOG_LOCAL1, fmt, ##__VA_ARGS__);\	
		}while(0)

#define ENNO_LOG_ERR(fmt,...) \
	do{\
		if (g_enno_debug_level&ENNO_DEBUG_ERR)\
		{\
		    printf("[enno_error]:func=%s,%d," fmt, __func__, __LINE__, ##__VA_ARGS__);\	    
		    syslog(LOG_INFO|LOG_LOCAL1, fmt, ##__VA_ARGS__);\	    
	    }\
	}while(0)



#endif
