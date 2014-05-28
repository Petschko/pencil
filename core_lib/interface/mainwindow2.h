#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QDomElement>
#include <QMainWindow>

template<typename T> class QList;
class QActionGroup;
class Object;
class Editor;
class ScribbleArea;
class BaseDockWidget;
class ColorPaletteWidget;
class DisplayOptionDockWidget;
class ToolOptionWidget;
class TimeLine;
class ToolBoxWidget;
class Preferences;
class RecentFileMenu;


namespace Ui
{
class MainWindow2;
}



class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget* parent = 0);
    ~MainWindow2();

    // Data Model
    Object* m_pObject;

    // Core controller
    Editor* m_pEditor;

protected:
    void tabletEvent(QTabletEvent* event) override;
    RecentFileMenu* m_recentFileMenu;

public slots:
    void setOpacity(int opacity);
    void undoActSetText(void);
    void undoActSetEnabled(void);
    void newDocument();
    void openDocument();
    bool saveAsNewDocument();
    void saveDocument();
    bool maybeSave();
    void showPreferences();
    bool openObject(QString strFilename);
    void resetToolsSettings();
    void openFile(QString filename);

private slots:
    bool saveObject(QString strSavedFilename);
    void dockAllPalettes();
    void helpBox();
    void aboutPencil();

    void loadAllShortcuts();
    void unloadAllShortcuts();

    void importPalette();
    void exportPalette();

    // XML save/load
    QDomElement createDomElement(QDomDocument& doc);
    bool loadDomElement(QDomElement docElem, QString filePath);

private:
    void createDockWidgets();
    void createMenus();

    void makeColorWheelConnections();
    

    void closeEvent(QCloseEvent*);

    void readSettings();
    void writeSettings();
    void makeConnections( Editor*, ScribbleArea* );
    void makeConnections( Editor*, ColorPaletteWidget* );
    void makeConnections( Editor*, TimeLine* );
    void makeConnections( Editor*, DisplayOptionDockWidget* );

    // UI: central Drawing Area
    ScribbleArea* m_pScribbleArea;

    // UI: Dock widgets
    QDockWidget*             m_pColorWheelWidget;
    ColorPaletteWidget*      m_pColorPalette;
    DisplayOptionDockWidget* m_pDisplayOptionWidget;
    ToolOptionWidget*        m_pToolOptionWidget;
    ToolBoxWidget*           m_pToolBox;

public:
    TimeLine*                m_pTimeLine; // be public temporary

private:
    // Dialogs
    Preferences* m_pPreferences;

    Ui::MainWindow2* ui;
    QList< BaseDockWidget* > m_subWidgets;
};

#endif // MAINWINDOW2_H