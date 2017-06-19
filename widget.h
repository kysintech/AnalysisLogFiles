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

private:
    Ui::Widget *ui;
    bool parseLog(QString filename);
    QList<QVariant> getlinedata;
    QList<QList<QVariant>> datasave;
    QAxObject * excelop;
    QString logfilepath;
    QString logfilename;
    void setgetlinedatanull();
    void castListListVariant2Variant(const QList<QList<QVariant> > &cells, QVariant &res);
    bool process_flag;
};

#endif // WIDGET_H
