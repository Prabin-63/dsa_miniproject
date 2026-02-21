#include "candidatetab.h"
#include "ui_candidatetab.h"
#include <QMessageBox>
#include <QHeaderView>

CandidateTab::CandidateTab(AppState* state, QWidget* parent)
    : QWidget(parent), ui(new Ui::CandidateTab), m_state(state)
{
    ui->setupUi(this);
    styleWidgets();
    populateTable();

    connect(ui->addBtn,    &QPushButton::clicked, this, &CandidateTab::onAddClicked);
    connect(ui->removeBtn, &QPushButton::clicked, this, &CandidateTab::onRemoveClicked);
}

CandidateTab::~CandidateTab() { delete ui; }


void CandidateTab::refresh() { populateTable(); }


void CandidateTab::onAddClicked() {
    QString name  = ui->nameInput->text().trimmed();
    QString party = ui->partyInput->text().trimmed();

    if (name.isEmpty() || party.isEmpty()) {
        QMessageBox::warning(this, "Missing Input", "Please fill in both Name and Party.");
        return;
    }

    // BST insertion
    m_state->candidateBST.insert(m_state->nextCandidateId++, name, party);

    ui->nameInput->clear();
    ui->partyInput->clear();
    populateTable();
    emit candidatesChanged();
}

void CandidateTab::onRemoveClicked() {
    int row = ui->candidateTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "No Selection", "Please select a candidate row first.");
        return;
    }

    int id = ui->candidateTable->item(row, 0)->text().toInt();

    //  cannot remove candidate with votes
    BSTNode* node = m_state->candidateBST.search(id);
    if (node && node->voteCount > 0) {
        QMessageBox::warning(this, "Cannot Remove",
                             QString("%1 already has %2 vote(s). Cannot remove.").arg(node->name).arg(node->voteCount));
        return;
    }

    // BST deletion
    m_state->candidateBST.remove(id);
    populateTable();
    emit candidatesChanged();
}

void CandidateTab::populateTable() {
    QVector<BSTNode*> nodes = m_state->candidateBST.inOrder();
    ui->candidateTable->setRowCount(nodes.size());
    for (int i = 0; i < nodes.size(); ++i) {
        BSTNode* n = nodes[i];
        auto* id    = new QTableWidgetItem(QString::number(n->candidateId));
        auto* name  = new QTableWidgetItem(n->name);
        auto* party = new QTableWidgetItem(n->party);
        auto* votes = new QTableWidgetItem(QString::number(n->voteCount));


        QColor bg = (n->voteCount > 0) ? QColor("#E8F5E9") : QColor("#FFFFFF");
        QColor fg = (n->voteCount > 0) ? QColor("#1B5E20") : QColor("#212121");
        for (auto* item : {id, name, party, votes}) {
            item->setBackground(bg);
            item->setForeground(fg);
        }
        ui->candidateTable->setItem(i, 0, id);
        ui->candidateTable->setItem(i, 1, name);
        ui->candidateTable->setItem(i, 2, party);
        ui->candidateTable->setItem(i, 3, votes);
    }
    ui->countLabel->setText(QString("Total Candidates: %1").arg(nodes.size()));
}

void CandidateTab::styleWidgets() {

    ui->panelTitle->setStyleSheet(
        "font-size:16px; font-weight:700; color:#1A237E; "
        "padding-bottom:6px; border-bottom:2px solid #3949AB;");
    ui->tableTitle->setStyleSheet(
        "font-size:14px; font-weight:600; color:#37474F;");


    for (auto* lbl : {ui->lblName, ui->lblParty})
        lbl->setStyleSheet("font-size:12px; font-weight:600; color:#455A64;");

//inputstyle
    QString inputStyle = "QLineEdit { border:2px solid #B0BEC5; border-radius:8px; "
                         "padding:7px 10px; font-size:13px; background:#FAFAFA; color:#000000; }"
                         "QLineEdit:focus { border-color:#3949AB; background:#FFFFFF; color:#000000; }";
//UI
    ui->nameInput->setStyleSheet(inputStyle);
    ui->partyInput->setStyleSheet(inputStyle);


    ui->addBtn->setStyleSheet(
        "QPushButton { background:#3949AB; color:white; border-radius:8px; "
        "padding:9px; font-size:13px; font-weight:600; }"
        "QPushButton:hover { background:#303F9F; }"
        "QPushButton:pressed { background:#1A237E; }");
    ui->removeBtn->setStyleSheet(
        "QPushButton { background:#E53935; color:white; border-radius:8px; "
        "padding:9px; font-size:13px; font-weight:600; }"
        "QPushButton:hover { background:#C62828; }"
        "QPushButton:pressed { background:#B71C1C; }");


    ui->countLabel->setStyleSheet(
        "font-size:14px; font-weight:700; color:#FFFFFF; background:#3949AB; "
        "border-radius:8px; padding:8px;");

    // Table
    ui->candidateTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->candidateTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section { background:#3949AB; color:white; font-weight:700; "
        "font-size:12px; padding:8px; border:none; }");
    ui->candidateTable->setStyleSheet(
        "QTableWidget { border:1px solid #E0E0E0; border-radius:8px; gridline-color:#F5F5F5; }"
        "QTableWidget::item { padding:6px; }"
        "QTableWidget::item:selected { background:#C5CAE9; color:#1A237E; }");

    ui->leftPanel->setStyleSheet(
        "QFrame { background:#FFFFFF; border-radius:12px; border:1px solid #E0E0E0; }");
    ui->rightPanel->setStyleSheet(
        "QFrame { background:#FFFFFF; border-radius:12px; border:1px solid #E0E0E0; }");
}
