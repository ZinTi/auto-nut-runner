#include "main_window.h"
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include "read_config.h"

// 检查配置文件是否存在，返回详细路径信息
bool check_config_file_exists(const QString& file_path, QString& absolute_path) {
    QFileInfo file_info(file_path);
    absolute_path = file_info.absoluteFilePath();
    return file_info.exists();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QString config_file = "config.ini";
    QString absolute_path;

    // 检查配置文件是否存在
    if (!check_config_file_exists(config_file, absolute_path)) {
        QMessageBox::critical(nullptr, "配置文件错误",
                            QString("配置文件丢失！\n\n"
                                   "文件名: %1\n"
                                   "期望路径: %2\n"
                                   "当前工作目录: %3\n\n"
                                   "请确保配置文件存在，然后重新启动程序。")
                                .arg(config_file)
                                .arg(absolute_path)
                                .arg(QDir::currentPath()),
                            QMessageBox::Ok);
        return -1;
    }

    try {
        ReadConfig& config = ReadConfig::getInstance();
        config.read_config_file(config_file);

        MainWindow w;
        w.setMinimumSize(600, 400);
        w.setWindowTitle("自动拧螺丝设备-调试配置助手（v0.0.1-@曾来）");
        w.show();

        return a.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "配置读取错误", QString("读取配置文件时发生错误：\n%1\n\n程序将退出。").arg(e.what()),QMessageBox::Ok);
        return -1;
    }
}
