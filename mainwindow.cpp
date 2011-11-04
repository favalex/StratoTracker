#include "mainwindow.h"

#include <QApplication>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QGraphicsView>
#include <QDockWidget>
#include <QMessageBox>
#include <QSettings>
#include <QScrollArea>
#include <QFileDialog>

#include "mapview.h"
#include "statuswidget.h"
#include "metadatawidget.h"
#include "directorymonitor.h"

#include <QDebug>

namespace
{
    QAction *createAction(QWidget *parent, const char *text, const char *tooltip, const QString &shortcut = QString())
    {
        QAction *action = new QAction(parent);
        action->setText(parent->tr(text));
        action->setStatusTip(parent->tr(tooltip));
        if (!shortcut.isEmpty())
            action->setShortcut(QKeySequence(shortcut));
        return action;
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    map_view = new MapView;
    setCentralWidget(map_view);

    // Sidebar
    sidebar = new QDockWidget(tr("Telemetry"));
    sidebar->setObjectName("sidebar");
    QStringList fields = QStringList() << tr("Time") << tr("Altitude") << tr("External Temp.") << tr("Pressure") << tr("Umidity") << tr("Internal Temp.") << tr("V Supply") << tr("Acceleration") << tr("HADARP");
    MetadataWidget *metadata_widget = new MetadataWidget(fields);
    connect(map_view, SIGNAL(markerClicked(QStringList)), metadata_widget, SLOT(setMetadata(QStringList)));
    QScrollArea *scroll_area = new QScrollArea;
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(metadata_widget);
    sidebar->setWidget(scroll_area);
    addDockWidget(Qt::RightDockWidgetArea, sidebar);

    // Bottom panel
    bottom_panel = new QDockWidget(tr("Status"));
    bottom_panel->setObjectName("bottompanel");
    StatusWidget *status_widget = new StatusWidget;
    bottom_panel->setWidget(status_widget);
    addDockWidget(Qt::BottomDockWidgetArea, bottom_panel);

    statusBar();

    toolbar = addToolBar(tr("View"));
    toolbar->setObjectName("toolbar");

    setupActions();
    setupMenus();
    setupToolbar();

    monitor = new DirectoryMonitor(this);
    connect(monitor, SIGNAL(telemethryReceived(QTime,qreal,qreal,QStringList)), map_view, SLOT(addData(QTime,qreal,qreal,QStringList)));
    connect(monitor, SIGNAL(statusReceived(QTime,QString)), status_widget, SLOT(addMessage(QTime,QString)));
}

void MainWindow::readSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/state").toByteArray());
    statusBar()->setVisible(settings.value("mainwindow/statusbar", true).toBool());
    viewStatusbar->setChecked(settings.value("mainwindow/statusbar", true).toBool());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("mainwindow/geometry", saveGeometry());
    settings.setValue("mainwindow/state", saveState());
    settings.setValue("mainwindow/statusbar", statusBar()->isVisible());

    QMainWindow::closeEvent(event);
}

void MainWindow::startWatch()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open a directory to watch"));

    monitor->setDirectory(path);
}

void MainWindow::showPreferencesDialog()
{
}

void MainWindow::showAboutDialog()
{
    QString text = QString("<b>%1 %2</b>\n").arg(qApp->applicationName()).arg(qApp->applicationVersion());
    QMessageBox::about(this, tr("About"), text);
}

void MainWindow::setupActions()
{
    // File
    fileWatch = createAction(this, "&Open directory to watch...", "Open directory to watch", "Ctrl+O");
    fileWatch->setIcon(QIcon::fromTheme("document-open"));
    connect(fileWatch, SIGNAL(triggered()), SLOT(startWatch()));

    fileQuit = createAction(this, "&Quit", "Quit the application", "Ctrl+Q");
    fileQuit->setIcon(QIcon::fromTheme("application-exit"));
    connect(fileQuit, SIGNAL(triggered()), SLOT(close()));

    // View
    viewToolbar = createAction(this, "Toolbar", "Toggle the toolbar");
    viewToolbar->setCheckable(true);
    connect(viewToolbar, SIGNAL(toggled(bool)), toolbar, SLOT(setVisible(bool)));
    connect(toolbar, SIGNAL(visibilityChanged(bool)), viewToolbar, SLOT(setChecked(bool)));

    viewStatusbar = createAction(this, "Statusbar", "Toggle the statusbar");
    viewStatusbar->setCheckable(true);
    viewStatusbar->setChecked(statusBar()->isVisible());
    connect(viewStatusbar, SIGNAL(toggled(bool)), statusBar(), SLOT(setVisible(bool)));

    viewSidebar = createAction(this, "Telemetry", "Toggle the telemetry", "F9");
    viewSidebar->setCheckable(true);
    connect(viewSidebar, SIGNAL(toggled(bool)), sidebar, SLOT(setVisible(bool)));
    connect(sidebar, SIGNAL(visibilityChanged(bool)), viewSidebar, SLOT(setChecked(bool)));

    viewBottomPanel = createAction(this, "Status panel", "Toggle the status panel");
    viewBottomPanel->setCheckable(true);
    connect(viewBottomPanel, SIGNAL(toggled(bool)), bottom_panel, SLOT(setVisible(bool)));
    connect(bottom_panel, SIGNAL(visibilityChanged(bool)), viewBottomPanel, SLOT(setChecked(bool)));

    viewZoomIn = createAction(this, "Zoom In", "Zoom in the map", "Ctrl++");
    viewZoomIn->setIcon(QIcon::fromTheme("zoom-in"));
    connect(viewZoomIn, SIGNAL(triggered()), map_view, SLOT(zoomIn()));

    viewZoomOut = createAction(this, "Zoom Out", "Zoom out the map", "Ctrl+-");
    viewZoomOut->setIcon(QIcon::fromTheme("zoom-out"));
    connect(viewZoomOut, SIGNAL(triggered()), map_view, SLOT(zoomOut()));

    viewPreferences = createAction(this, "Preferences...", "Show the preferences dialog");
    connect(viewPreferences, SIGNAL(triggered()), SLOT(showPreferencesDialog()));

    // Help
    helpAbout = createAction(this, "About...", "About this application");
    connect(helpAbout, SIGNAL(triggered()), SLOT(showAboutDialog()));
}

void MainWindow::setupMenus()
{
    QMenuBar *menu_bar = menuBar();

    // File
    QMenu *file_menu = menu_bar->addMenu(tr("&File"));
    file_menu->insertAction(0, fileWatch);
    file_menu->insertAction(0, fileQuit);

    // View
    QMenu *view_menu = menu_bar->addMenu(tr("&View"));
    view_menu->insertAction(0, viewToolbar);
    view_menu->insertAction(0, viewStatusbar);
    view_menu->insertAction(0, viewSidebar);
    view_menu->insertAction(0, viewBottomPanel);
    view_menu->addSeparator();
    view_menu->insertAction(0, viewZoomIn);
    view_menu->insertAction(0, viewZoomOut);
    view_menu->addSeparator();
    view_menu->insertAction(0, viewPreferences);

    // Help
    QMenu *help_menu = menu_bar->addMenu(tr("&Help"));
    help_menu->insertAction(0, helpAbout);
}

void MainWindow::setupToolbar()
{
    toolbar->insertAction(0, fileWatch);
    toolbar->insertSeparator(viewZoomIn);
    toolbar->insertAction(0, viewZoomIn);
    toolbar->insertAction(0, viewZoomOut);
}
