#ifndef UDP_H
#define UDP_H

#include <QMutex>
#include <QUdpSocket>
#include <QList>
#include <QDateTime>

#include "datagenerator.h"


class Udp
{
public:
    static Udp* Instance()
    {
        if (!mInstance)
        {
            mInstance = new Udp();
        }

        return mInstance;
    }

    quint64  counter()
    {
        return mCounter;
    }

    quint64  counterWithInc()
    {
        return mCounter++;
    }

    qint64  write(QList<uint16_t> *data, RawHeaderPacket *header, QHostAddress host, int port)
    {
        QMutexLocker  m(&mMutex);

        header->counter   = Udp::counterWithInc();
        header->timestamp = QDateTime::currentMSecsSinceEpoch();

        qint64  ret;

        if (header->length == 1)
        {
            memcpy(dataScalar.data(), (char *)header, sizeof(RawHeaderPacket));

            memcpy(dataScalar.data() + sizeof(RawHeaderPacket), data->data(), sizeof(uint16_t));

            auto  cs = qChecksum(dataScalar.data() + sizeof(header->header) + sizeof(header->crc16),
                                 dataScalar.size() - sizeof(header->header) - sizeof(header->crc16) - sizeof(header->header));

            header->crc16 = cs;

            memcpy(dataScalar.data(), (char *)header, sizeof(RawHeaderPacket));

            uint32_t  f = FOOTER;
            char     *p = (char *)&f;
            memcpy(dataScalar.data() + sizeof(RawHeaderPacket) + data->size() * sizeof(uint16_t), p, sizeof(FOOTER));

            ret = mUdpSocket->writeDatagram(dataScalar, host, port);
        }
        else
        {
            memcpy(dataArray.data(), (char *)header, sizeof(RawHeaderPacket));

            memcpy(dataArray.data() + sizeof(RawHeaderPacket), data->data(), header->length * sizeof(uint16_t));

            auto  cs = qChecksum(dataArray.data() + sizeof(header->header) + sizeof(header->crc16),
                                 dataArray.size() - sizeof(header->header) - sizeof(header->crc16) - sizeof(header->header));

            header->crc16 = cs;

            memcpy(dataArray.data(), (char *)header, sizeof(RawHeaderPacket));

            uint32_t  f = FOOTER;
            char     *p = (char *)&f;
            memcpy(dataArray.data() + sizeof(RawHeaderPacket) + data->size() * sizeof(uint16_t), p, sizeof(FOOTER));

            ret = mUdpSocket->writeDatagram(dataArray, host, port);
        }

        return ret;
    }

    int dataLength() const;
    void setDataLength(int newDataLength);

private:
    Udp();

private:
    static Udp *mInstance;
    quint64     mCounter   = 0;
    QUdpSocket *mUdpSocket = nullptr;
    QByteArray  dataArray;
    QByteArray  dataScalar;
    QMutex      mMutex;
    int mDataLength = 2400;

};

#endif // UDP_H
