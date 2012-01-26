#ifndef FOCUSDETECTOR_H
#define FOCUSDETECTOR_H

#include <QxtGui/QxtNativeEventFilter>

#include "focussignal.h"

class FocusSignal;

/** Wykrywa i obsluguje zdarzenia zmiany focusu w systemie.
 * Do działania korzysta z mechanizmu sygnałów Qt. Nie można jednocześnie
 * dziedziczyć po QxtNativeEventFilter i po QObject więc w funkcji
 * reportFocusChange tworzony jest maly obiekt emitujacy sygnał zmiany focusa.
 * @see FocusSignal
 */
class FocusDetector : public QxtNativeEventFilter {

    public:
        /** Tworzy obiekt \a FocusSignal */
        FocusDetector();

        /** Sprzata \a FocusSignal */
        ~FocusDetector();
        
        /** Główna funkcja filtrująca zdarzenia. Zasłania x11EventFilter(...)
         * z QxtNativeEventFilter
         */
        bool x11EventFilter(XEvent* event);

        /**
         * Uchwyt do obiektu wysyłającego sygnał.
         * @see FocusSignal
         */
        FocusSignal* signalHandle;

    private:
        /** Wywołuje funkcję z klasy FocusSignal która emituje sygnał
         * zmiany focusa.
         */
        void reportFocusChange() const;
};

#endif //FOCUSDETECTOR_H
