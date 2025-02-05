#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QTextEdit> // Include this if QTextEdit is used
#include <QProcess>
#include <QCoreApplication>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AddB_clicked();
    void on_StartL_clicked();
    void on_tableView_activated(const QModelIndex &index);
    void on_tableView_clicked(const QModelIndex &index);

    // Add the onDataChanged slot declaration
    void onDataChanged();

    void on_StopB_clicked();

private:
    Ui::MainWindow *ui;

    // Declare the userEnteredData flag as a private member variable
    bool userEnteredData;
    bool isProcessRunning;

    QSqlDatabase db;
    // Helper function to refresh data in the QComboBox
    void refreshComboBoxData();
    QProcess *externalProcess;
};

#endif // MAINWINDOW_H
