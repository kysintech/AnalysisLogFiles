#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    datasave.clear();
    getlinedata.clear();
    for(int m=0;m<5;m++)
    {
        getlinedata.append(" ");
    }

    process_flag = false;
    ui->progressBar->setValue(0);
    ui->label->setText("");
}

Widget::~Widget()
{
    delete ui;
     delete excelop;
}

bool Widget::parseLog(QString filename)
{
    process_flag=false;
    qDebug()<<filename;
    QStringList getfilename = filename.split('/');
    qDebug()<<getfilename;
    logfilename.clear();
    logfilename.append(getfilename.last());
    qDebug()<<logfilename;
    ui->lineEdit->setText(filename);
    logfilepath.clear();
    logfilepath.append(filename);

    QFile logfile(filename);
    if(!(logfile.open(QIODevice::ReadOnly|QIODevice::Text)))
    {
        qDebug()<<"Sorry , the log file open failed .";
        return false;
    }
    qDebug()<<"The Log file size is : "<<logfile.size();
    QString loggetline;
    long num_dut=0;
    long num_measure=0;
    long num_limit=0;
    datasave.clear();
    while(!logfile.atEnd())
    {
        loggetline = logfile.readLine();
        if(loggetline.indexOf("Dut DUT:",0,Qt::CaseSensitive)>0)
        {
            setgetlinedatanull();
            num_dut++;
           // qDebug()<<loggetline.mid(loggetline.indexOf("Dut DUT:",0,Qt::CaseSensitive)+8,loggetline.indexOf("\\tab ",0,Qt::CaseSensitive)-(loggetline.indexOf("Dut DUT:",0,Qt::CaseSensitive)+8));
            getlinedata[0]=(loggetline.mid(loggetline.indexOf("Dut DUT:",0,Qt::CaseSensitive)+8,loggetline.indexOf("\\tab ",0,Qt::CaseSensitive)-(loggetline.indexOf("Dut DUT:",0,Qt::CaseSensitive)+8)));

            // qDebug()<<loggetline.mid(loggetline.indexOf("\\tab ",0,Qt::CaseSensitive)+5,loggetline.indexOf("\\line ",0,Qt::CaseSensitive)-(loggetline.indexOf("\\tab ",0,Qt::CaseSensitive)+5));
            getlinedata[1]=(loggetline.mid(loggetline.indexOf("\\tab ",0,Qt::CaseSensitive)+5,loggetline.indexOf("\\line ",0,Qt::CaseSensitive)-(loggetline.indexOf("\\tab ",0,Qt::CaseSensitive)+5)));
            if(loggetline.indexOf("Measured:",0,Qt::CaseSensitive)>0)
            {
                num_measure++;
              //  qDebug()<<"Measured: "<<loggetline.mid(loggetline.indexOf("Measured:",0,Qt::CaseSensitive)+14,loggetline.indexOf("\\cf0\\tab",0,Qt::CaseSensitive)-(loggetline.indexOf("Measured:",0,Qt::CaseSensitive)+14));
                getlinedata[2]=loggetline.mid(loggetline.indexOf("Measured:",0,Qt::CaseSensitive)+14,loggetline.indexOf("\\cf0\\tab",0,Qt::CaseSensitive)-(loggetline.indexOf("Measured:",0,Qt::CaseSensitive)+14));
            }
            else
            {
                return false;
            }
            if(loggetline.indexOf("Limits:",0,Qt::CaseSensitive)>0)
            {
                num_limit++;
                //qDebug()<<loggetline.mid(loggetline.indexOf("Limits:",0,Qt::CaseSensitive)+7,loggetline.indexOf("\\tab\\cf",0,Qt::CaseSensitive)-(loggetline.indexOf("Limits:",0,Qt::CaseSensitive)+7));
                getlinedata[3]=loggetline.mid(loggetline.indexOf("Limits:",0,Qt::CaseSensitive)+7,loggetline.indexOf("\\tab\\cf",0,Qt::CaseSensitive)-(loggetline.indexOf("Limits:",0,Qt::CaseSensitive)+7));
            }
            else
            {
             //   qDebug()<<"No Limits!!!";
               //  return false;
            }
            if(loggetline.indexOf("Pass")>0)
            {
                getlinedata[4]="Pass";
            }
            if(loggetline.indexOf("Fail")>0)
            {
                getlinedata[4]="Fail";
            }

            datasave.append(getlinedata);
        }

    }
    qDebug()<<num_dut<<num_measure<<num_limit<<datasave.size();
    ui->message->setText("Please select the excel file save path! ");
    process_flag=true;
    return true;
}

void Widget::setgetlinedatanull()
{
    getlinedata.clear();
    for(int m=0;m<5;m++)
    {
        getlinedata.append(" ");
    }
}

void Widget::on_pushButton_clicked()
{
    ui->progressBar->setValue(0);
    ui->label->setText("");
    bool res = parseLog(QFileDialog::getOpenFileName(this,tr("Select the RTF Format Log file "),".",tr("RTF Files (*.rtf)")));
    if(!res)
    {
        qDebug()<<"the log file parse failed!!";
        return ;
    }

}

void Widget::on_pushButton_2_clicked()
{
    if(!process_flag)
    {
        qDebug()<<"the log file parsing is not finished!  Please Wait !";
        ui->message->setText("the log file parsing is not finished!  Please Wait !");
        return ;
    }
    ui->message->setText("The excel file is on the way...");
    QString xlsfile = QFileDialog::getExistingDirectory(this);
    if(xlsfile.isEmpty())
    {
        qDebug()<<"the file name is empty!!";
        ui->message->setText("the file name is empty!!");
        return ;
    }
    xlsfile.append("/");
    xlsfile.append(logfilename.left(logfilename.indexOf(".")));
    xlsfile.append(".xlsx");
    ui->lineEdit_2->setText(xlsfile);
    ui->progressBar->setValue(10);
    excelop = new QAxObject(this);
    excelop->setControl("Excel.Application");
    excelop->dynamicCall("SetVisible (bool Visible)","false");
    excelop->setProperty("DisplayAlerts",false);

    ui->progressBar->setValue(20);
    QAxObject * workbooks = excelop->querySubObject("WorkBooks");
    workbooks->dynamicCall("Add");
    QAxObject * workbook = excelop->querySubObject("ActiveWorkBook");
    QAxObject * worksheets = workbook->querySubObject("Sheets");
    QAxObject * worksheet = worksheets->querySubObject("Item(int)",1);
    worksheet->setProperty("Name","logresult");

    ui->progressBar->setValue(30);

    QAxObject * CellX = worksheet->querySubObject("Range(QVariant,QVariant)","B1");
    QAxObject * Interior = CellX->querySubObject("Interior");
    Interior->setProperty("Color",QColor(0,212,22));
    CellX->dynamicCall("SetValue(const QVariant&)","Index");

    CellX = worksheet->querySubObject("Range(QVariant,QVariant)","C1");
    CellX->dynamicCall("SetValue(const QVariant&)","MP");
    Interior = CellX->querySubObject("Interior");
    Interior->setProperty("Color",QColor(112,0,22));

    CellX = worksheet->querySubObject("Range(QVariant,QVariant)","D1");
    CellX->dynamicCall("SetValue(const QVariant&)","Value");
    Interior = CellX->querySubObject("Interior");
    Interior->setProperty("Color",QColor(0,212,22));

    CellX = worksheet->querySubObject("Range(QVariant,QVariant)","E1");
    CellX->dynamicCall("SetValue(const QVariant&)","Limits");
    Interior = CellX->querySubObject("Interior");
    Interior->setProperty("Color",QColor(123,120,202));

    CellX = worksheet->querySubObject("Range(QVariant,QVariant)","F1");
    CellX->dynamicCall("SetValue(const QVariant&)","Result");
    Interior = CellX->querySubObject("Interior");
    Interior->setProperty("Color",QColor(10,112,122));

    ui->progressBar->setValue(40);

    int rows = datasave.size();
    QString num_t;
    QString nums;

        nums.clear();
        num_t.clear();
        nums.setNum(rows+2,10);
        num_t.append("B2:F");
        num_t.append(nums);
        CellX = worksheet->querySubObject("Range(const QString&)",num_t);
        qDebug()<<num_t;
        QVariant res ;
        castListListVariant2Variant(datasave,res);
        CellX->setProperty("Value",res);

        ui->progressBar->setValue(60);
        if(ui->checkBox_2->isChecked())
        {
            for(int m=0;m<rows;m++)
            {
                nums.clear();
                num_t.clear();
                nums.setNum(m+2,10);
                num_t.append("F");
                num_t.append(nums);
                CellX = worksheet->querySubObject("Range(const QString&)",num_t);
                if(CellX->property("Value")=="Fail")
                {
                     Interior = CellX->querySubObject("Interior");
                     Interior->setProperty("Color",QColor(200,2,2));
                }
            }
        }

    ui->progressBar->setValue(80);

    workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(xlsfile));
    ui->progressBar->setValue(100);
    ui->message->setText(tr("Excel is ready! "));
    ui->label->setText(tr("完成"));

    if(ui->checkBox->isChecked())
    {
        excelop->dynamicCall("SetVisible (bool Visible)","true");
    }else
    {
         workbook->dynamicCall("Close()");
         excelop->dynamicCall("Quit()");
         delete excelop;
    }

}



void Widget::castListListVariant2Variant(const QList<QList<QVariant> > &cells, QVariant &res)
{
    QVariantList vars;
    const int rows = cells.size();
    for(int i=0;i<rows;++i)
    {
        vars.append(QVariant(cells[i]));
    }
    res = QVariant(vars);
    qDebug()<<vars;
}
