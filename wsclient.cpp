#define BOOST_ALL_NO_LIB

#include "wsclient.h"
#include "ui_wsclient.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <QDebug>


namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
using namespace std;

net::io_context ioc;
tcp::resolver resolver{ioc};
websocket::stream<tcp::socket> ws{ioc};

WSClient::WSClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WSClient)
{
    ui->setupUi(this);
}

WSClient::~WSClient()
{
    delete ui;
}


void WSClient::on_btnConnect_clicked()
{
    try {
        if (ui->btnConnect->text() == "Connect") {
            // connect ke server
            modeConnect();

            // cari tanda ":" dulu pada txtAddress
            string address = ui->txtAddress->text().toStdString();
            size_t pos= address.find(":");

            string ipAddress;
            string port;

            if (pos > 500) {
                // tanda ":" tidak ditemukan, maka gunakan port 80
                ipAddress = address;
                port = "80";
            } else {
                // tanda ":" ditemukan, maka gunakan IP Address:Port atau URL:Port
                ipAddress = address.substr(0,pos);
                port= address.substr(pos+1,address.length());
            }

            /*
             * format protocol websocket:
             * ws://ip_address:port (protocol biasa)
             * wss://ip_address:port (protocol secure)
            */
            string host = "ws://" + ipAddress + ":"+port;

            auto const result =resolver.resolve(ipAddress, port);

            net::connect(::ws.next_layer(), result.begin(),result.end());

            ::ws.handshake(host, "/");
        } else {
            // disconnect dari server
            ::ws.close(websocket::close_code::normal);

            modeDisconnect();
        }


    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}

void WSClient::on_btnSend_clicked()
{
    string message = ui->txtMessage->text().toStdString();
    ::ws.write(net::buffer(message));

    ui->txtLog->append(QString::fromStdString("<b>Sent:</b> " + message));
    ui->txtMessage->clear();
    ui->txtMessage->setFocus();
}

void WSClient::modeDisconnect() {
    // dijalankan pada saat disconnect
    ui->btnConnect->setText("Connect");
    ui->txtAddress->setEnabled(true);

    ui->txtMessage->setEnabled(false);
    ui->btnSend->setEnabled(false);
}


void WSClient::modeConnect() {
    // dijalankan pada saat connect
    ui->btnConnect->setText("Disconnect");
    ui->txtAddress->setEnabled(false);

    ui->txtMessage->setEnabled(true);
    ui->btnSend->setEnabled(true);
}
