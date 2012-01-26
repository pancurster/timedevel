#ifndef FOCUSDETECTOR_H
#define FOCUSDETECTOR_H

#include <QDebug>
#include <QObject>

#include <QxtGui/QxtNativeEventFilter>

#include <X11/Xlib.h>

#include "focussignal.h"

class Timedevel;

class FocusDetector : public QxtNativeEventFilter {

    public:
        FocusDetector();
        ~FocusDetector();
        bool x11EventFilter(XEvent* event);

        FocusSignal* signalHandle;
    private:
        void reportFocusChange();
};

#endif //FOCUSDETECTOR_H
