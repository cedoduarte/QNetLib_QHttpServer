#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class QHttpServer;
class QNetAccessManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
private:
    void init();
    void connectSlots();
    void defineRoutes();
    void startServer();
    void doRequests();

    Ui::MainWindow *ui;
    QHttpServer *m_httpServer;
    QNetAccessManager *m_manager;
};

#endif // MAINWINDOW_H
