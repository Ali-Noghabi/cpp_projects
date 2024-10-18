#ifndef RENDEREGL_H
#define RENDEREGL_H

#include <QObject>

class RenderEGL : public QObject
{
    Q_OBJECT
public:
    explicit RenderEGL(QObject *parent = nullptr);

signals:
};

#endif // RENDEREGL_H
