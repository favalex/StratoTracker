#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QToolbar;
class QDockWidget;
class MapView;
class DirectoryMonitor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    void readSettings();
    void closeEvent(QCloseEvent *event);

private slots:
    void startWatch();
    void showPreferencesDialog();
    void showAboutDialog();

private:
    void setupActions();
    void setupMenus();
    void setupToolbar();


    // File
    QAction *fileWatch;
    QAction *fileQuit;

    // View
    QAction *viewToolbar;
    QAction *viewStatusbar;
    QAction *viewSidebar;
    QAction *viewBottomPanel;
    QAction *viewZoomIn;
    QAction *viewZoomOut;
    QAction *viewPreferences;

    // Help
    QAction *helpAbout;

    QToolBar *toolbar;
    QDockWidget *sidebar;
    QDockWidget *bottom_panel;
    MapView *map_view;
    DirectoryMonitor *monitor;
};

#endif // MAINWINDOW_H
