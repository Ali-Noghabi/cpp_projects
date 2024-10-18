#include "udp.h"

Udp *Udp::mInstance = nullptr;

Udp::Udp()
{
    mUdpSocket = new QUdpSocket();
    dataArray.resize(sizeof(RawHeaderPacket) + mDataLength * sizeof(uint16_t) + sizeof(RawHeaderPacket::header));
    dataScalar.resize(sizeof(RawHeaderPacket) + sizeof(uint16_t) + sizeof(RawHeaderPacket::header));
}

int Udp::dataLength() const
{
    return mDataLength;
}

void Udp::setDataLength(int newDataLength)
{
    dataArray.resize(sizeof(RawHeaderPacket) + mDataLength * sizeof(uint16_t) + sizeof(RawHeaderPacket::header));
    mDataLength = newDataLength;
}
