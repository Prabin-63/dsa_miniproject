#ifndef RESULTSTAB_H
#define RESULTSTAB_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "appstate.h"
//Sorting and display results using 3 sorting algo
namespace Ui { class ResultsTab; }

class ResultsTab : public QWidget {
    Q_OBJECT

public:
    explicit ResultsTab(AppState* state, QWidget* parent = nullptr);
    ~ResultsTab();

    void refresh();

private slots:
    void onComputeClicked();

private:
    Ui::ResultsTab* ui;
    AppState*       m_state;

    void populateTable(const QVector<CandidateResult>& sorted);
    void styleWidgets();
};

#endif // RESULTSTAB_H
