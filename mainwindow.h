#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "appstate.h"
#include "candidatetab.h"
#include "votertab.h"
#include "votingqueuetab.h"
#include "resultstab.h"


// MainWindow
namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow*    ui;
    AppState*          m_state;       // Single source of truth

    // One object per tab
    CandidateTab*      m_candidateTab;
    VoterTab*          m_voterTab;
    VotingQueueTab*    m_votingQueueTab;
    ResultsTab*        m_resultsTab;

    void wireCrossTabSignals();
    void applyGlobalStyles();
};

#endif // MAINWINDOW_H

