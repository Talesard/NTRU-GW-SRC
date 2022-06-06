#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>

std::string read_file_to_string(std::string filename) {
    std::ifstream t(filename);
    std::string str;
    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
}

void write_string_to_file(std::string str, std::string filename) {
    std::ofstream fout(filename, std::ios_base::out | std::ios_base::trunc);
    if (!fout.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    fout << str;
    fout.close();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_bt_select_source_clicked()
{
    src_filename = QFileDialog::getOpenFileName(this, tr("Source file")).toStdString();
    std::cout << src_filename << std::endl;
}


void MainWindow::on_bt_select_encrypted_clicked()
{
    enc_filename = QFileDialog::getOpenFileName(this, tr("Encrypted file")).toStdString();
    std::cout << enc_filename << std::endl;
}

void MainWindow::on_bt_select_decrypted_clicked()
{
    dec_filename = QFileDialog::getOpenFileName(this, tr("Decrypted file")).toStdString();
    std::cout << dec_filename << std::endl;
}

void MainWindow::on_bt_select_key_h_clicked()
{
    key_h_filename = QFileDialog::getOpenFileName(this, tr("Public Key h file")).toStdString();
    std::cout << key_h_filename << std::endl;
}


void MainWindow::on_bt_select_key_f_clicked()
{
    key_f_filename = QFileDialog::getOpenFileName(this, tr("Private Key f file")).toStdString();
    std::cout << key_f_filename << std::endl;
}


void MainWindow::on_bt_select_key_g_clicked()
{
    key_g_filename = QFileDialog::getOpenFileName(this, tr("Private Key g file")).toStdString();
    std::cout << key_g_filename << std::endl;
}


void MainWindow::on_bt_create_keys_clicked()
{
    ntru.create_private_keys();
    ntru.create_public_key();
    ntru.save_private_f_to_file(key_f_filename.c_str());
    ntru.save_private_g_to_file(key_g_filename.c_str());
    ntru.save_public_h_to_file(key_h_filename.c_str());
}


void MainWindow::on_bt_encrypt_clicked()
{
    ntru.load_public_h_from_file(key_h_filename.c_str());
    ntru.load_private_f_from_file(key_f_filename.c_str()); // !
    ntru.load_private_g_from_file(key_g_filename.c_str()); // !
    std::string src_message = read_file_to_string(src_filename);
    std::vector<NTL::ZZ_pX> encrypted_blocks = ntru.blocked_encrypt_str(src_message, true); // !
    ntru.blocked_save_encrypted_to_file(encrypted_blocks, enc_filename.c_str());
}


void MainWindow::on_bt_decrypt_clicked()
{
    std::vector<NTL::ZZ_pX> encrypted_blocks;
    encrypted_blocks = ntru.blocked_load_encrypted_from_file(enc_filename.c_str());
    ntru.load_private_f_from_file(key_f_filename.c_str());
    ntru.load_private_g_from_file(key_g_filename.c_str());
    std::string decr_message = ntru.blocked_decrypt_str(encrypted_blocks);
    write_string_to_file(decr_message, dec_filename);
}


void MainWindow::on_bt_show_source_clicked()
{
    std::string src = read_file_to_string(src_filename);
    ui->plainTextEdit->setPlainText(QString::fromStdString(src));
}


void MainWindow::on_bt_show_encrypted_clicked()
{
    std::string enc = read_file_to_string(enc_filename);
    ui->plainTextEdit->setPlainText(QString::fromStdString(enc));
}


void MainWindow::on_bt_show_decrypted_clicked()
{
    std::string dec = read_file_to_string(dec_filename);
    ui->plainTextEdit->setPlainText(QString::fromStdString(dec));
}


void MainWindow::on_bt_show_key_h_clicked()
{
    std::string key_h = read_file_to_string(key_h_filename);
    ui->plainTextEdit->setPlainText(QString::fromStdString(key_h));
}


void MainWindow::on_bt_show_key_f_clicked()
{
    std::string key_f = read_file_to_string(key_f_filename);
    ui->plainTextEdit->setPlainText(QString::fromStdString(key_f));
}


void MainWindow::on_bt_show_key_g_clicked()
{
    std::string key_g = read_file_to_string(key_g_filename);
    ui->plainTextEdit->setPlainText(QString::fromStdString(key_g));
}

