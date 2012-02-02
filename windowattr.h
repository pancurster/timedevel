#ifndef WINDOW_ATTR_H
#define WINDOW_ATTR_H

class QString;

class WindowAttr {
    public:
        /** Podstawowy konstruktor klasy WindowAttr */
        WindowAttr(int WId);

        /** 
         * Konstruktor wskazany kiedy nieznane jest WId aktywnego okna.
         */
        WindowAttr();

        /** Zwraca ID okna w postaci liczby o podstawie 10 */
        int getWId() const;

        /** Zwraca Pid procesu okna */
        int getPid() const;

        /** Zwraca tytul okna */
        QString getWName() const;

        /** Zwraca nazwe procesu okna */
        QString getAppName() const;

    private:

        /** Wywoluje polecenie systemowe i zwraca rezultat
         * @param cmd polecenie systemowe do wykonania
         * @return wynik dzialania polecenia \a cmd
         */
        QString exec(QString cmd);

        /// Odszukuje WindowID (X11) okna
        void findActiveWindowWId();

        /// Odszukuje PID programu
        void findPid(int WId);

        /// Odszukuje tytul okna
        void findWName(int WId);

        /// Odczytuje z /proc nazwe programu
        void findAppName();

        int m_WId;
        int m_PID;
        QString m_WName;
        QString m_AppName;
};

#endif // WINDOW_ATTR_H
