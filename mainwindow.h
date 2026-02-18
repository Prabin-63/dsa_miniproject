#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "appstate.h"
#include "candidatetab.h"
#include "votertab.h"
#include "votingqueuetab.h"
#include "resultstab.h"
#include "dsavisualizertab.h"

// ─────────────────────────────────────────────────────────────
// MainWindow
//   UI file : mainwindow.ui
//   Role    : Thin orchestrator — owns AppState, creates tabs,
//             connects cross-tab signals so when one tab changes
//             data the other tabs refresh automatically.
// ─────────────────────────────────────────────────────────────
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
    DsaVisualizerTab*  m_dsaTab;

    void wireCrossTabSignals();
    void applyGlobalStyles();
};

#endif // MAINWINDOW_H
˜
