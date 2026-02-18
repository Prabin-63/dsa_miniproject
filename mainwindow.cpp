#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_state(new AppState())
{
    ui->setupUi(this);
    applyGlobalStyles();

    // Seeded some demo data so the app isn't blank on launch,
    m_state->seedDemo();

    // Created each tab, passing the shared AppState
    m_candidateTab   = new CandidateTab(m_state,   this);
    m_voterTab       = new VoterTab(m_state,        this);
    m_votingQueueTab = new VotingQueueTab(m_state,  this);
    m_resultsTab     = new ResultsTab(m_state,      this);
    m_dsaTab         = new DsaVisualizerTab(m_state, this);

    // Added tabs to the QTabWidget from mainwindow.ui
    ui->tabWidget->addTab(m_candidateTab,   "🏛   Candidates");
    ui->tabWidget->addTab(m_voterTab,       "👥   Voters");
    ui->tabWidget->addTab(m_votingQueueTab, "🗳   Voting Queue");
    ui->tabWidget->addTab(m_resultsTab,     "📊   Results");
    ui->tabWidget->addTab(m_dsaTab,         "🌲   DSA View");

    // ── Wire signals so tabs stay in sync ─────────────────────
    wireCrossTabSignals();

    // Status bar
    ui->statusBar->showMessage("Ready — Demo data loaded. Add candidates/voters and start voting!");
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_state;
}


void MainWindow::wireCrossTabSignals() {

    // When a candidate is added/removed:
    connect(m_candidateTab, &CandidateTab::candidatesChanged, this, [this]() {
        m_votingQueueTab->refreshCandidateCombo();  // keep combo up to date
        m_dsaTab->refresh();
        ui->statusBar->showMessage("Candidate list updated.");
    });

    // When a voter is added/removed:
    connect(m_voterTab, &VoterTab::votersChanged, this, [this]() {
        m_dsaTab->refresh();
        ui->statusBar->showMessage("Voter registry updated.");
    });

    // When a vote is processed:
    connect(m_votingQueueTab, &VotingQueueTab::voteProcessed, this, [this]() {
        m_candidateTab->refresh();           // update vote counts column
        m_voterTab->refresh();               // mark voter as voted
        m_resultsTab->refresh();             // auto-refresh results
        m_dsaTab->refresh();                 // update all visualizations
        ui->statusBar->showMessage("Vote processed successfully.");
    });
}

// ─────────────────────────────────────────────────────────────
void MainWindow::applyGlobalStyles() {
    // Header bar
    ui->headerBar->setStyleSheet(
        "QFrame { background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "  stop:0 #1A237E, stop:1 #283593); }");
    ui->appTitle->setStyleSheet(
        "font-size:22px; font-weight:700; color:#FFFFFF; letter-spacing:1px;");
    ui->appSubtitle->setStyleSheet(
        "font-size:12px; color:#9FA8DA; margin-left:16px;");
    ui->versionLabel->setStyleSheet(
        "font-size:11px; color:#7986CB; font-weight:600;");

    // Tab bar
    ui->tabWidget->setStyleSheet(
        "QTabWidget::pane {"
        "  border:none;"
        "  background:#F5F6FA;"
        "}"
        "QTabBar::tab {"
        "  padding:11px 22px;"
        "  font-size:13px;"
        "  font-weight:500;"
        "  color:#546E7A;"
        "  background:#ECEFF1;"
        "  border-bottom:3px solid transparent;"
        "  margin-right:2px;"
        "}"
        "QTabBar::tab:selected {"
        "  color:#1A237E;"
        "  background:#FFFFFF;"
        "  border-bottom:3px solid #3949AB;"
        "  font-weight:700;"
        "}"
        "QTabBar::tab:hover:!selected {"
        "  background:#E8EAF6;"
        "  color:#3949AB;"
        "}");

    // Global window background
    setStyleSheet("QMainWindow { background:#F5F6FA; }");

    // Status bar
    ui->statusBar->setStyleSheet(
        "QStatusBar { background:#ECEFF1; color:#546E7A; font-size:12px; padding:4px; }");
}
