#include "enno_encode.h"
#include <QFile>

#ifdef __cplusplus
	extern "C"
	{
#endif
//int *pulLen       encode used len;
int64_t GetTickCount()
{
	int64_t system_current_time = 0;
    struct timespec ts;
	int64_t sec;
	int64_t ns;
	
    clock_gettime(CLOCK_MONOTONIC, &ts);
	
	sec = ts.tv_sec;
	ns = ts.tv_nsec;
	system_current_time = (sec * 1000 + ns / 1000000);
	
    return system_current_time;
}

enno_result enno_encode_char(void *buf, char value, uint32 *pulLen, uint32 buf_len)
{
    ENNO_CHECK_NULL_RET(buf);
	ENNO_CHECK_NULL_RET(pulLen);
	ENNO_CHECK_PARA_RET(buf_len < sizeof(char));

    ((char *)buf)[0] = value;
	*pulLen = sizeof(char);
	
	return ENNO_OK;
}

enno_result enno_decode_char(void *buf, char *value, uint32 *pulLen, uint32 buf_len)
{
    ENNO_CHECK_NULL_RET(buf);
	ENNO_CHECK_NULL_RET(value);
	ENNO_CHECK_NULL_RET(pulLen);
	ENNO_CHECK_PARA_RET(buf_len < sizeof(char));

    *value = *((char *)buf);
	*pulLen = sizeof(char);
	
	return ENNO_OK;
}
/*************************************************
  Function:       enno_encode_uint
  Description:    unsigned int data encode
  Calls:          
  Called By:      
  Input:    _uint32 value     data ready to encode
            _uint32 buf_len   buf length for encode, check the buf is enough
  Output:   void *buf         memory buf 
            _uint32 *pulLen   encode used buf length
  Return:   enno_result  ENNO_OK(0):ok  
                         other:error
  Others:         
*************************************************/
enno_result enno_encode_uint(void *buf, uint32 value, uint32 *pulLen, uint32 buf_len)
{
    ENNO_CHECK_NULL_RET(buf);
	ENNO_CHECK_NULL_RET(pulLen);
	ENNO_CHECK_PARA_RET(buf_len < sizeof(uint32));

    ((uint32 *)buf)[0] = value;
	*pulLen = sizeof(uint32);
	
	return ENNO_OK;
}


/*************************************************
  Function:       enno_encode_uint
  Description:    unsigned int data encode
  Calls:          
  Called By:      
  Input:    _uint32 value     data ready to encode
            _uint32 buf_len   buf length for encode, check the buf is enough
  Output:   void *buf         memory buf 
            _uint32 *pulLen   encode used buf length
  Return:   enno_result  ENNO_OK(0):ok  
                         other:error
  Others:         
*************************************************/
enno_result enno_encode_uint_hkt(void *buf, uint32 value, uint32 *pulLen, uint32 buf_len)
{
    ENNO_CHECK_NULL_RET(buf);
	ENNO_CHECK_NULL_RET(pulLen);
	ENNO_CHECK_PARA_RET(buf_len < sizeof(uint32));

    *((uint32 *)buf) = ((value&0xff)<<24) + (((value>>8)&0xff)<<16) 
                       + (((value>>16)&0xff)<<8) + ((value>>24)&0xff);
	*pulLen = sizeof(uint32);
	
	return ENNO_OK;
}

enno_result enno_read_uint_from_file(uint32 *int_val,QString file_name,qint8 read_len)
{
    char tmp[8];
    int nbytes = 1;
    QFile file(file_name);


    if (!file.open(QFile::ReadOnly))
    {
        qDebug()<<"fileopen fail";
        return ENNO_UNKOWN;
    }
    nbytes = file.read(tmp,read_len);

    if (!nbytes)
    {
        *int_val = 0;
        file.close();
        return ENNO_UNKOWN;
    }

    *int_val = atoi(tmp);

    qDebug()<<"read int="<<*int_val;
    file.close();

    return ENNO_OK;
}

enno_result enno_decode_uint(void *buf, uint32 *value, uint32 *pulLen, uint32 buf_len)
{
    ENNO_CHECK_NULL_RET(buf);
	ENNO_CHECK_NULL_RET(value);
	ENNO_CHECK_NULL_RET(pulLen);
	ENNO_CHECK_PARA_RET(buf_len < sizeof(uint32));

    *value = *((uint32*)buf);
	*pulLen = sizeof(uint32);
	
	return ENNO_OK;
}

enno_result enno_encode_str(void *buf, char *value, uint32 *pulLen, uint32 buf_len)
{
    uint32 str_len = 0;
	enno_result ret = ENNO_OK;
	uint32 size_buf;
	
    ENNO_CHECK_NULL_RET(buf);
	ENNO_CHECK_NULL_RET(value);
	ENNO_CHECK_NULL_RET(pulLen);

    str_len = strlen(value);
	if (0 == str_len)
	{
	    return ENNO_MSGBUF_NULL;
	}
	ENNO_CHECK_PARA_RET(buf_len < str_len + sizeof(uint32));
	
	ret = enno_encode_uint(buf, str_len, &size_buf, buf_len-sizeof(uint32));
	if (ret != ENNO_OK)
	{
	    return ENNO_UNKOWN;
	}

    buf += 	size_buf;
	memcpy(buf, value, str_len);
	*pulLen = size_buf + str_len;    //update the rest len;
	return ENNO_OK;
}

enno_result enno_decode_str(void *buf, char *value, uint32 *pulLen, uint32 buf_len)
{
    uint32 str_len = 0;
	enno_result ret = ENNO_OK;
	uint32 size_buf;
	
    ENNO_CHECK_NULL_RET(buf);
	ENNO_CHECK_NULL_RET(value);
	ENNO_CHECK_NULL_RET(pulLen);
	ENNO_CHECK_PARA_RET(buf_len < sizeof(uint32));

    ret = enno_decode_uint(buf, &str_len, &size_buf, buf_len);
	if (0 == str_len)
	{
	    return ENNO_MSGBUF_NULL;
	}
	//ENNO_CHECK_PARA_RET(buf_len < str_len + size_buf);
	if (buf_len < str_len + size_buf)
	{
	    ENNO_LOG_ERR("buf_len=%d, str_len=%d, size_buf=%d\n", buf_len, str_len, size_buf);
		return ENNO_UNKOWN;
	}
	memcpy(value, &((char *)buf)[size_buf], str_len);
	value[str_len] = 0;
	*pulLen = size_buf + str_len;
	return ENNO_OK;
}



#ifdef __cplusplus
    }
#endif


