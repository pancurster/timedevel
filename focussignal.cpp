#include "focussignal.h"

FocusSignal::FocusSignal(QObject* parent) : QObject(parent)
{
}

void FocusSignal::emitFocusChanged()
{
    emit focusChanged();
}

