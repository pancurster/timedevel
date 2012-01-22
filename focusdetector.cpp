/* focusdetector.cpp */
#include "timedevel.h"
#include "focusdetector.h"
#include "focussignal.h"

#include <X11/Xlib.h>
#include <QDebug>

FocusDetector::FocusDetector() : signalHandle(new FocusSignal)
{
}

bool FocusDetector::x11EventFilter(XEvent* event)
{
    if (event->type == EnterNotify) {
    //if (event->type == VisibilityNotify) {
        reportFocusChange();
        return true;
    } else {
        return false;
    }
}

void FocusDetector::reportFocusChange()
{
    qDebug() << "Zmieniono focus";
    signalHandle->emitFocusChanged();
}

