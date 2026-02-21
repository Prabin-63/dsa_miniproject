#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_state(new AppState())
{
    ui->setupUi(this);
    applyGlobalStyles();

    // demo seed
    m_state->seedDemo();

    m_candidateTab   = new CandidateTab(m_state,   this);
    m_voterTab       = new VoterTab(m_state,        this);
    m_votingQueueTab = new VotingQueueTab(m_state,  this);
    m_resultsTab     = new ResultsTab(m_state,      this);


    ui->tabWidget->addTab(m_candidateTab,   "🏛   Candidates");
    ui->tabWidget->addTab(m_voterTab,       "👥   Voters");
    ui->tabWidget->addTab(m_votingQueueTab, "🗳   Voting Queue");
    ui->tabWidget->addTab(m_resultsTab,     "📊   Results");
    wireCrossTabSignals();

}


MainWindow::~MainWindow() {
    delete ui;
    delete m_state;
}


void MainWindow::wireCrossTabSignals() {

    // When a candidate is added/removed
    connect(m_candidateTab, &CandidateTab::candidatesChanged, this, [this]() {
        m_votingQueueTab->refreshCandidateCombo();  // keep combo up to date

        ui->statusBar->showMessage("Candidate list updated.");
    });

    // When a voter is added/removed
    connect(m_voterTab, &VoterTab::votersChanged, this, [this]() {

        ui->statusBar->showMessage("Voter registry updated.");
    });

    // When a vote is processed
    connect(m_votingQueueTab, &VotingQueueTab::voteProcessed, this, [this]() {
        m_candidateTab->refresh();           // update vote counts column
        m_voterTab->refresh();               // mark voter as voted
        m_resultsTab->refresh();             // auto-refresh results

        ui->statusBar->showMessage("Vote processed successfully.");
    });
}
void MainWindow::applyGlobalStyles() {
    ui->headerBar->setStyleSheet(
        "QFrame { background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "  stop:0 #1A237E, stop:1 #283593); }");
    ui->appTitle->setStyleSheet(
        "font-size:22px; font-weight:700; color:#FFFFFF; letter-spacing:1px;");
    ui->appSubtitle->setStyleSheet(
        "font-size:12px; color:#9FA8DA; margin-left:16px;");


    ui->tabWidget->setStyleSheet(
        "QTabWidget::pane { border:none; background:#F5F6FA; }"
        "QTabBar::tab {"
        "  padding:11px 22px; font-size:13px; font-weight:500;"
        "  color:#546E7A; background:#ECEFF1;"
        "  border-bottom:3px solid transparent; margin-right:2px; }"
        "QTabBar::tab:selected {"
        "  color:#1A237E; background:#FFFFFF;"
        "  border-bottom:3px solid #3949AB; font-weight:700; }"
        "QTabBar::tab:hover:!selected {"
        "  background:#E8EAF6; color:#3949AB; }");


    ui->statusBar->setStyleSheet(
        "QStatusBar { background:#ECEFF1; color:#546E7A; font-size:12px; padding:4px; }");


    setStyleSheet(
        "QMainWindow { background:#F5F6FA; }"
        "QTableWidget { background:#FFFFFF; color:#212121; }"
        "QTableWidget QTableCornerButton::section { background:#E0E0E0; }"
        );
}
