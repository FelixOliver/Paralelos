/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *parallel;
    QLineEdit *file_p;
    QPushButton *compilar;
    QLabel *image1;
    QLineEdit *threads_p;
    QLineEdit *file_s;
    QPushButton *sequencial;
    QLineEdit *threads_s;
    QFrame *line;
    QLabel *image2;
    QPushButton *compilar_seq;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1108, 715);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        parallel = new QPushButton(centralWidget);
        parallel->setObjectName(QStringLiteral("parallel"));
        parallel->setGeometry(QRect(20, 20, 181, 31));
        file_p = new QLineEdit(centralWidget);
        file_p->setObjectName(QStringLiteral("file_p"));
        file_p->setGeometry(QRect(100, 80, 151, 21));
        compilar = new QPushButton(centralWidget);
        compilar->setObjectName(QStringLiteral("compilar"));
        compilar->setGeometry(QRect(316, 30, 151, 51));
        image1 = new QLabel(centralWidget);
        image1->setObjectName(QStringLiteral("image1"));
        image1->setGeometry(QRect(40, 120, 481, 561));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(image1->sizePolicy().hasHeightForWidth());
        image1->setSizePolicy(sizePolicy);
        image1->setFocusPolicy(Qt::NoFocus);
        image1->setScaledContents(true);
        threads_p = new QLineEdit(centralWidget);
        threads_p->setObjectName(QStringLiteral("threads_p"));
        threads_p->setGeometry(QRect(20, 80, 71, 21));
        file_s = new QLineEdit(centralWidget);
        file_s->setObjectName(QStringLiteral("file_s"));
        file_s->setGeometry(QRect(730, 80, 161, 21));
        sequencial = new QPushButton(centralWidget);
        sequencial->setObjectName(QStringLiteral("sequencial"));
        sequencial->setGeometry(QRect(650, 20, 191, 31));
        threads_s = new QLineEdit(centralWidget);
        threads_s->setObjectName(QStringLiteral("threads_s"));
        threads_s->setGeometry(QRect(650, 80, 71, 21));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(570, 10, 16, 701));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        image2 = new QLabel(centralWidget);
        image2->setObjectName(QStringLiteral("image2"));
        image2->setGeometry(QRect(620, 120, 461, 561));
        image2->setScaledContents(true);
        compilar_seq = new QPushButton(centralWidget);
        compilar_seq->setObjectName(QStringLiteral("compilar_seq"));
        compilar_seq->setGeometry(QRect(940, 30, 131, 51));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        parallel->setText(QApplication::translate("MainWindow", "Coloring Graph Parallel ", 0));
        file_p->setText(QString());
        compilar->setText(QApplication::translate("MainWindow", "Compilar", 0));
        image1->setText(QString());
        sequencial->setText(QApplication::translate("MainWindow", "Coloring Graph Secuencial", 0));
        image2->setText(QString());
        compilar_seq->setText(QApplication::translate("MainWindow", "Compilar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
