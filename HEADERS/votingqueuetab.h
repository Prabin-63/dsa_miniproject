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

//uses queue
namespace Ui { class VotingQueueTab; }

class VotingQueueTab : public QWidget {
    Q_OBJECT

public:
    explicit VotingQueueTab(AppState* state, QWidget* parent = nullptr);
    ~VotingQueueTab();

    void refreshCandidateCombo();

signals:

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
