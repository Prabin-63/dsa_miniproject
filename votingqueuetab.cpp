#include "votingqueuetab.h"
#include "ui_votingqueuetab.h"
#include <QMessageBox>
#include <QDateTime>

VotingQueueTab::VotingQueueTab(AppState* state, QWidget* parent)
    : QWidget(parent), ui(new Ui::VotingQueueTab), m_state(state)
{
    ui->setupUi(this);
    styleWidgets();
    refreshCandidateCombo();
    refreshQueueDisplay();

    connect(ui->enqueueBtn, &QPushButton::clicked, this, &VotingQueueTab::onEnqueueClicked);
    connect(ui->processBtn, &QPushButton::clicked, this, &VotingQueueTab::onProcessClicked);
}

VotingQueueTab::~VotingQueueTab() { delete ui; }

void VotingQueueTab::refreshCandidateCombo() {
    ui->candidateCombo->clear();
    for (auto* n : m_state->candidateBST.inOrder())
        ui->candidateCombo->addItem(
            QString("[%1]  %2  —  %3").arg(n->candidateId).arg(n->name).arg(n->party),
            n->candidateId);
}

void VotingQueueTab::onEnqueueClicked() {
    int voterId = ui->voterIdSpin->value();

    VoterNode* voter = m_state->voterList.findById(voterId);
    if (!voter) {
        appendLog(QString("⚠  Voter ID %1 not found in registry.").arg(voterId));
        return;
    }
    if (voter->hasVoted) {
        appendLog(QString("⚠  %1 has already voted.").arg(voter->name));
        return;
    }

    for (int id : m_state->votingQueue.toVector()) {
        if (id == voterId) {
            appendLog(QString("⚠  %1 is already in the queue.").arg(voter->name));
            return;
        }
    }

    m_state->votingQueue.enqueue(voterId);

    appendLog(QString("[%1]  ✅ Enqueued: %2 (ID %3)")
                  .arg(QTime::currentTime().toString("hh:mm:ss"))
                  .arg(voter->name).arg(voterId));

    refreshQueueDisplay();
}

void VotingQueueTab::onProcessClicked() {
    if (m_state->votingQueue.isEmpty()) {
        appendLog("⚠  Queue is empty — no one to process.");
        return;
    }
    if (ui->candidateCombo->count() == 0) {
        appendLog("⚠  No candidates registered yet.");
        return;
    }

    int voterId = m_state->votingQueue.dequeue();
    int candId  = ui->candidateCombo->currentData().toInt();

    VoterNode* voter = m_state->voterList.findById(voterId);
    if (voter) {
        voter->hasVoted   = true;
        voter->votedForId = candId;
    }

    m_state->candidateBST.addVote(candId);

    BSTNode* cand     = m_state->candidateBST.search(candId);
    QString candName  = cand  ? cand->name  : QString::number(candId);
    QString voterName = voter ? voter->name : QString::number(voterId);

    appendLog(QString("[%1]  🗳 Vote cast: %2 → %3  (total: %4)")
                  .arg(QTime::currentTime().toString("hh:mm:ss"))
                  .arg(voterName).arg(candName)
                  .arg(cand ? cand->voteCount : 0));

    refreshQueueDisplay();
    emit voteProcessed();
}

void VotingQueueTab::refreshQueueDisplay() {
    ui->queueList->clear();
    QVector<int> q = m_state->votingQueue.toVector();

    for (int i = 0; i < q.size(); ++i) {
        VoterNode* v = m_state->voterList.findById(q[i]);
        QString label;
        if (i == 0)
            label = QString("▶  FRONT  |  ID %1  —  %2").arg(q[i]).arg(v ? v->name : "?");
        else
            label = QString("   %1.       ID %2  —  %3").arg(i+1).arg(q[i]).arg(v ? v->name : "?");
        ui->queueList->addItem(label);
        if (i == 0)
            ui->queueList->item(i)->setForeground(QColor("#E65100"));
    }

    ui->queueSizeLabel->setText(QString("Queue Size: %1").arg(m_state->votingQueue.count));
}

void VotingQueueTab::appendLog(const QString& msg) {
    ui->voteLog->append(msg);
}

void VotingQueueTab::styleWidgets() {
    ui->panelTitle->setStyleSheet(
        "font-size:16px; font-weight:700; color:#E65100; "
        "padding-bottom:6px; border-bottom:2px solid #FF6D00;");
    ui->queueTitle->setStyleSheet("font-size:14px; font-weight:600; color:#37474F;");
    ui->logTitle->setStyleSheet("font-size:13px; font-weight:600; color:#37474F;");

    for (auto* lbl : {ui->lblVoterId, ui->lblCandidate})
        lbl->setStyleSheet("font-size:12px; font-weight:600; color:#455A64;");

    ui->voterIdSpin->setStyleSheet(
        "QSpinBox { border:2px solid #B0BEC5; border-radius:8px; padding:6px 10px; font-size:13px; }"
        "QSpinBox:focus { border-color:#FF6D00; }");
    ui->candidateCombo->setStyleSheet(
        "QComboBox { border:2px solid #B0BEC5; border-radius:8px; padding:6px 10px; font-size:13px; }"
        "QComboBox:focus { border-color:#FF6D00; }");

    ui->enqueueBtn->setStyleSheet(
        "QPushButton { background:#FF6D00; color:white; border-radius:8px; "
        "padding:9px; font-size:13px; font-weight:600; }"
        "QPushButton:hover { background:#E65100; }"
        "QPushButton:pressed { background:#BF360C; }");
    ui->processBtn->setStyleSheet(
        "QPushButton { background:#00897B; color:white; border-radius:8px; "
        "padding:9px; font-size:13px; font-weight:600; }"
        "QPushButton:hover { background:#00796B; }"
        "QPushButton:pressed { background:#004D40; }");

    ui->queueSizeLabel->setStyleSheet(
        "font-size:14px; font-weight:700; color:#FFFFFF; background:#FF6D00; "
        "border-radius:8px; padding:8px;");

    ui->queueList->setStyleSheet(
        "QListWidget { border:1px solid #E0E0E0; border-radius:8px; font-size:13px; }"
        "QListWidget::item { padding:8px 12px; border-bottom:1px solid #F5F5F5; }");

    ui->voteLog->setStyleSheet(
        "QTextEdit { border:1px solid #E0E0E0; border-radius:8px; "
        "font-family:monospace; font-size:12px; background:#FAFAFA; color:#37474F; }");

    ui->leftPanel->setStyleSheet(
        "QFrame { background:#FFFFFF; border-radius:12px; border:1px solid #E0E0E0; }");
    ui->rightPanel->setStyleSheet(
        "QFrame { background:#FFFFFF; border-radius:12px; border:1px solid #E0E0E0; }");
}
