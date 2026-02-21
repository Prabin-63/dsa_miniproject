#include "resultstab.h"
#include "ui_resultstab.h"
#include <QHeaderView>

ResultsTab::ResultsTab(AppState* state, QWidget* parent)
    : QWidget(parent), ui(new Ui::ResultsTab), m_state(state)
{
    ui->setupUi(this);
    styleWidgets();

    connect(ui->computeBtn, &QPushButton::clicked, this, &ResultsTab::onComputeClicked);
}

ResultsTab::~ResultsTab() { delete ui; }

void ResultsTab::refresh() { onComputeClicked(); }


void ResultsTab::onComputeClicked() {
    // BST IN-ORDER
    QVector<CandidateResult> results;
    for (auto* n : m_state->candidateBST.inOrder())
        results.append({n->candidateId, n->name, n->party, n->voteCount});

    if (results.isEmpty()) {
        ui->winnerLabel->setText("🏆  No candidates registered yet.");
        return;
    }

    //Apply chosen sorting algorithm descending by votes
    int algo = ui->algoCombo->currentIndex();
    if (algo == 0)
        SortingAlgorithms::bubbleSort(results);
    else if (algo == 1)
        SortingAlgorithms::selectionSort(results);
    else
        SortingAlgorithms::mergeSort(results, 0, results.size() - 1);

    //Populate table
    populateTable(results);

    // Announce winner
    int total = 0;
    for (auto& r : results) total += r.votes;

    if (results[0].votes > 0) {
        ui->winnerLabel->setText(
            QString("🏆  Winner: %1  (%2)  —  %3 votes")
                .arg(results[0].name).arg(results[0].party).arg(results[0].votes));
        ui->winnerLabel->setStyleSheet(
            "font-size:20px; font-weight:700; color:#FFFFFF; "
            "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
            "  stop:0 #1565C0, stop:1 #283593); "
            "border-radius:10px; padding:12px;");
    } else {
        ui->winnerLabel->setText("🏆  Winner: — (no votes cast yet)");
        ui->winnerLabel->setStyleSheet(
            "font-size:18px; font-weight:600; color:#78909C; "
            "background:#ECEFF1; border-radius:10px; padding:12px;");
    }

    ui->totalVotesLabel->setText(QString("Total votes: %1").arg(total));
}

void ResultsTab::populateTable(const QVector<CandidateResult>& sorted) {
    static const QStringList medals = {"1st", "2nd", "3rd"};
    static const QList<QColor> medalBg = {
        QColor("#FFF9C4"), QColor("#F3F3F3"), QColor("#FBE9E7")
};

ui->resultsTable->setRowCount(sorted.size());
for (int i = 0; i < sorted.size(); ++i) {
    QString rank = (i < 3) ? medals[i] : QString("%1th").arg(i + 1);
    QColor bg = (i < 3) ? medalBg[i] : QColor("#FFFFFF");

    QList<QTableWidgetItem*> items = {
        new QTableWidgetItem(rank),
        new QTableWidgetItem(QString::number(sorted[i].id)),
        new QTableWidgetItem(sorted[i].name),
        new QTableWidgetItem(sorted[i].party),
        new QTableWidgetItem(QString::number(sorted[i].votes))
    };
    for (int c = 0; c < 5; ++c) {
        items[c]->setBackground(bg);
        items[c]->setForeground(QColor("#212121"));
        items[c]->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        if (i == 0) items[c]->setFont(QFont("", -1, QFont::Bold));
        ui->resultsTable->setItem(i, c, items[c]);
    }
}
}
//UI
void ResultsTab::styleWidgets() {
    ui->winnerLabel->setStyleSheet(
        "font-size:18px; font-weight:600; color:#78909C; "
        "background:#ECEFF1; border-radius:10px; padding:12px;");

    ui->algoLabel->setStyleSheet("font-size:13px; font-weight:600; color:#37474F;");
    ui->totalVotesLabel->setStyleSheet(
        "font-size:13px; font-weight:700; color:#37474F; "
        "background:#ECEFF1; border-radius:6px; padding:6px 12px;");

    ui->algoCombo->setStyleSheet(
        "QComboBox { border:2px solid #B0BEC5; border-radius:8px; "
        "padding:6px 10px; font-size:13px; min-width:200px; }"
        "QComboBox:focus { border-color:#1565C0; }");

    ui->computeBtn->setStyleSheet(
        "QPushButton { background:#1565C0; color:white; border-radius:8px; "
        "padding:9px 18px; font-size:13px; font-weight:600; }"
        "QPushButton:hover { background:#0D47A1; }"
        "QPushButton:pressed { background:#01579B; }");

    ui->resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->resultsTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section { background:#1565C0; color:white; font-weight:700; "
        "font-size:12px; padding:8px; border:none; }");
    ui->resultsTable->setStyleSheet(
        "QTableWidget { border:1px solid #E0E0E0; border-radius:8px; "
        "gridline-color:#F5F5F5; font-size:13px; }"
        "QTableWidget::item { padding:8px; }"
        "QTableWidget::item:selected { background:#BBDEFB; color:#0D47A1; }");
}
