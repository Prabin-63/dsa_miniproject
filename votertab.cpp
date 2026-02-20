#include "votertab.h"
#include "ui_votertab.h"
#include <QMessageBox>
#include <QHeaderView>

VoterTab::VoterTab(AppState* state, QWidget* parent)
    : QWidget(parent), ui(new Ui::VoterTab), m_state(state)
{
    ui->setupUi(this);
    styleWidgets();
    populateTable();

    connect(ui->addBtn,    &QPushButton::clicked, this, &VoterTab::onAddClicked);
    connect(ui->removeBtn, &QPushButton::clicked, this, &VoterTab::onRemoveClicked);
}

VoterTab::~VoterTab() { delete ui; }

void VoterTab::refresh() { populateTable(); }

// ── Slots ─────────────────────────────────────────────────────
void VoterTab::onAddClicked() {
    QString name = ui->nameInput->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Missing Input", "Please enter the voter's name.");
        return;
    }

    // LINKED LIST INSERT — appends to end: O(n)
    m_state->voterList.insert(m_state->nextVoterId++, name);

    ui->nameInput->clear();
    populateTable();
    emit votersChanged();
}

void VoterTab::onRemoveClicked() {
    int row = ui->voterTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "No Selection", "Please select a voter row first.");
        return;
    }

    int id = ui->voterTable->item(row, 0)->text().toInt();

    // LINKED LIST SEARCH — O(n)
    VoterNode* vn = m_state->voterList.findById(id);
    if (vn && vn->hasVoted) {
        QMessageBox::warning(this, "Cannot Remove",
                             QString("%1 has already voted. Cannot remove from registry.").arg(vn->name));
        return;
    }

    // LINKED LIST DELETE — O(n)
    m_state->voterList.deleteById(id);
    populateTable();
    emit votersChanged();
}

// ── Helpers ───────────────────────────────────────────────────
void VoterTab::populateTable() {
    // LINKED LIST TRAVERSE — O(n)
    QVector<VoterNode*> voters = m_state->voterList.toVector();

    ui->voterTable->setRowCount(voters.size());
    for (int i = 0; i < voters.size(); ++i) {
        VoterNode* v = voters[i];
        ui->voterTable->setItem(i, 0, new QTableWidgetItem(QString::number(v->voterId)));
        ui->voterTable->setItem(i, 1, new QTableWidgetItem(v->name));
        ui->voterTable->setItem(i, 2, new QTableWidgetItem(v->hasVoted ? "Yes" : "No"));
        ui->voterTable->setItem(i, 3, new QTableWidgetItem(
                                          v->hasVoted ? QString::number(v->votedForId) : "—"));

        // Green row for voters who have voted
        QColor bg = v->hasVoted ? QColor("#E8F5E9") : QColor("#FFFFFF");
        QColor fg = v->hasVoted ? QColor("#1B5E20") : QColor("#212121");
        for (int c = 0; c < 4; ++c) {
            ui->voterTable->item(i, c)->setBackground(bg);
            ui->voterTable->item(i, c)->setForeground(fg);
        }
    }

    ui->countLabel->setText(QString("Total Voters: %1").arg(voters.size()));
}

void VoterTab::styleWidgets() {
    ui->panelTitle->setStyleSheet(
        "font-size:16px; font-weight:700; color:#1B5E20; "
        "padding-bottom:6px; border-bottom:2px solid #388E3C;");
    ui->tableTitle->setStyleSheet("font-size:14px; font-weight:600; color:#37474F;");
    ui->lblName->setStyleSheet("font-size:12px; font-weight:600; color:#455A64;");

    QString inputStyle = "QLineEdit { border:2px solid #B0BEC5; border-radius:8px; "
                         "padding:7px 10px; font-size:13px; background:#FAFAFA; color:#000000; }"
                         "QLineEdit:focus { border-color:#3949AB; background:#FFFFFF; color:#000000; }";
    ui->nameInput->setStyleSheet(inputStyle);

    ui->addBtn->setStyleSheet(
        "QPushButton { background:#388E3C; color:white; border-radius:8px; "
        "padding:9px; font-size:13px; font-weight:600; }"
        "QPushButton:hover { background:#2E7D32; }"
        "QPushButton:pressed { background:#1B5E20; }");
    ui->removeBtn->setStyleSheet(
        "QPushButton { background:#F4511E; color:white; border-radius:8px; "
        "padding:9px; font-size:13px; font-weight:600; }"
        "QPushButton:hover { background:#D84315; }"
        "QPushButton:pressed { background:#BF360C; }");


    ui->countLabel->setStyleSheet(
        "font-size:14px; font-weight:700; color:#FFFFFF; background:#388E3C; "
        "border-radius:8px; padding:8px;");

    ui->voterTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->voterTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section { background:#388E3C; color:white; font-weight:700; "
        "font-size:12px; padding:8px; border:none; }");
    ui->voterTable->setStyleSheet(
        "QTableWidget { border:1px solid #E0E0E0; border-radius:8px; gridline-color:#F5F5F5; }"
        "QTableWidget::item { padding:6px; }"
        "QTableWidget::item:selected { background:#C8E6C9; color:#1B5E20; }");

    ui->leftPanel->setStyleSheet(
        "QFrame { background:#FFFFFF; border-radius:12px; border:1px solid #E0E0E0; }");
    ui->rightPanel->setStyleSheet(
        "QFrame { background:#FFFFFF; border-radius:12px; border:1px solid #E0E0E0; }");
}
