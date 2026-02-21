#ifndef CANDIDATETAB_H
#define CANDIDATETAB_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "appstate.h"


namespace Ui { class CandidateTab; }

class CandidateTab : public QWidget {
    Q_OBJECT

public:
    explicit CandidateTab(AppState* state, QWidget* parent = nullptr);
    ~CandidateTab();

    // Called by MainWindow whenever another tab changes candidate data
    void refresh();

signals:
    // Used to pass BST
    void candidatesChanged();

private slots:
    void onAddClicked();
    void onRemoveClicked();

private:
    Ui::CandidateTab* ui;
    AppState*         m_state;

    void populateTable();
    void styleWidgets();
};

#endif // CANDIDATETAB_H
