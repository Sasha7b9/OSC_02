#include "defines.h"
#include "AT25160N.h"
#include <cstdlib>
#include "Hardware/HAL/HAL_PIO.h"


using HAL::PIO::Mode;
using HAL::PIO::Pull;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PIN_OUT     HAL::PIO::Port::_C, HAL::PIO::Pin::_3
#define PIN_IN      HAL::PIO::Port::_C, HAL::PIO::Pin::_2
#define PIN_CLK     HAL::PIO::Port::_B, HAL::PIO::Pin::_10
#define PIN_CS      HAL::PIO::Port::_B, HAL::PIO::Pin::_12

#define WREN    BIN_U8(00000110)        ///< Set Write Enable Latch
#define WRDI    BIN_U8(00000100)        ///< Reset Write Enable Latch
#define RDSR    BIN_U8(00000101)        ///< Read Status Register
#define READ    BIN_U8(00000011)        ///< Read Data from Memory Array
#define WRITE   BIN_U8(00000010)        ///< Write Data to Memory Array
#define WRSR    BIN_U8(00000001)        ///< Write Status Register


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��������� ������
static void SetWriteLatch();
/// ��������� ������
static void ResetWriteLatch();
/// ������ ������� �������
static uint8 ReadStatusRegister();
/// �������� ������� �������
static void WriteStatusRegister(uint8 data);
/// ���������� size ����, ������� � ������ address
static void WriteData(uint address, uint8 *data, uint size);
/// �������� ������ ������ �� ������� ������. ������ �� ����� ���� ������ 32 ����
static void Write32BytesOrLess(uint address, const uint8 *data, uint size);
/// ������ size ����, ������� � ������ address
static void ReadData(uint address, uint8 *data, uint size);

/// ���������� ���� � ����������
static void WriteByte(uint8 byte);
/// ������ ���� �� ����������
static uint8 ReadByte();

/// �������, ���� �� ���������� ��������� ���� ������
static void WaitFinishWrite();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AT25160N::Init()
{
    //__HAL_RCC_GPIOB_CLK_ENABLE();
    //__HAL_RCC_GPIOC_CLK_ENABLE();

    /*  ���������� �������
    SPI2
    PB12 - NSS
    PB10 - SCK
    PC3  - MOSI
    PC2 -  MISO   */

    //__HAL_RCC_SPI2_CLK_ENABLE();

    //                                  SCK                    NSS
    uint pins = (uint)(HAL::PIO::Pin::_10 | HAL::PIO::Pin::_12);
    HAL::PIO::Init(HAL::PIO::Port::_B, pins, Mode::Output_PP, Pull::Down);

    //                                                      MOSI
    HAL::PIO::Init(HAL::PIO::Port::_C, HAL::PIO::Pin::_3, Mode::Output_PP, Pull::Down);

    //                                                      MISO
    HAL::PIO::Init(HAL::PIO::Port::_C, HAL::PIO::Pin::_2, Mode::Input, Pull::Down);

    HAL::PIO::Set(PIN_CS);
    HAL::PIO::Reset(PIN_OUT);
    HAL::PIO::Reset(PIN_CLK);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::Test()
{
    const uint size = 1000;
    uint8 data[size];
    uint8 out[size];
    for(uint i = 0; i < size; i++)
    {
        data[i] = (uint8)std::rand();
    }

//    uint timeStart = TIME_MS;

    SetWriteLatch();

//    uint time1 = TIME_MS;

    WriteData(0, data, size);

//    uint time2 = TIME_MS;

    //WaitFinishWrite();

    ReadData(0, out, size);
    ResetWriteLatch();

//    uint time3 = TIME_MS;

    //LOG_WRITE("1 = %d, 2 = %d, 3 = %d, %d", time1 - timeStart, time2 - time1, time3 - time2, time3 - timeStart);

    bool testIsOk = true;

    for(uint i = 0; i < size; i++)
    {
        if(data[i] != out[i])
        {
            testIsOk = false;
            LOG_WRITE("������ �� %d-� ��������", i);
            break;
        }
    }

    WriteStatusRegister(0);

    if(testIsOk)
    {
        LOG_WRITE("Test is OK!!!");
    }
    else
    {
//        LOG_WRITE("WARNING!!! Test is failed!!!");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteData(uint address, uint8 *data, uint size)
{
    while(1)
    {
        if(size <= 32)
        {
            Write32BytesOrLess(address, data, size);
            break;
        }
        Write32BytesOrLess(address, data, 32);
        address += 32;
        data += 32;
        size -= 32;    
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Write32BytesOrLess(uint address, const uint8 * /*data*/, uint size)
{
    WaitFinishWrite();

    SetWriteLatch();

    HAL::PIO::Reset(PIN_CS);

    WriteByte(WRITE); //-V2501

    WriteByte((address >> 8) & 0xff);

    WriteByte(address & 0xff);

    for (uint i = 0; i < size; i++)
    {
        //uint8 byte = data[i];

        for (int bit = 7; bit >= 0; bit--)
        {
            //if (_GET_BIT(byte, bit))
            //{
            //    GPIOC->BSRR = GPIO_PIN_3;
            //}
            //GPIOB->BSRR = GPIO_PIN_10;
            //GPIOC->BSRR = GPIO_PIN_3 << 16U;
            //GPIOB->BSRR = GPIO_PIN_10 << 16U;
        }
    }

    HAL::PIO::Set(PIN_CS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetWriteLatch()
{
    HAL::PIO::Reset(PIN_CS);
    WriteByte(WREN); //-V2501
    HAL::PIO::Set(PIN_CS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ResetWriteLatch()
{
    WaitFinishWrite();

    HAL::PIO::Reset(PIN_CS);
    WriteByte(WRDI); //-V2501
    HAL::PIO::Set(PIN_CS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 ReadStatusRegister()
{
    HAL::PIO::Reset(PIN_CS);
    WriteByte(RDSR); //-V2501
    uint8 result = ReadByte();
    HAL::PIO::Set(PIN_CS);
    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteStatusRegister(uint8 data)
{
    WaitFinishWrite();

    HAL::PIO::Reset(PIN_CS);
    WriteByte(WRSR); //-V2501
    WriteByte(data);
    HAL::PIO::Set(PIN_CS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WaitFinishWrite()
{
    while (_GET_BIT(ReadStatusRegister(), 0))
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::Save(Settings &)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::Load(Settings &)
{
    /*
    �������� �������� ��������
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteByte(uint8 byte)
{
    for(int bit = 7; bit >= 0; bit--)
    {
        if (_GET_BIT(byte, bit))
        {
            HAL::PIO::Set(PIN_OUT);
        }
        HAL::PIO::Set(PIN_CLK);
        HAL::PIO::Reset(PIN_CLK);
        HAL::PIO::Reset(PIN_OUT);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 ReadByte()
{
    uint8 retValue = 0;

    for(int i = 0; i < 8; i++)
    {
        HAL::PIO::Set(PIN_CLK);
        retValue <<= 1;
        if(HAL::PIO::Read(PIN_IN))
        {
            retValue |= 0x01;
        }
        HAL::PIO::Reset(PIN_CLK);
    }

    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadData(uint address, uint8 *data, uint size)
{
    WaitFinishWrite();

    HAL::PIO::Reset(PIN_CS);

    WriteByte(READ); //-V2501
    WriteByte((address >> 8) & 0xff);
    WriteByte(address & 0xff);

    for(uint i = 0; i < size; i++) //-V756
    {
        data[i] = 0;

        for (int j = 0; j < 8; j++)
        {
            //GPIOB->BSRR = GPIO_PIN_10;
            //
            //data[i] <<= 1;
            //if (HAL::PIO::Read(PIN_IN))
            //{
            //    data[i] |= 0x01;
            //}
            //
            //GPIOB->BSRR = GPIO_PIN_10 << 16U;
        }
    }

    HAL::PIO::Set(PIN_CS);
}
