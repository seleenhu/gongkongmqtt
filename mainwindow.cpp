#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setting.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //this->setWindowTitle("Qt 5.14.2 MQTT 订阅客户端");
    ui->setupUi(this);


   // ✅ 在这里创建，不会空指针       //add by seleen 04 29
    settingsPage =new Setting(nullptr);
   // ========== 安全创建 ==========
   mqttClient = new QMqttClient(this);

   connect(mqttClient, &QMqttClient::connected, this, &MainWindow::onConnected);
   connect(mqttClient, &QMqttClient::messageReceived, this, &MainWindow::onMessageReceived);

   // 默认值
   ui->EditHost->setText("test.mosquitto.org");
   ui->EditPort->setText("1883");
   ui->EditTopic->setText("test/qt514/mqtt");



   statusBar()->showMessage("就绪");
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnConnect_clicked()
{
    QString host = ui->EditHost->text().trimmed();
    int port = ui->EditPort->text().toInt();
    QString clientId = ui->EditClientID->text().trimmed();
    QString name = ui->EditName->text().trimmed();
    //QString name = settingsPage->ui->Set_EditName->text().trimmed();
    QString password = ui->EditPassword->text().trimmed();

     mqttClient->setHostname(host);
     mqttClient->setPort(port);
     mqttClient->setClientId(clientId);
     mqttClient->setUsername(name);
     mqttClient->setPassword(password);


     addLog("正在连接:" + host + ":" + QString::number(port));
     mqttClient->connectToHost();

}


void MainWindow::addLog(const QString &text)
{
    ui->EditMessage->append(text);
}


// 连接成功 → 订阅主题
void MainWindow::onConnected()
{
    ui->EditMessage->append("✅ 连接成功！");

    QString topic = ui->EditTopic->text().trimmed();
    mqttClient->subscribe(topic);

    ui->EditMessage->append("📩 已订阅主题：" + topic);
}

// 收到消息 → 显示在窗口
void MainWindow::onMessageReceived(const QByteArray &msg, const QMqttTopicName &topic)
{
#if 1
    ui->EditMessage->append("\n=================================");
    ui->EditMessage->append("主题：" + topic.name());
    ui->EditMessage->append("内容：" + msg);
    ui->EditMessage->append("=================================\n");
#endif


        qDebug() << "\n=== 收到 MQTT 消息 ===";
        qDebug() << "主题：" << topic.name();
        qDebug() << "原始数据：" <<msg;

        // 2. 解析 JSON（带异常保护）

//        {
//            "device_id": "QT_MQTT_001",
//            "temperature": 25.6,
//            "humidity": 60,
//            "status": true,
//            "location": {
//        s        "area": "workshop"
//            },
//            "data_list": [10, 20, 30]
//        }

         QJsonParseError parseError;
         QJsonDocument jsonDoc = QJsonDocument::fromJson(msg, &parseError); //这里的msg QByteArray 就是mqtt 的payload


         // JSON 格式错误
          if (parseError.error != QJsonParseError::NoError) {
              qDebug() << "JSON 解析失败：" << parseError.errorString();
              return;
           }

          // 3. 转为 JSON 对象
              QJsonObject jsonObj = jsonDoc.object();

              // ==============================================
              // 4. 在这里读取你需要的字段（示例）
              // ==============================================
              // 读取普通字符串/数字
              QString deviceId = jsonObj["device_id"].toString();
              double temperature = jsonObj["temperature"].toDouble();
              int humidity = jsonObj["humidity"].toInt();
              bool status = jsonObj["status"].toBool();

              // 读取嵌套 JSON（如果有）
              QJsonObject locationObj = jsonObj["location"].toObject();
              QString area = locationObj["area"].toString();

              // 读取 JSON 数组（如果有）
              QJsonArray dataArray = jsonObj["data_list"].toArray();

              // 打印解析结果
              qDebug() << "设备ID：" << deviceId;
              qDebug() << "温度：" << temperature;
              qDebug() << "湿度：" << humidity;
              qDebug() << "状态：" << status;
              qDebug() << "区域：" << area;
              qDebug() << "数组长度：" << dataArray.size();

              ui->lcdNumberTemp->display(temperature);
              ui->lcdNumberHdmi->display(humidity);







}


void MainWindow::on_actionlot_Set_triggered()
{
    settingsPage->show(); // 打开你设计的界面
}
