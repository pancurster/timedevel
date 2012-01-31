#ifndef TIMER_ACTIVATOR_H
#define TIMER_ACTIVATOR_H

#include "focusactivator.h"

class QTimer;

class TimerActivator : public FocusActivator {

    Q_OBJECT

    signals:
        void focusChanged();

    public:
        TimerActivator(int timeOutSec, QObject* parent = 0);
        void setTimeOut(int timeOutSec);

    private slots:
        void przerwanieT();

    private:
        int m_interval;
        QTimer* m_timer;
};

#endif // TIMER_ACTIVATOR_H
