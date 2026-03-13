#ifndef _ENNO_ENCODE_H_
#define _ENNO_ENCODE_H_

#include <stdio.h>  
#include <stdlib.h>   
#include <string.h>  
#include "common.h"


//#include "platform_common.h"
typedef enum  
{
    ENNO_OK = 0,
    ENNO_FID_ERR = 1,
    ENNO_FID_NUM_ERR = 2,
    ENNO_PID_ERR = 3,
    ENNO_REGPID_ERR = 4,           /* PID注册失败 */
    ENNO_MEMALLOC_ERR = 5,         /* 内存申请失败 */
    ENNO_MSGSYNCSEND_ERR = 6,      /* 任务消息同步发送失败 */
    ENNO_PISTASKCREAT_ERR = 7,     /* 创建PID任务失败 */
    ENNO_MSGBUF_NULL = 8,          /* 消息缓冲区为空 */
    ENNO_INPUTPTR_NULL = 9,        /* 函数入参指针为空 */
    ENNO_MSGBUG_OVERWRITE = 10,     /* 消息写越界 */
    ENNO_FILENAME_OVER = 11,        /* 超长文件名 */
    ENNO_READFILE_ERR = 12,			/* 文件读错误 */
	ENNO_OPENDIR_ERR = 13,			/* 文件打开出错 */
    ENNO_SYSLOGFEED_ERR = 14,		/* 文件上传报文返回失败 */
    ENNO_SYSLOGREAD_ERR = 15,     /* 日志文件读失败,大小为0或者文件不存在 */
    ENNO_INIT_SEM_ERR = 16,       /* 初始化信号量失败 */
    ENNO_HASH_ERR = 17,           /* 初始化信号量失败 */
    ENNO_EVENTMSGTYPE_ERR = 18,   /* 事件消息类型不对 */
    ENNO_MSGSTATIC_CANTFINDPID = 19,	/* 消息统计时找不到目的PID */
    ENNO_MSGSEND_CANTFINDPID=20,      /* 消息发送时找不到目的PID */
    ENNO_BOARDMSG_CANTFINDPID=21,     /* 发送广播消息找不到目的PID */
    ENNO_INTPUT_ERR=22,     			/* 函数入参出错 */
    ENNO_JSON_PARSE_ERR=23,     /*json parse error*/
    ENNO_UNKOWN=0xffffffff,    /* 未知错误 */
}enno_result;


#ifdef __cplusplus
extern "C"
{
#endif
int64_t GetTickCount();
enno_result enno_encode_char(void *buf, char value, uint32 *pulLen, uint32 buf_len);
enno_result enno_decode_char(void *buf, char *value, uint32 *pulLen, uint32 buf_len);
enno_result enno_encode_uint(void *buf, uint32 value, uint32 *pulLen, uint32 buf_len);
enno_result enno_decode_uint(void *buf, uint32 *value, uint32 *pulLen, uint32 buf_len);
enno_result enno_encode_str(void *buf, char *value, uint32 *pulLen, uint32 buf_len);
enno_result enno_decode_str(void *buf, char *value, uint32 *pulLen, uint32 buf_len);
enno_result enno_encode_uint_hkt(void *buf, uint32 value, uint32 *pulLen, uint32 buf_len);
enno_result enno_read_uint_from_file(uint32 *int_val,QString file_name,qint8 read_len=4);

#define ENNO_CHECK_NULL_RET(ptr) \
	if (NULL == ptr)\
	{\
		printf("[error]func=%s,%d, nullptr\n", __func__, __LINE__);\
	    return ENNO_INPUTPTR_NULL;\
	}

#define ENNO_CHECK_RET(ret) \
	if (ret != ENNO_OK)\
	{\
		printf("[error]func=%s,%d, ret=%d\n", __func__, __LINE__, ret);\
	    return ENNO_UNKOWN;\
	}

#define ENNO_CHECK_NORTN_RET(ret) \
	if (ret != ENNO_OK)\
	{\
		printf("[error]func=%s,%d, ret=%d\n", __func__, __LINE__, ret);\
	    return;\
	}

#define ENNO_CHECK_PARA_RET(contion) \
	if (contion)\
	{\
		printf("[error]func=%s,%d, contion err\n", __func__, __LINE__);\
	    return ENNO_UNKOWN;\
	}
	

#ifdef __cplusplus
}
#endif

#endif 


