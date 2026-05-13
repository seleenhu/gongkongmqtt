#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include "ui_setting.h"

class Setting : public QWidget
{
    Q_OBJECT
public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

//private:
public:
     Ui::setting *ui;  // 关键：绑定UI
signals:

};

#endif // SETTING_H
