#ifndef VOTERTAB_H
#define VOTERTAB_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "appstate.h"

//uses linked list
namespace Ui { class VoterTab; }

class VoterTab : public QWidget {
    Q_OBJECT

public:
    explicit VoterTab(AppState* state, QWidget* parent = nullptr);
    ~VoterTab();

    void refresh();   // Called from MainWindow when vote is processed

signals:
    void votersChanged();

private slots:
    void onAddClicked();
    void onRemoveClicked();

private:
    Ui::VoterTab* ui;
    AppState*     m_state;

    void populateTable();
    void styleWidgets();
};

#endif // VOTERTAB_H
