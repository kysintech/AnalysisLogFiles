#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QVariant>
#include <windows.h>
#include <ActiveQt/qaxobject.h>
#include <QtXml/QDomDocument>
#include <QtSql/QSqlDatabase>
#include <QtSql/qtsql-config.h>
#include <QtSql/qsqlquery.h>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Widget *ui;
    bool parseLog(QString filename, QString &nlogfilename);
    QList<QVariant> getlinedata;
    QList<QList<QVariant>> datasave;
    QList<QList<QVariant>> originallogfile;
    QList<QList<QVariant>> comparedlogfile;
    QAxObject * excelop;
    QString logfilepath;
    QString originallogfilepath;
    QString comparedlogfilepath;
    QString logfilename;
    QString originallogfilename;
    QString comparedlogfilename;
    void setgetlinedatanull();
    void castListListVariant2Variant(const QList<QList<QVariant> > &cells, QVariant &res);
    bool process_flag;
    QSqlDatabase dbcon;
    QSqlQuery dbquery;
};

#endif // WIDGET_H
