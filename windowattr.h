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

        /** Zwraca ID okna w liczbie o podstawie 10 */
        int getWId() const;

        /** Zwraca Pid procesu okna */
        int getPid() const;

        /** Zwraca nazwe procesu okna */
        QString getWName() const;

    private:

        /** Wywoluje polecenie systemowe i zwraca rezultat
         * @param cmd - polecenie systemowe
         */
        QString exec(QString cmd);

        /** Uaktualnia pole m_WId */
        void findActiveWindowWId();

        /** Uaktualnia pole m_PID */
        void findPid(int WId);

        /** Uaktualnia pole m_WName */
        void findWName();

        int m_WId;
        int m_PID;
        QString m_WName;
};

#endif // WINDOW_ATTR_H
