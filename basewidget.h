/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_BASEWIDGET_H
#define SEKURA_BASEWIDGET_H

#include <QWidget>

namespace Sekura {

    /*!
     * \brief The BaseWidget class - базовый класс для всех форм
     */
    class BaseWidget : public QWidget {
        Q_OBJECT
      public:
        explicit BaseWidget(QWidget *parent = nullptr);
        void setMainForm(bool m) { m_mainForm = m; }
        bool mainForm() const { return m_mainForm; }

      signals:
        void appendWidget(Sekura::BaseWidget *);
        void parentReload();
        void closeParent();
        void idChanged(const QString &table, const QString &id);

      public slots:
        virtual void changeId(const QString &table, const QString &id);

      protected:
        bool m_mainForm;
    };

} // namespace Sekura

#endif // SEKURA_BASEWIDGET_H
