/* focusdetector.cpp */
#include <QDebug>

#include "timedevel.h"
#include "focusdetector.h"
#include "focussignal.h"

#include <X11/Xlib.h>

FocusDetector::FocusDetector() : signalHandle(new FocusSignal)
{
}

FocusDetector::~FocusDetector()
{
    delete signalHandle;
    signalHandle = 0;
}

bool FocusDetector::x11EventFilter(XEvent* event)
{
    if (event->type == EnterNotify ||
        event->type == MapNotify ||
        event->type == VisibilityNotify) {
        reportFocusChange();
        return true;
    } else {
        return false;
    }
}

void FocusDetector::reportFocusChange() const
{
    qDebug() << "Zmieniono focus";
    signalHandle->emitFocusChanged();
}

