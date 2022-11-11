#include "mainwindow.h"
#include "qnetrequest.h"
#include "ui_mainwindow.h"
#include "qnetaccessmanager.h"
#include "qflexiblejsonobject.h"

#include <QDebug>
#include <QJsonArray>
#include <QHttpServer>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QHttpServerRequest>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    defineRoutes();
    connectSlots();
    startServer();
    doRequests();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    m_httpServer = new QHttpServer(this);
    m_manager = new QNetAccessManager(this);
}

void MainWindow::connectSlots()
{
    connect(m_manager, &QNetAccessManager::finished, this, [&](QNetworkReply *reply)
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            qDebug() << reply->readAll();
        }
    });
}

void MainWindow::defineRoutes()
{
    m_httpServer->route("/", [&]() -> QString
    {
        return "hello world";
    });

    m_httpServer->route("/name", [&]() -> QString
    {
        return "DuarteCorporation";
    });

    m_httpServer->route("/patchUsage", [&](const QHttpServerRequest &request) -> QString
    {
        if (request.method() == QHttpServerRequest::Method::Patch)
        {
            QJsonObject object = QJsonDocument::fromJson(request.body()).object();
            return QFlexibleJsonObject::value(object, "value").toString();
        }
        else
        {
            return "Error: Only PATCH method is allowed!";
        }
    });

    m_httpServer->route("/putUsage", [&](const QHttpServerRequest &request) -> QString
    {
        if (request.method() == QHttpServerRequest::Method::Put)
        {
            QJsonObject object = QJsonDocument::fromJson(request.body()).object();
            return QFlexibleJsonObject::value(object, "content").toString();
        }
        else
        {
            return "Error: Only PUT method is allowed!";
        }
    });

    m_httpServer->route("/create", [&](const QHttpServerRequest &request) -> QString
    {
        if (request.method() == QHttpServerRequest::Method::Post)
        {
            QJsonObject object = QJsonDocument::fromJson(request.body()).object();
            return QString::number(QFlexibleJsonObject::value(object, "value").toInt());
        }
        else
        {
            return "Error: Only POST method is allowed!";
        }
    });
}

void MainWindow::startServer()
{
    m_httpServer->listen(QHostAddress::Any, 1234);
}

void MainWindow::doRequests()
{
    m_manager->get(QNetRequest(QUrl("http://localhost:1234"))); // hello world
    m_manager->get(QNetRequest(QUrl("http://localhost:1234/name"))); // DuarteCorporation
    m_manager->patch(QNetRequest(QUrl("http://localhost:1234/patchUsage")), QString("{\"Value\":\"hello patch\"}").toLatin1());
    m_manager->put(QNetRequest(QUrl("http://localhost:1234/putUsage")), QString("{\"CONTENT\": \"hello put\"}").toLatin1());
    m_manager->post(QNetRequest(QUrl("http://localhost:1234/create")), QString("{\"Value\": 10}").toLatin1());
    m_manager->get(QNetRequest(QUrl("http://localhost:1234/create")));
}
