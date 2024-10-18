#include "datagenerator.h"

#include <QDateTime>

#include <QElapsedTimer>
#include <QRandomGenerator>
#include <random>
#include <iostream>

#include "udp.h"

DataGenerator::DataGenerator(QObject *parent):
    QObject{parent}
{
    dataArray.resize(sizeof(RawHeaderPacket) + mDataLength * sizeof(uint16_t) + sizeof(RawHeaderPacket::header));
    dataScalar.resize(sizeof(RawHeaderPacket) + sizeof(uint16_t) + sizeof(RawHeaderPacket::header));

    std::cout << "SIZEOF : RawArrayPacket : " << dataArray.size() << std::endl;
    std::cout << "SIZEOF : RawScalarPacket : " << dataScalar.size() << std::endl;

    mData = new QList<uint16_t>;
    mData->reserve(mDataLength);
}

void  DataGenerator::setHardwareId(int id, bool isArray)
{
    RawHeaderPacket *h;

// RawArrayPacket  *a = nullptr;
// RawScalarPacket *b = nullptr;

    if (isArray)
    {
// a = new RawArrayPacket;
// h = a;

        h = new RawHeaderPacket;

        h->length = mDataLength;
// a->footer = FOOTER;
    }
    else
    {
// b = new RawScalarPacket;
// h = b;

        h = new RawHeaderPacket;

        h->length = 1;
// b->footer = FOOTER;
    }

    h->header    = HEADER;
    h->crc16     = 0;
    h->counter   = 0;
    h->timestamp = QDateTime::currentMSecsSinceEpoch();
    h->uid       = id;
    h->type      = 1;
    h->status    = mStatus;
    h->interval  = 1;

    if (mHardware)
    {
        delete mHardware;
        mHardware = nullptr;
    }

    mHardware = h;
}

void  DataGenerator::setBoardStatus(uint16_t boardStatus)
{
    mHardware->status = mStatus = boardStatus;
}

void  DataGenerator::setStartStop(bool s)
{
    if (s)
    {
        mCounter = 0;
        mTimer->start();
        mTimerStatus->start();
    }
    else
    {
        mTimer->stop();
        mTimerStatus->stop();
    }
}

void  DataGenerator::setInterval(int ms)
{
    mTimer->setInterval(ms);
}

void  DataGenerator::generateData()
{
    QMutexLocker  locker(&mMutex);
// QElapsedTimer  t;

// t.start();
    auto            rand = QRandomGenerator::global();
    QList<QPointF>  points;

    if (!mHardware)
    {
        return;
    }

// for (auto h : mHardware)
    {
        mData->clear();
        mCounter++;

        if (mCounter > UINT32_MAX)
        {
            mCounter = 0;
        }

        mHardware->status = mStatus;

        if (mHardware->length == 1)
        {
            mData->resize(mHardware->length);
            mData->replace(0, rand->bounded(0, 255));

            *mData->data() = rand->bounded(mScalarData - mRandomInterval, mScalarData + mRandomInterval);

            mMsgSize += Udp::Instance()->write(mData, mHardware, mHost, mPort);
            mMsgCounter++;
        }
        else
        {
            mHardware->length = mDataLength;

            mData->resize(mHardware->length);

            for (int i = 0; i < mHardware->length; i++)
            {
                if (mRandomInterval)
                {
                    mData->replace(i, rand->bounded(mStaticPoints[i] - mRandomInterval, mStaticPoints[i] + mRandomInterval));
                }
                else
                {
                    mData->replace(i, mStaticPoints[i]);
                }

                points.append(QPointF(i, mData->at(i)));
            }

            mMsgSize += Udp::Instance()->write(mData, mHardware, mHost, mPort);
            mMsgCounter++;
        }

// }
    }

    emit  dataReady(points);

// std::cout << "TIMER : " << t.nsecsElapsed() << " : " << t.elapsed() << std::endl;
}

void  DataGenerator::printStatus()
{
    std::cout << "STATUS : " << /*mCounter*/ Udp::Instance()->counter() << " : " << mMsgCounter << " : " << mMsgSize / 1024 << " (KB) " << std::endl;

    mMsgCounter = 0;
    mMsgSize    = 0;
}

void  DataGenerator::movePointTowards(int x1, int y1, int x2, int y2, int d, int &x, int &y)
{
    int     pX     = x2 - x1;
    int     pY     = y2 - y1;
    double  length = sqrt(pX * pX + pY * pY);
    double  pFx    = pX / length;
    double  pFy    = pY / length;

    x = x1 + pFx * d;
    y = y1 + pFy * d;
}

float  DataGenerator::length(int x1, int y1, int x2, int y2)
{
    int  pX = x2 - x1;
    int  pY = y2 - y1;

    return sqrt(pX * pX + pY * pY);
}

int  DataGenerator::dataLength() const
{
    return mDataLength;
}

void  DataGenerator::setDataLength(int newDataLength)
{
    dataArray.resize(sizeof(RawHeaderPacket) + mDataLength * sizeof(uint16_t) + sizeof(RawHeaderPacket::header));
    mData->reserve(mDataLength);

    mDataLength = newDataLength;
}

int  DataGenerator::randomInterval() const
{
    return mRandomInterval;
}

void  DataGenerator::setRandomInterval(int newRandomInterval)
{
    mRandomInterval = newRandomInterval;
}

void  DataGenerator::setScalarData(quint16 newData)
{
    mScalarData = newData;
}

void  DataGenerator::setPoints(const QList<QPointF> &newPoints)
{
    QMutexLocker  locker(&mMutex);
    int           x   = 0;
    int           y   = 0;
    auto          itr = newPoints.begin();
    int           Y   = itr->y();
    int           X1  = itr->x();
    float         D   = 0;
    float         LD  = 0;

    itr++;

    int  X2    = itr->x();
    int  nextY = itr->y();

    mStaticPoints.clear();
    LD = length(X1, Y, X2, nextY) / 100.0;

    mStaticPoints.append(Y);

    for (int i = 1; i < mDataLength; i++)
    {
        movePointTowards(X1, Y, X2, nextY, D, x, y);
        mStaticPoints.append(y);
        D += LD;

        if (i % 100 == 0)
        {
            Y  = nextY;
            X1 = itr->x();
            itr++;

            if (itr != newPoints.end())
            {
                X2    = itr->x();
                nextY = itr->y();
            }

            LD = length(X1, Y, X2, nextY) / 100.0;
            D  = 0;
        }
    }
}

int  DataGenerator::port() const
{
    return mPort;
}

void  DataGenerator::setPort(int newPort)
{
    mPort = newPort;
}

void  DataGenerator::initialize()
{
    mUdpSocket = new QUdpSocket();
    mTimer     = new QTimer();
    mTimer->setInterval(1000);
    connect(mTimer, &QTimer::timeout, this, &DataGenerator::generateData, Qt::QueuedConnection);

    mTimerStatus = new QTimer();
    mTimerStatus->setInterval(1000);
    connect(mTimerStatus, &QTimer::timeout, this, &DataGenerator::printStatus, Qt::QueuedConnection);
}

const QHostAddress& DataGenerator::host() const
{
    return mHost;
}

void  DataGenerator::setHost(const QHostAddress &newHost)
{
    mHost = newHost;
}
