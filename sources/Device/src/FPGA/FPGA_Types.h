#pragma once


#define ADDR_ALTERA             ((uint8*)NOR_MEMORY_ADRESS1)


namespace FPGA
{
    namespace ADDR
    {
        namespace WR
        {
            extern uint8 *START;        ///< �����
            extern uint8 *TBASE;        ///< ��������
            extern uint8 *PRED_LO;      ///< ����������
            extern uint8 *PRED_HI;
            extern uint8 *POST_LO;      ///< �����������
            extern uint8 *POST_HI;

            extern uint8 *UPR;          ///< ����������
#define BIT_UPR_RAND            0       ///< ���. ������������
#define BIT_UPR_PEAK            1       ///< ������� ��������
#define BIT_UPR_CALIBR_AC_DC    2       ///< ���������� AC/DC
#define BIT_UPR_CALIBR_ZERO     3       ///< ���������� 0/4�

            extern uint8 *FREQMETER;    ///< ���������� ������������
            extern uint8 *TRIG;         ///< ���������� ��������������/��������
#define BIT_TRIG_POLARITY       0       ///< ���������� ����������� �������������
#define BIT_TRIG_ENABLED        1       ///< ��� ��������� ������������. 0 - ���, 1 - ����

            extern uint8 *START_ADDR;               ///< \brief ����� ������. ����� ����������� �� ����� ������, ����� ������ ������ � 
                                                    ///  ������������� ������ (������� �� ���������� � ����������)
            extern uint8 *RESET_COUNTER_FREQ;       ///< ���������� ���� ����� ��������� ��������� ����������� ��� ��������� �������� �������
            extern uint8 *RESET_COUNTER_PERIOD;     ///< ���������� ���� ����� ��������� ��������� ����������� ��� ��������� �������� �������
            
            extern uint8 *TRIG_HOLD_ENABLE;         ///< ���������� ��������� �������������. "0" - ����, "1" - ���
            extern uint8 *TRIG_HOLD_VALUE_LOW;      ///< ������� ���� �������� �������������
            extern uint8 *TRIG_HOLD_VALUE_MID;      ///<    �������� �������������� �� ������� N = 0 - (�������� / 100��). ������������ �������� - 1.5 ���
            extern uint8 *TRIG_HOLD_VALUE_HI;       ///< ������� ���� �������� �������������
        }

        namespace RD
        {
            extern uint8 *DATA_A;               ///< ������ ������� ������
            extern uint8 *DATA_A_PEAK_MAX;      ///< ������ �������� ��������� ������� ������
            extern uint8 *DATA_A_PEAK_MIN;      
            extern uint8 *DATA_B;               ///< ������ ������� ������
            extern uint8 *DATA_B_PEAK_MAX;      ///< ������ ������� ��������� ������� ������
            extern uint8 *DATA_B_PEAK_MIN;      
            extern uint8 *LAST_RECORD_LO;       ///< ����� ��������� ������
            extern uint8 *LAST_RECORD_HI;       
            extern uint8 *FREQ_BYTE_0;          ///< ���������� �������� �������
            extern uint8 *FREQ_BYTE_1;          
            extern uint8 *FREQ_BYTE_2;          
            extern uint8 *FREQ_BYTE_3;          
            extern uint8 *PERIOD_BYTE_0;        ///< ���������� �������� �������
            extern uint8 *PERIOD_BYTE_1;        
            extern uint8 *PERIOD_BYTE_2;        
            extern uint8 *PERIOD_BYTE_3;        
            extern uint8 *FLAG_LO;              ///< ������� ���� �����
            extern uint8 *FLAG_HI;              ///< ������� ���� �����
        }
    }
}

#define AVE_VALUE               127
#define MIN_VALUE               (AVE_VALUE - 125)
#define MAX_VALUE               (AVE_VALUE + 125)

/// ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������
#define NONE_VALUE  0
