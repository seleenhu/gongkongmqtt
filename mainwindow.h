#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "setting.h"
#include "QtMqtt/qmqttclient.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();
    void addLog(const QString &text);
    void onConnected();
    void onMessageReceived(const QByteArray &msg, const QMqttTopicName &topic);



    void on_actionlot_Set_triggered();

private:

    QMqttClient *mqttClient;
    Ui::MainWindow *ui;
    Setting *settingsPage;
};
#endif // MAINWINDOW_H
