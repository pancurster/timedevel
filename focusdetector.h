#ifndef FOCUSDETECTOR_H
#define FOCUSDETECTOR_H

#include <QDebug>
#include <QxtGui/QxtNativeEventFilter>
#include <X11/Xlib.h>

class Timedevel;

class FocusDetector : public QxtNativeEventFilter {
    public:
        FocusDetector(Timedevel* mw);
        bool x11EventFilter(XEvent* event);

    private:
        void reportFocusChange();

        Timedevel* m_mw;
};

#endif //FOCUSDETECTOR_H
