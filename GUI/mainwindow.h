#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ntru.h"
#include <string>

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
    void on_bt_select_source_clicked();

    void on_bt_select_encrypted_clicked();

    void on_bt_select_key_h_clicked();

    void on_bt_select_key_f_clicked();

    void on_bt_select_key_g_clicked();

    void on_bt_create_keys_clicked();

    void on_bt_encrypt_clicked();

    void on_bt_decrypt_clicked();

    void on_bt_show_source_clicked();

    void on_bt_show_encrypted_clicked();

    void on_bt_show_decrypted_clicked();

    void on_bt_show_key_h_clicked();

    void on_bt_show_key_f_clicked();

    void on_bt_show_key_g_clicked();

    void on_bt_select_decrypted_clicked();

private:
    Ui::MainWindow *ui;
    NTRU ntru = NTRU(257, 3, 256);
    std::string src_filename = "";
    std::string enc_filename = "";
    std::string dec_filename = "";
    std::string key_h_filename = "";
    std::string key_f_filename = "";
    std::string key_g_filename = "";
};
#endif // MAINWINDOW_H
