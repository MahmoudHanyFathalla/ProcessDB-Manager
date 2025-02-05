#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QProcess>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setFixedSize(800, 600);

    // Initialize SQLite database connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/hp/Desktop/sqlite/comp.db");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database" << db.lastError();
    }

    // Create a QSqlTableModel and set the table name
    QSqlTableModel *tableModel = new QSqlTableModel(this, db);
    tableModel->setTable("m");
    tableModel->select(); // Fetch the data

    // Set the model for the QTableView
    ui->tableView->setModel(tableModel);

    for (int i = 0; i < tableModel->columnCount(); ++i) {
        ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableView, &QTableView::clicked, this, &MainWindow::on_tableView_clicked);

    // Populate the QComboBox with data from the "m" table
    refreshComboBoxData();

    userEnteredData = false;

    // Connect signals to slots
    connect(ui->tableView, &QTableView::clicked, this, &MainWindow::on_tableView_clicked);
    connect(ui->AtextEdit, &QTextEdit::textChanged, this, &MainWindow::onDataChanged);
    connect(ui->LtextEdit, &QTextEdit::textChanged, this, &MainWindow::onDataChanged);

    externalProcess = new QProcess(this);
    isProcessRunning = false;

}

// Helper function to refresh data in the QComboBox
void MainWindow::refreshComboBoxData() {
    QSqlTableModel *comboModel = new QSqlTableModel(this, db);
    comboModel->setTable("m");
    comboModel->select(); // Fetch the data

    // Clear existing items in the combo box
    ui->comboBox->clear();

    // Populate the QComboBox with data from the "m" table
    for (int row = 0; row < comboModel->rowCount(); ++row) {
        QModelIndex indexId = comboModel->index(row, comboModel->fieldIndex("id"));
        QModelIndex indexName = comboModel->index(row, comboModel->fieldIndex("name"));

        QString chanelId = comboModel->data(indexId).toString();
        QString chanelName = comboModel->data(indexName).toString();

        // Assuming you want to display both chanelId and chanelName in the combo box
        QString displayText = chanelId + " - " + chanelName;

        ui->comboBox->addItem(displayText);
    }

    // Clean up the comboModel to prevent memory leaks
    delete comboModel;
}

MainWindow::~MainWindow() {
    delete ui;
    delete externalProcess;
}

void MainWindow::onDataChanged() {
    // Get the text from the text fields
    QString name = ui->AtextEdit->toPlainText();
    QString link = ui->LtextEdit->toPlainText();

    // Check if the text fields were initially empty
    bool fieldsWereEmpty = name.isEmpty() && link.isEmpty();

    // Set the flag to true when the user enters data into an existing text field
    // or enters text into an initially empty field
    userEnteredData = !fieldsWereEmpty;
}


void MainWindow::on_AddB_clicked() {
    // Get the text from the text fields
    QString name = ui->AtextEdit->toPlainText();
    QString link = ui->LtextEdit->toPlainText();

    // Get the selected index from the table view
    QModelIndex selectedIndex = ui->tableView->currentIndex();

    // Check if both fields are empty
    bool fieldsAreEmpty = name.isEmpty() && link.isEmpty();

    if (fieldsAreEmpty) {
        QMessageBox::warning(this, "Empty Fields", "Please enter data before clicking Add.");
        return;
    }

    // Check if the name already exists in the table, excluding the current record being updated.
    QSqlQuery checkQuery;
    if (selectedIndex.isValid()) {
        QString selectedId = ui->tableView->model()->data(ui->tableView->model()->index(selectedIndex.row(), 0)).toString();
        checkQuery.prepare("SELECT * FROM m WHERE name = :name AND id != :id");
        checkQuery.bindValue(":id", selectedId);
    } else {
        checkQuery.prepare("SELECT * FROM m WHERE name = :name");
    }

    checkQuery.bindValue(":name", name);

    if (checkQuery.exec() && checkQuery.next()) {
        // Name already exists, show a warning
        QMessageBox::warning(this, "Duplicate Name", "A record with the same name already exists.");
        return;
    }


    if (userEnteredData) {
        // Data entered by the user - either update or insert
        if (selectedIndex.isValid()) {
            // Data entered into an existing text field - update the selected row
            QString selectedId = ui->tableView->model()->data(ui->tableView->model()->index(selectedIndex.row(), 0)).toString();

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE m SET name = :name, link = :link WHERE id = :id");
            updateQuery.bindValue(":name", name);
            updateQuery.bindValue(":link", link);
            updateQuery.bindValue(":id", selectedId);

            bool empty = name.isEmpty() || link.isEmpty();
            if (empty) {
                QMessageBox::warning(this, "Empty Fields", "Please enter data before in both fields.");
                return;
            }

            if (updateQuery.exec()) {
                QMessageBox::information(this, "Update Successful", "Record updated successfully!");
                ui->AtextEdit->clear();
                ui->LtextEdit->clear();
            } else {
                QMessageBox::warning(this, "Update Error", "Error updating record: " + updateQuery.lastError().text());
            }
        } else {
            // Data entered into initially empty fields - insert a new row
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO m (name, link) VALUES (:name, :link)");
            insertQuery.bindValue(":name", name);
            insertQuery.bindValue(":link", link);


            bool Empty = name.isEmpty() || link.isEmpty();
            if (Empty) {
                QMessageBox::warning(this, "Empty Fields", "Please enter data before in both fields.");
                return;
            }

            if (insertQuery.exec()) {
                QMessageBox::information(this, "Insert Successful", "Record inserted successfully!");
                ui->AtextEdit->clear();
                ui->LtextEdit->clear();
            } else {
                QMessageBox::warning(this, "Insert Error", "Error inserting record: " + insertQuery.lastError().text());
            }
        }
    }

    // Reset the flag
    userEnteredData = false;

    // Refresh the table view to reflect the changes immediately
    QSqlTableModel *tableModel = dynamic_cast<QSqlTableModel*>(ui->tableView->model());
    if (tableModel) {
        tableModel->select();
    }
     refreshComboBoxData();
}









void MainWindow::on_StartL_clicked() {
    // Check if the process is already running
    if (!isProcessRunning) {
        // Get the path to the external executable
        QString executablePath = "C:/Users/hp/dist/calco.exe";
        //"C:/Users/hp/Desktop/pyton_projects/dist/GPA.exe"
        // Start the external process
        externalProcess->start(executablePath);

        // Check if the process started successfully
        if (externalProcess->waitForStarted()) {
            QMessageBox::information(this, "Process Started", "External process started successfully!");

            // Update the state variable
            isProcessRunning = true;

            // Disable the Start button
            ui->StartL->setEnabled(false);
            // Enable the Stop button
            ui->StopB->setEnabled(true);
        } else {
            QMessageBox::warning(this, "Error", "Error starting external process: " + externalProcess->errorString());
        }
    } else {
        QMessageBox::information(this, "Process Running", "External process is already running.");
    }
}



void MainWindow::on_tableView_activated(const QModelIndex &index)
{

}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    // Check if the selected index is valid
    if (index.isValid()) {
        // Get the QSqlTableModel associated with the QTableView
        QSqlTableModel *tableModel = dynamic_cast<QSqlTableModel*>(ui->tableView->model());

        // Check if the table model is valid
        if (tableModel) {
            // Get the data from the selected row
            QString name = tableModel->data(tableModel->index(index.row(), tableModel->fieldIndex("name"))).toString();
            QString link = tableModel->data(tableModel->index(index.row(), tableModel->fieldIndex("link"))).toString();

            // Populate the text fields with the selected data
            ui->AtextEdit->setPlainText(name);
            ui->LtextEdit->setPlainText(link);
        }
    }
    userEnteredData = false;
     refreshComboBoxData();
}





void MainWindow::on_StopB_clicked() {
    // Check if the process is running
    if (isProcessRunning) {
        // Obtain the PID of the external process
        qint64 pid = externalProcess->processId();

        // Terminate the process by its PID
        if (QProcess::execute("taskkill", QStringList() << "/F" << "/PID" << QString::number(pid)) == 0) {
            // Process terminated successfully
            QMessageBox::information(this, "Process Stopped", "External process stopped successfully!");

            // Update the state variable
            isProcessRunning = false;

            // Enable the Start button
            ui->StartL->setEnabled(true);
            // Disable the Stop button
            ui->StopB->setEnabled(false);
        } else {
            // Error terminating process
            QMessageBox::warning(this, "Error", "Error stopping external process.");
        }
    } else {
        QMessageBox::information(this, "Process Not Running", "External process is not currently running.");
    }
}


