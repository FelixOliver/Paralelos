#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_compilar_clicked()
{
    system("mpic++ coloring_graph.cpp -o coloring_parallel");
}

void MainWindow::on_parallel_clicked()
{
    string thread = this->ui->threads_p->text().toStdString();
    string name_file = this->ui->file_p->text().toStdString();
    string command = "mpirun -np "+thread+" ./coloring_parallel "+name_file;
    system(command.c_str());

    string file_img = "grafo.png";
    QImage imageObject;
    imageObject.load(QString::fromStdString(file_img));


    ui->image1->setPixmap(QPixmap::fromImage(imageObject));
}

void MainWindow::on_compilar_seq_clicked()
{
    system("mpic++ coloring_serial.cpp -o coloring_secuencial");
}

void MainWindow::on_sequencial_clicked()
{
    string thread = this->ui->threads_s->text().toStdString();
    string name_file = this->ui->file_s->text().toStdString();
    string command = "mpirun -np "+thread+" ./coloring_secuencial "+name_file;
    system(command.c_str());

    string file_img= "grafo_serial.png";
    QImage imageObject;
    imageObject.load(QString::fromStdString(file_img));


    ui->image2->setPixmap(QPixmap::fromImage(imageObject));
}
