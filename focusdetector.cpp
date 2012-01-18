/* focusdetector.cpp */
#include "mainwindow.h"
#include "focusdetector.h"

#include <X11/Xlib.h>
#include <QDebug>

FocusDetector::FocusDetector(MainWindow* mw)
    : m_mw(mw)
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
    m_mw->processFocusChange();
}
