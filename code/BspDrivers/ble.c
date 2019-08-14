#include "ble.h"
#include "hy40r204.h"

/*做主机时，连接从机操作*/
unsigned char connection_com[10] = "AT+CON#1\r\n";
/*做主机时，连接从机的MAC地址*/
unsigned char connet_mac[6] = {0x84,0xDD,0x20,0x01,0x03,0x36};
volatile unsigned char slave_num = 0;            //扫描到的从机个数
/*存放扫描到的从机MAC地址*/
volatile unsigned char slave_mac[8][6]={0};


void delaynms(uint16_t time)
{
  uint16_t num;
  for(;time>0;time--)
  {
    for(num=2030;num>0;num--)
    ;
  }
}

/*********************************************************************
*函 数 名：BLE_test
*参    数：NONE
*返 回 值：NONE
*功能说明：发送字符串
*********************************************************************/
unsigned char ble_test(void)
{
    hy40r204_wake_con(HY40R204_WAKE_OK);
	delay_ms(1);
	ble_slave_mac_show();
	hy40r204_wake_con(HY40R204_WAKE_NOT_OK);
	return(0);   
}

/*********************************************************************
*函 数 名：ble_host_scanf_slave
*参    数：NONE
*返 回 值：
*功能说明：开始扫描从设备
*********************************************************************/
unsigned char ble_host_scanf_slave(void)
{
	hy40r204_wake_con(HY40R204_WAKE_OK);
	delaynms(2);
    hy40r204_send_data("AT+SCA=START\r\n",strlen("AT+SCA=START\r\n"));
    delaynms(10);
	hy40r204_wake_con(HY40R204_WAKE_NOT_OK);
	return(0);
}

/*********************************************************************
*函 数 名：ble_host_connect_slave
*参    数：mac->从机设备mac地址,connt->1：连接，0：断开连接
*返 回 值：NONE
*功能说明：连接从设备
*********************************************************************/
unsigned char ble_host_connect_slave(unsigned char connt,unsigned char slavenumb,unsigned char *mac)
{
    unsigned char i;

	hy40r204_wake_con(HY40R204_WAKE_OK);
	delaynms(2);
    for(i=0;i<slavenumb;i++)
    {
        if(strncmp((char*)&connet_mac[0],(char*)slave_mac[i],6) == 0)
        {
            connection_com[7] = (i+0x31);
            hy40r204_send_data(connection_com,10);
            ble_data_mode();
            break;
        }
    }
	hy40r204_wake_con(HY40R204_WAKE_NOT_OK);
	return(0);
}

/*********************************************************************
*函 数 名：ble_data_mode
*参    数：
*返 回 值：NONE
*功能说明：选择工作模式
*********************************************************************/
void ble_data_mode(void)
{
    hy40r204_wake_con(HY40R204_WAKE_OK);
    delaynms(2);
    hy40r204_send_data("AT+MODE=DATA\r\n",strlen("AT+MODE=DATA\r\n"));
    hy40r204_wake_con(HY40R204_WAKE_NOT_OK);
}

/*********************************************************************
*函 数 名：ble_host_init
*参    数：NONE
*返 回 值：
*功能说明：
*********************************************************************/
unsigned char ble_host_init(void)
{
//	 hy40r204_send_data("AT+SOFT_RST=1\r\n",sizeof("AT+SOFT_RST=1\r\n"));
//	 delay_ms(100);
    hy40r204_wake_con(HY40R204_WAKE_OK);
    delaynms(2);
    hy40r204_send_data( "AT\r\n", strlen("AT\r\n") );
    delaynms(200);
    hy40r204_send_data("AT+SET=0\r\n",strlen("AT+SET=0\r\n"));
    delaynms(10);
//hy40r204_wake_con(HY40R204_WAKE_NOT_OK);
    return 0;
}

/*********************************************************************
*函 数 名：ble_slave_init
*参    数：NONE
*返 回 值：
*功能说明：
*********************************************************************/
unsigned char ble_slave_init(void)
{
    hy40r204_wake_con(HY40R204_WAKE_OK);
    delaynms(2);
    hy40r204_send_data( "AT\r\n", strlen("AT\r\n") );
    delaynms(200);
    hy40r204_send_data( "AT+SET=1\r\n", strlen("AT+SET=1\r\n") );
    delaynms(200);
    //ble_data_mode();
    hy40r204_send_data( "AT+MODE=DATA\r\n", strlen("AT+MODE=DATA\r\n") );
    delaynms(10);
    hy40r204_wake_con(HY40R204_WAKE_NOT_OK);
    return 0;
}

/*********************************************************************
*函 数 名：ble_data_send
*参    数：buf->发送数据缓存，len->数据长度
*返 回 值：
*功能说明：
*********************************************************************/
unsigned char ble_data_send(unsigned char *buf,unsigned char len)
{   
    hy40r204_wake_con(HY40R204_WAKE_OK);
    delaynms(2);
    hy40r204_send_data(buf, len);
    hy40r204_wake_con(HY40R204_WAKE_NOT_OK);
    return 0;
}
/*********************************************************************
*函 数 名：ble_data_receive
*参    数：buf->数据缓存，
*参    数：len->接收数据长度容量
*返 回 值：实际接收数据长度
*功能说明：
*********************************************************************/
unsigned char ble_data_receive(unsigned char *buf, uint16_t len)
{   
    unsigned char tempLen = 0;
    if(hy40r204RecData.len != 0)
    {
        tempLen = len;
        if(hy40r204RecData.len <= tempLen)
            tempLen = hy40r204RecData.len;
        memcpy(buf, hy40r204RecData.pBuf, tempLen);
    }
    return tempLen;
}
/*********************************************************************
*函 数 名：ble_slave_mac_show
*参    数：
*返 回 值：
*功能说明：
*********************************************************************/
unsigned char ble_slave_mac_show(void)
{
    hy40r204_wake_con(HY40R204_WAKE_OK);
    delaynms(1);
    hy40r204_send_data("AT+SET=1\r\n",strlen("AT+SET=1\r\n"));/*UART接口控制下需设置为从机，才可以获取本机地址*/
    delaynms(200);
    hy40r204_send_data("AT+DEVID=?\r\n",strlen("AT+DEVID=?\r\n"));/*获取本机地址*/
    delaynms(200);
    hy40r204_send_data("AT+SOFT_RST=1\r\n",strlen("AT+SOFT_RST=1\r\n"));
    delaynms(200);
    hy40r204_wake_con(HY40R204_WAKE_NOT_OK);
    return 0;
}
/*********************************************************************
*函 数 名：ble_connection_check
*参    数：host_slave_flag主从机标志 1:主机  2：从机，0：无效
*返 回 值：1->已连接，0->未连接
*功能说明：在检测蓝牙主、从机 是否已经连接配对上
*********************************************************************/
unsigned char ble_connection_check(unsigned char host_slave_flag)
{   
    return 0;    
}
/*********************************************************************
*函 数 名：str_to_hex
*参    数：buf->数据缓存，str->待转换的字符串
*返 回 值：NONE
*功能说明：例如：输入str:"84DD20010919",输出buf:{0x84,0xDD,0x20,0x01,0x09,0x19}
*********************************************************************/
void str_to_hex(unsigned char *str,unsigned char *buf)
{
    unsigned char i,j;
    unsigned char temp[12];

    for(i = 0,j = 0;i < 12;i++)
    {
        if((str[i]>=0x30)&&(str[i]<=0x39))
        {  
            temp[i] = str[i]-0x30;
        }
        else
        {
            if(str[i]==0x41)
                temp[i] = 0x0A;
            if(str[i]==0x42)
                temp[i] = 0x0B;
            if(str[i]==0x43)
                temp[i] = 0x0C;
            if(str[i]==0x44)
                temp[i] = 0x0D;
            if(str[i]==0x45)
                temp[i] = 0x0E;
            if(str[i]==0x46)
                temp[i] = 0x0F;
        }
    }
    for(i = 0,j = 0;i < 12;)
    {
        buf[j] = (temp[i]&0x0F)<<4;//赋值，不能用或运算
        i++;//分开写
        buf[j] |= (temp[i]&0x0F);
        i++;//分开写
        j++;//分开写
    }
}

/*********************************************************************
 * @fn      bdAddr2Str
 *
 * @brief   Convert Bluetooth address to string. Only needed when
 *          LCD display is used.
 *
 * @return  none
 ********************************************************************/
unsigned char ble_addr_to_str( unsigned char *pAddr, unsigned char pAddrString[B_ADDR_STR_LEN] )
{
  unsigned char       i;
  unsigned char       hex[] = "0123456789ABCDEF";
//  unsigned char       str[B_ADDR_STR_LEN];
  unsigned char       *pStr = pAddrString;

  *pStr++ = '0';
  *pStr++ = 'x';

  // Start from end of addr
  pAddr += B_ADDR_LEN;

  for ( i = B_ADDR_LEN; i > 0; i-- )
  {
    *pStr++ = hex[*--pAddr >> 4];
    *pStr++ = hex[*pAddr & 0x0F];
  }

  *pStr = 0;

  return 1;
}



