#include "calcwgt.h"
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalcWgt w;
    w.setWindowTitle( " Калькулятор 1.0 " );
    QApplication::setOrganizationName("Lentayka");
    QApplication::setApplicationName("Calculator");
    w.show();

    // Расположение окна
    w.setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    w.size(),
                    qApp->desktop()->availableGeometry()
                    )
                );

    return a.exec();
}
