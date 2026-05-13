#include "setting.h"

Setting::Setting(QWidget *parent) : QWidget(parent)
{
    // 👇 崩溃就是因为你漏了这一行！！！
    ui = new Ui::setting;
    ui->setupUi(this);

}

// 析构函数
Setting::~Setting()
{
    delete ui;
}
