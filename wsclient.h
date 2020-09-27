#ifndef WSCLIENT_H
#define WSCLIENT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class WSClient; }
QT_END_NAMESPACE

class WSClient : public QMainWindow
{
    Q_OBJECT

public:
    WSClient(QWidget *parent = nullptr);
    ~WSClient();

private slots:
    void on_btnConnect_clicked();

    void on_btnSend_clicked();

    void modeDisconnect();

    void modeConnect();

private:
    Ui::WSClient *ui;
};
#endif // WSCLIENT_H
