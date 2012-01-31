#ifndef FOCUS_ACTIVATOR_H
#define FOCUS_ACTIVATOR_H

#include <QObject>

class FocusActivator : public QObject {
    
    Q_OBJECT

    signals:
        virtual void focusChanged() = 0;

    public:
        FocusActivator(QObject* parent = 0);
};

#endif // FOCUS_ACTIVATOR_H
