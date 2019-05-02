#include "defines.h"
#include "Communication.h"
#include "Communicator.h"


namespace Communicator
{
    void InitSendPin();
    void InitPins();
    void Set_REQ_SEND();
    void Reset_REQ_SEND();
    int Read_ALLOW_SEND();
    int Read_CONF_DATA();
    void Set_CLK();
    void Reset_CLK();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Communicator::Init()
{
    Transceiver::SetCallbacks(
        InitSendPin,
        InitPins,
        Set_REQ_SEND,
        Reset_REQ_SEND,
        Read_ALLOW_SEND,
        Read_CONF_DATA,
        Set_CLK,
        Reset_CLK
        );
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::InitSendPin()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::InitPins()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::Set_REQ_SEND()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::Reset_REQ_SEND()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Communicator::Read_ALLOW_SEND()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Communicator::Read_CONF_DATA()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::Set_CLK()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::Reset_CLK()
{

}
