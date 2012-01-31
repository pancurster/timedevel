#include "focussignal.h"

FocusSignal::FocusSignal(QObject* parent) : FocusActivator(parent)
{
}

void FocusSignal::emitFocusChanged()
{
    emit focusChanged();
}

