#ifndef FOCUS_SIGNAL_H
#define FOCUS_SIGNAL_H

#include "focusactivator.h"

/** Obiekt wysyła sygnał zmiany focusa.
 * Urzywany przez klasę FocusDetector.
 * @see FocusDetector
 */
class FocusSignal : public FocusActivator {

    Q_OBJECT

    signals:
        /** Sygnal emitowany jeśli nastąpiła zmiana focusu w
         * menedżerze okien.
         * Narazie działa tylko dla systemów z X11.
         */
        void focusChanged();

    public:
        /// Konstruktor klasy \a FocusSignal.
        FocusSignal(QObject* parent = 0);

        /// Funkcja emituje sygnał \a focusChanged().
        void emitFocusChanged();
};

#endif // FOCUS_SIGNAL_H
