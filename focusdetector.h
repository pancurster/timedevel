#ifndef FOCUSDETECTOR_H
#define FOCUSDETECTOR_H

#include <QDebug>
#include <QxtGui/QxtNativeEventFilter>
#include <X11/Xlib.h>

class FocusDetector : public QxtNativeEventFilter {
    public:
        FocusDetector(MainWindow* mw);
        bool x11EventFilter(XEvent* event);

    private:
        void reportFocusChange();

        MainWindow* m_mw;
};

#endif //FOCUSDETECTOR_H
