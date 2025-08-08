#include "Int_W5500.h"

// ����� socket ��� (��ѡ sn=1~7)
#define SOCKET_SN 0
// ����� ���ض˿ں�
#define LOCAL_PORT 5555

// ����� �������˿ں�
#define SERVER_PORT 8000

// ���� ������ IP ��ַ
uint8_t server_ip[4] = {192,168,11,36};

// ���屾�����ò���
Com_LocalConfig_t LocalConfig = {
    .ip = {192,168,11,100},                       //IP��ַ
    .mac = {0x7C,0x88,0xDC,0x12,0x2A,0x56},    //MAC��ַ
    .snm = {255,255,255,0},                      //��������
    .gw = {192,168,11,1}                          //���ص�ַ
};

void Int_W5500_Init(void)
{
    // 1 ��λ
    // 1.1 ���͸�λ���� W5500_RST
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
    Com_Delay_ms(10);       //������Ҫ���� 500 us
    // 1.3 ���߸�λ���� W5500_RST
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
    Com_Delay_ms(10);       //�����ȶ�

    // 2 ע�ắ��
    development_reg_func();

    // 3 ����IP��ַ.MAC��ַ.��������.���ص�ַ
    setSIPR(LocalConfig.ip);    //����IP��ַ
    setSHAR(LocalConfig.mac);   //����MAC��ַ
    setSUBR(LocalConfig.snm);   //������������
    setGAR(LocalConfig.gw);     //�������ص�ַ

    DEBUG_PRINTLN("W5500 ��ʼ�����......");
}

uint8_t Int_W5500_SendData(uint8_t *data, uint16_t len)
{
    // 0 ������� ���� socket ��״̬
    uint8_t status = 0;

    // 1 ��鵱ǰ������״̬ ,ֱ��Ϊ ESTABLISHED ״̬
    do
    {
        // 1.1 ��ȡ socket ״̬ (�ر�״̬;����YCPģʽ;�Ͽ�����)
        status = getSn_SR(SOCKET_SN);
        
        // 1.2 ��� socket ״̬�Ƿ�Ϊ�ر�״̬ ,���� socket �ͻ���
        if (status == SOCK_CLOSED)
        {
            // 1.2.1 ���� socket �ͻ���
            uint8_t socket_status = socket(SOCKET_SN, Sn_MR_TCP, LOCAL_PORT, 0);
            if (socket_status == SOCKET_SN)
            {
                DEBUG_PRINTLN("socket �ͻ��˴����ɹ�......");
            }
            else
            {
                DEBUG_PRINTLN("socket �ͻ��˴���ʧ��......");
            }
        }

        // 1.3 ��� socket ״̬�Ƿ�Ϊ INIT״̬ ,����� ,�����ӷ�����
        if (status == SOCK_INIT)
        {
            // 1.3.1 ���ӷ�����
            uint8_t connect_status = connect(SOCKET_SN, server_ip, SERVER_PORT);

            if (connect_status == SOCK_OK)
            {
                DEBUG_PRINTLN("socket �ͻ������ӷ������ɹ�......");
            }
            else
            {
                DEBUG_PRINTLN("socket �ͻ������ӷ�����ʧ��......");

            }
        }

        // 1.4 ��� socket ״̬�Ƿ�Ϊ CLOSE_WAIT ״̬,����� ,��ر����ӷ�����
        if (status == SOCK_CLOSE_WAIT)
        {
            // 1.4.1 �ر����ӷ�����
            uint8_t close_status = close(SOCKET_SN);
            if (close_status == SOCK_OK)
            {
                DEBUG_PRINTLN("socket �ͻ��˹ر����ӳɹ�......");
            }
            else
            {
                DEBUG_PRINTLN("socket �ͻ��˹ر�����ʧ��......");
            }
        }
    } while (status != SOCK_ESTABLISHED);
    
    // 2 ��������
    uint16_t send_status = send(SOCKET_SN, data, len);
    if (send_status == len)
    {
        DEBUG_PRINTLN("socket �ͻ��˷������ݳɹ�......");
        return 0;

    }
    else
    {
        DEBUG_PRINTLN("socket �ͻ��˷�������ʧ��......");
        return 1;
    }
}
