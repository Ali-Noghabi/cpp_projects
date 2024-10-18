#ifndef RENDEROFFSCREEN_H
#define RENDEROFFSCREEN_H

#include <QImage>
#include <QObject>
#include <GLES2/gl2.h>

class RenderOffScreen: public QObject
{
    Q_OBJECT

public:
    explicit RenderOffScreen(QObject *parent = nullptr);

    ~RenderOffScreen();

    void           setData(QList<QJsonArray> &data, QList<QJsonArray> &colorValues);

    void           setData2(QList<QJsonArray> &data, QList<QJsonArray> &colorValues);

    QImage         render(const QSize &size);

    QList<QColor>  colors() const;

    void           setColors(const QList<QColor> &newColors);

    QList<qreal>   colorStops() const;

    void           setColorStops(const QList<qreal> &newColorStops);

    void           getColorFromGradient(double value, GLfloat &r, GLfloat &g, GLfloat &b);

private:
    QList<QJsonArray>  m_data;
    QList<QJsonArray>  m_value;
    QList<double>      m_colorStops;
    QList<QColor>      m_colors;
    double             m_minColor;
    double             m_maxColor;
};


#endif // RENDEROFFSCREEN_H
