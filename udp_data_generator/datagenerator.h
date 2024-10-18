#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QMutex>
#include <QObject>
#include <QPointF>
#include <QTimer>
#include <QUdpSocket>


#define HEADER                  0x00AADBAF
#define FOOTER                  0x00DFAAFF

//#define DATA_LENGTH             3600
#define RESERVED_LENGTH         18
#define CRC_LENGTH              4

struct RawHeaderPacket
{
    uint32_t  header;
    uint16_t  crc16;
    uint32_t  counter;
    uint64_t  timestamp;
    uint64_t  uid;
    uint16_t  type;
    uint16_t  status;
    uint32_t  interval;
    uint16_t  length;
} __attribute__((packed()));

// struct RawArrayPacket: RawHeaderPacket
// {
// uint16_t  data[DATA_LENGTH];
// uint16_t  crc16;
// uint32_t  footer;
// } __attribute__((packed()));

// struct RawScalarPacket: RawHeaderPacket
// {
// uint16_t  data;
// uint16_t  crc16;
// uint32_t  footer;
// } __attribute__((packed()));
struct Hardware
{
    RawHeaderPacket  header;
    bool             isArray = false;
    QList<float>     data;
};

class DataGenerator: public QObject
{
    Q_OBJECT

public:
    explicit DataGenerator(QObject *parent = nullptr);

    void                 setHardwareId(int id, bool isArray);

    void                 setBoardStatus(uint16_t boardStatus);

    const QHostAddress & host() const;

    void                 setHost(const QHostAddress &newHost);

    int                  port() const;

    void                 setPort(int newPort);

    const QList<int>   & points() const;

    void                 setPoints(const QList<QPointF> &newPoints);

    int                  randomInterval() const;

    void                 setRandomInterval(int newRandomInterval);

    void                 setScalarData(quint16 newData);

    int dataLength() const;
    void setDataLength(int newDataLength);

public slots:
    void                 initialize();

    void                 setStartStop(bool s);

    void                 setInterval(int ms);

signals:
    void                 dataReady(QList<QPointF>);

private slots:
    void                 generateData();

    void                 printStatus();

private:
    void                 movePointTowards(int x1, int y1, int x2, int y2, int d, int &x, int &y);

    float                length(int x1, int y1, int x2, int y2);

private:
    QHostAddress     mHost;
    int              mPort;
    QUdpSocket      *mUdpSocket   = nullptr;
    QTimer          *mTimer       = nullptr;
    QTimer          *mTimerStatus = nullptr;
    RawHeaderPacket *mHardware    = nullptr;
    QList<uint16_t> *mData        = nullptr;
    uint32_t         mCounter     = 0;
    uint32_t         mMsgCounter  = 0;
    uint32_t         mMsgSize     = 0;
    QByteArray       dataArray;
    QByteArray       dataScalar;
    QList<int>       mPoints;
    QList<int>       mStaticPoints;
    int              mRandomInterval = 5;
    uint16_t         mStatus         = 1;
    quint16          mScalarData     = 0;
    QMutex           mMutex;
    int mDataLength = 2400;
};

#endif // DATAGENERATOR_H
