#include <QDebug>
#include <QTimer>

#include "timeractivator.h"

TimerActivator::TimerActivator(int timeOutSec, QObject* parent)
    : FocusActivator(parent),
      m_interval(1000*timeOutSec),
      m_timer(new QTimer(this))
{
    Q_ASSERT(timeOutSec >= 1);
    connect(m_timer, SIGNAL(timeout()), this, SIGNAL(focusChanged()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(przerwanieT()));
    m_timer->start(m_interval);
}

void TimerActivator::setTimeOut(int timeOutSec)
{
    m_interval = 1000*timeOutSec;
    m_timer->stop();
    m_timer->start(m_interval);
}

void TimerActivator::przerwanieT()
{
    qDebug() << Q_FUNC_INFO << ": Timer zadzialal";
}

