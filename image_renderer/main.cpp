#include <QtGui>
#include "RenderOffScreen.h"
#include "renderegl2.h"


#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QElapsedTimer>

int  main(int argc, char *argv[])
{
    QGuiApplication  app(argc, argv);

    // TriangleRenderer renderer;
    // renderer.renderTriangle(QSize(512, 512));
    RenderEGL2  bb;
    QFile       fp("data.csv");

    if (!fp.open(QIODevice::ReadOnly))
    {
        std::cout << "Failed to open file data.csv" << std::endl;

        return 0;
    }

    QList<QJsonArray>  m_data;
    QList<QJsonArray>  m_value;

    // Actual Data
    QJsonArray  tmpData;
    QJsonArray  colorTmpData;
    int         lastRow = 0;

    while (!fp.atEnd())
    {
        auto    rawData = fp.readLine();
        auto    colData = rawData.split(',');
        int     row     = colData[0].toInt();
        int     col     = colData[1].toInt();
        double  y       = colData[2].toDouble();
        double  c       = colData[3].toDouble();

        if (lastRow != row)
        {
            m_data.push_back(tmpData);
            m_value.push_back(colorTmpData);

            tmpData      = QJsonArray();
            colorTmpData = QJsonArray();
            lastRow      = row;
        }

        tmpData.append(y);
        colorTmpData.append(c);
    }

    // m_data.clear();
    // m_value.clear();
    //! Test Data
    // m_data.append(QJsonArray({ -1000.0, -1200.0, -1000, -1100 }));
    // m_data.append(QJsonArray({ -2000.0, -1800.0, -2200, -1800 }));
    // m_data.append(QJsonArray({ -3000.0, -2800.0, -3200, -2800 }));
    // m_data.append(QJsonArray({ -1000.0, -1200.0, -1000 }));
    // m_data.append(QJsonArray({ -2000.0, -1800.0, -2200 }));
    // m_data.append(QJsonArray({ -3000.0, -2800.0, -3200 }));

    // Y.append( QJsonArray({0.5,1.0}));
    // m_value.append(QJsonArray({ "#FF0000", "#FF0000", "#00FF00", "#0000FF" }));
    // m_value.append(QJsonArray({ "#0000FF", "#0000FF", "#FFFF00", "#0000FF" }));
    // m_value.append(QJsonArray({ "#00FF00", "#00FF00", "#FF0000", "#0000FF" }));

    // m_value.append(QJsonArray({ "#FF0000", "#FF0000", "#00FF00" }));
    // m_value.append(QJsonArray({ "#0000FF", "#0000FF", "#FFFF00" }));
    // m_value.append(QJsonArray({ "#00FF00", "#00FF00", "#FF0000" }));

    // m_value.append(QJsonArray({ "#0000FF", "#0000FF" }));
    // m_value.append(QJsonArray({ "#FFFF00", "#FFFF00" }));
    // m_value.append(QJsonArray({ "#FF0000", "#FF0000" }));

    // m_value.append( QJsonArray({"#000000","#000000"}));

    // QList<QJsonArray> Y;
    // QList<QJsonArray> color;

    // Y.append( QJsonArray({0,0.5}));
    // Y.append( QJsonArray({0,0.8}));
    // Y.append( QJsonArray({0,1.1}));
    // color.append( QJsonArray({"#FF0000","#FF0000"}));
    // color.append( QJsonArray({"#00FF00","#00FF00"}));
    // color.append( QJsonArray({"#0000FF","#0000FF"}));

    QElapsedTimer  t;

    t.start();

    RenderOffScreen  ros;

    ros.setData(m_data, m_value);

    auto  img2 = ros.render(QSize(8000, 6000));

    for (int y = 0; y < img2.height(); y++)
    {
        for (int x = 0; x < img2.width(); x++)
        {
            QRgb  pixel = img2.pixel(x, y);

            if ((qRed(pixel) == 0) && (qGreen(pixel) == 0) && (qBlue(pixel) == 0))
            {
                img2.setPixel(x, y, qRgba(0, 0, 0, 0)); // Black pixels are set to transparent (alpha = 0)
            }
        }
    }

    if (!img2.save("output.png", "PNG"))
    {
        qDebug() << "Failed to save image";

        return 1;
    }

    img2.save("egl.jpg");
    std::cout << "Render Time : " << t.elapsed() << " : " << t.nsecsElapsed() << std::endl;

    //// Write all pixels to file
    // FILE *output = fopen("triangle.raw", "wb");

    // if (output)
    // {
    // fwrite(buffer, 1, desiredWidth * desiredHeight * 3, output);
    // fclose(output);
    // }
    // else
    // {
    // fprintf(stderr, "Failed to open file triangle.raw for writing!\n");
    // }

    return 0;
}
