#ifndef __INT_W5500_H__
#define __INT_W5500_H__

#include "wizchip_conf.h"
#include "socket.h"
#include "Com_Debug.h"
#include "Com_Util.h"
#include "Com_Config.h"

/**
 * @brief W5500��̫��ģ�� ��ʼ��

 * 
 */
void Int_W5500_Init(void);

/**
 * @brief W5500��̫��ģ�� �������ݵ�������
 * 
 * @param data Ҫ���͵�����
 * @param len ���ݳ���
 * @return uint8_t ����״̬ (0:�ɹ�;1:ʧ��)

 */
uint8_t Int_W5500_SendData(uint8_t *data, uint16_t len);

#endif /* __INT_W5500_H__ */
