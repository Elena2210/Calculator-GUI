#include "calcwgt.h"
#include "ui_calcwgt.h"
//#include <QDebug>
#include <QPushButton>

//=====================================================================
CalcWgt::CalcWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalcWgt)
{
    ui->setupUi(this);

    ui->btn_powerTwo->setText("x\302\262");
    ui->btn_multiplication->setText("\303\227");
    ui->btn_division->setText("\303\267");

    sumInMemory = 0.0;
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    waitingForOperand = true;

    ui->display->setReadOnly(true);
    ui->display->setMaxLength(10);
    ui->display->setText("0");

    QFont font = ui->display->font();
    font.setPointSize(font.pointSize()+8);
    ui->display->setFont(font);

    ui->btn_clear->setStyleSheet("QPushButton { background-color: lightgreen;}");
    ui->btn_clear_All->setStyleSheet("QPushButton { background-color: lightgreen;}");
    ui->btn_backspace->setStyleSheet("QPushButton { background-color: lightgreen;}");

    ui->btn_MC->setStyleSheet("QPushButton { background-color: lightgrey;}");
    ui->btn_MS->setStyleSheet("QPushButton { background-color: lightgrey;}");
    ui->btn_MR->setStyleSheet("QPushButton { background-color: lightgrey;}");
    ui->btn_M_ADD->setStyleSheet("QPushButton { background-color: lightgrey;}");

    ui->btn_equal->setStyleSheet("QPushButton { background-color: lightgrey;}");

    // Вектор ЦИФР
    numbers.append(ui->btn_0);
    numbers.append(ui->btn_1);
    numbers.append(ui->btn_2);
    numbers.append(ui->btn_3);
    numbers.append(ui->btn_4);
    numbers.append(ui->btn_5);
    numbers.append(ui->btn_6);
    numbers.append(ui->btn_7);
    numbers.append(ui->btn_8);
    numbers.append(ui->btn_9);

    for(int i = 0; i < 10; i++)
    {
        numbers[i]->setStyleSheet("QPushButton { background-color: lightblue;}");
        connect(numbers[i], &QPushButton::clicked ,
                [this, i]()
        {
            this->slotPushNumBtn(i);
        } );
    }
}
//=====================================================================
CalcWgt::~CalcWgt()
{
    delete ui;
}
//=====================================================================
bool CalcWgt::calc(double rightOper, const QString &pendingOperation)
{
    if(pendingOperation == tr("+"))
    {
        sumSoFar += rightOper;
    }
    else if(pendingOperation == tr("-"))
    {
        sumSoFar -= rightOper;
    }
    else if (pendingOperation == tr("\303\227"))
    {
        factorSoFar *=rightOper;
    }
    else if(pendingOperation == tr("\303\267"))
    {
        if(rightOper == 0.0)
            return false;
        factorSoFar /= rightOper;
    }

    return true;
}
//=====================================================================
// " . "
void CalcWgt::on_btn_point_clicked()
{
    if(waitingForOperand)
        ui->display->setText("0");
    if(!ui->display->text().contains('.'))
        ui->display->setText(ui->display->text() + tr("."));
    waitingForOperand = false;
}
//=====================================================================
// " BACKSPACE "
void CalcWgt::on_btn_backspace_clicked()
{
    if(waitingForOperand)
        return;

    QString text = ui->display->text();
    text.chop(1);
    if(text.isEmpty()){
        text = "0";
        waitingForOperand = true;
    }
    ui->display->setText(text);
}
//=====================================================================
// " CLEAR "
void CalcWgt::on_btn_clear_clicked()
{
    if(waitingForOperand)
        return;

    ui->display->setText("0");
    waitingForOperand = true;
}
//=====================================================================
// " CLEAR ALL "
void CalcWgt::on_btn_clear_All_clicked()
{
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAddOperator.clear();
    pendingMultiplicativeOperator.clear();

    ui->display->setText("0");
    waitingForOperand = true;
}
//=====================================================================
// " CLEAR Memory "
void CalcWgt::on_btn_MC_clicked()
{
    sumInMemory = 0.0;
}
//=====================================================================
// " READ Memory "
void CalcWgt::on_btn_MR_clicked()
{
    ui->display->setText(QString::number(sumInMemory));
    waitingForOperand = true;
}
//=====================================================================
// " SET Memory "
void CalcWgt::on_btn_MS_clicked()
{
    on_btn_equal_clicked();
    sumInMemory = ui->display->text().toDouble();
}
//=====================================================================
// " ADD to Memory "
void CalcWgt::on_btn_M_ADD_clicked()
{
    on_btn_equal_clicked();
    sumInMemory += ui->display->text().toDouble();
}
//=====================================================================
// " + "
void CalcWgt::on_btn_plus_clicked()
{
    QString clickedOper = ui->btn_plus->text();
    double operand = ui->display->text().toDouble();

    if(!pendingMultiplicativeOperator.isEmpty()){
        if(!calc(operand, pendingMultiplicativeOperator)){
            abortOperation();
            return;
        }
        ui->display->setText(QString::number(factorSoFar));
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }

    if(!pendingAddOperator.isEmpty()){
        if(!calc(operand, pendingAddOperator)){
            abortOperation();
            return;
        }
        ui->display->setText(QString::number(sumSoFar));
    } else {
        sumSoFar = operand;
    }

    pendingAddOperator = clickedOper;
    waitingForOperand = true;
}
//=====================================================================
// " - "
void CalcWgt::on_btn_minus_clicked()
{
    QString clickedOper = ui->btn_minus->text();
    double operand = ui->display->text().toDouble();

    if(!pendingMultiplicativeOperator.isEmpty()){
        if(!calc(operand, pendingMultiplicativeOperator)){
            abortOperation();
            return;
        }
        ui->display->setText(QString::number(factorSoFar));
        operand = factorSoFar;
        pendingMultiplicativeOperator.clear();
    }
    if(!pendingAddOperator.isEmpty()){
        if(!calc(operand, pendingAddOperator)){
            abortOperation();
            return;
        }
        ui->display->setText(QString::number(sumSoFar));
    } else {
        sumSoFar = operand;
    }

    pendingAddOperator = clickedOper;
    waitingForOperand = true;
}
//=====================================================================
// " * "
void CalcWgt::on_btn_multiplication_clicked()
{
    QString clickedOper = ui->btn_multiplication->text();
    double operand = ui->display->text().toDouble();

    if(!pendingMultiplicativeOperator.isEmpty()){
        if(!calc(operand, pendingMultiplicativeOperator)){
            abortOperation();
            return;
        }
        ui->display->setText(QString::number(factorSoFar));
    } else {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOper;
    waitingForOperand = true;
}
//=====================================================================
// " = "
void CalcWgt::on_btn_equal_clicked()
{
    double operand = ui->display->text().toDouble();

    if(!pendingMultiplicativeOperator.isEmpty()){
        if(!calc(operand, pendingMultiplicativeOperator)){
            abortOperation();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }
    if(!pendingAddOperator.isEmpty()){
        if(!calc(operand, pendingAddOperator)){
            abortOperation();
            return;
        }
        pendingAddOperator.clear();
    } else {
        sumSoFar = operand;
    }

    ui->display->setText(QString::number(sumSoFar));
    sumSoFar = 0.0;
    waitingForOperand = true;
}
//=====================================================================
// " / "
void CalcWgt::on_btn_division_clicked()
{
    QString clickedOper = ui->btn_division->text();
    double operand = ui->display->text().toDouble();

    if(!pendingMultiplicativeOperator.isEmpty()){
        if(!calc(operand, pendingMultiplicativeOperator)){
            abortOperation();
            return;
        }
        ui->display->setText(QString::number(factorSoFar));
    } else {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOper;
    waitingForOperand = true;
}
//=====================================================================
// " sqrt "
void CalcWgt::on_btn_square_clicked()
{
    QPushButton *cliBtn = ui->btn_square;
    QString cliOper = cliBtn->text();
    double operand = ui->display->text().toDouble();
    double result = 0.0;

    if(cliOper == tr("Sqrt")){
        if(operand < 0.0){
            abortOperation();
            return;
        }
        result = std::sqrt(operand);
    }

    ui->display->setText(QString::number(result));
    waitingForOperand = true;
}
//=====================================================================
// " x^2 "
void CalcWgt::on_btn_powerTwo_clicked()
{
    QPushButton *cliBtn = ui->btn_powerTwo;
    QString cliOper = cliBtn->text();
    double operand = ui->display->text().toDouble();
    double result = 0.0;

    if (cliOper == tr("x\302\262")){
        result = std::pow(operand, 2.0);
    }

    ui->display->setText(QString::number(result));
    waitingForOperand = true;
}
//=====================================================================
// " 1/x "
void CalcWgt::on_btn_reciprocal_clicked()
{
    QPushButton *cliBtn = ui->btn_reciprocal;
    QString cliOper = cliBtn->text();
    double operand = ui->display->text().toDouble();
    double result = 0.0;

     if(cliOper == tr("1/x")){
        if(operand == 0.0){
            abortOperation();
            return;
        }
        result = 1.0 / operand;
    }

    ui->display->setText(QString::number(result));
    waitingForOperand = true;
}
//=====================================================================
// Обработка нажатия цифр
void CalcWgt::slotPushNumBtn(int num)
{
    int digitValue = numbers[num]->text().toInt();
    if (ui->display->text() == "0" && digitValue == 0.0)
        return;
    if(waitingForOperand){
        ui->display->clear();
        waitingForOperand = false;
    }

    ui->display->setText(ui->display->text() +
                         QString::number(digitValue));
}
//=====================================================================
// Функция очистки
void CalcWgt::abortOperation()
{
    on_btn_clear_All_clicked();
    ui->display->setText(tr("####"));
}
//=====================================================================
// "+/-"
void CalcWgt::on_btn_plus_minus_clicked()
{
    QString text = ui->display->text();
    double value = text.toDouble();

    if(value > 0.0){
        text.prepend(tr("-"));
    } else if (value < 0.0){
        text.remove(0,1);
    }
    ui->display->setText(text);
}
//=====================================================================
