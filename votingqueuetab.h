#ifndef VOTINGQUEUETAB_H
#define VOTINGQUEUETAB_H

#include <QWidget>
#include <QListWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include "appstate.h"

// ─────────────────────────────────────────────────────────────
// VotingQueueTab
//   UI file : votingqueuetab.ui
//   DSA used: VotingQueue (enqueue / dequeue / peek)
//             VoterLinkedList.findById — mark voter as voted
//             CandidateBST.addVote    — increment vote count
// ─────────────────────────────────────────────────────────────
namespace Ui { class VotingQueueTab; }

class VotingQueueTab : public QWidget {
    Q_OBJECT

public:
    explicit VotingQueueTab(AppState* state, QWidget* parent = nullptr);
    ~VotingQueueTab();

    // Rebuild candidate combo after candidate list changes
    void refreshCandidateCombo();

signals:
    // Emitted after a vote is fully processed
    void voteProcessed();

private slots:
    void onEnqueueClicked();
    void onProcessClicked();

private:
    Ui::VotingQueueTab* ui;
    AppState*           m_state;

    void refreshQueueDisplay();
    void appendLog(const QString& msg);
    void styleWidgets();
};

#endif // VOTINGQUEUETAB_H
