#ifndef FOCUS_SIGNAL_H
#define FOCUS_SIGNAL_H

#include <QObject>

class FocusSignal : public QObject {

    Q_OBJECT

    signals:
        void focusChanged();

    public:
        FocusSignal(QObject* parent = 0);
        void emitFocusChanged();
};

#endif // FOCUS_SIGNAL_H
