#pragma once
#include <QWidget>

namespace datall
{
namespace bases
{
class CircularLoading: public QWidget
{
    Q_OBJECT

public:
    explicit CircularLoading(QWidget *parent = 0);

protected:
    void  paintEvent(QPaintEvent *);

    void  timerEvent(QTimerEvent *event);

    void  showEvent(QShowEvent *event);

private:
    qint64  m_value;
};
} // namespace bases
} // namespace datall
