#ifndef CALCWGT_H
#define CALCWGT_H

#include <QWidget>
#include <QPushButton>

class QLineEdit;
class Button;

namespace Ui {
class CalcWgt;
}

class CalcWgt : public QWidget
{
    Q_OBJECT

public:
    explicit CalcWgt(QWidget *parent = nullptr);
    ~CalcWgt();

private slots:
    void on_btn_MC_clicked();
    void on_btn_MR_clicked();
    void on_btn_MS_clicked();
    void on_btn_M_ADD_clicked();

    void on_btn_backspace_clicked();
    void on_btn_clear_clicked();
    void on_btn_clear_All_clicked();

    void slotPushNumBtn(int num);
    void on_btn_plus_minus_clicked();
    void on_btn_point_clicked();

    void on_btn_plus_clicked();
    void on_btn_minus_clicked();
    void on_btn_multiplication_clicked();
    void on_btn_reciprocal_clicked();
    void on_btn_division_clicked();
    void on_btn_square_clicked();
    void on_btn_powerTwo_clicked();

    void on_btn_equal_clicked();

private:
    Ui::CalcWgt *ui;

    void abortOperation();
    bool calc(double rightOper, const QString &pendingOperation);

    double sumInMemory;
    double sumSoFar;
    double factorSoFar;
    bool waitingForOperand;

    QString pendingAddOperator;
    QString pendingMultiplicativeOperator;

    QVector< QPushButton * > numbers;
};

#endif // CALCWGT_H

