#include "mainwindow.h"
#include <appcontroller.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    AppController ac(&w);
    return a.exec();
}


/*
ToDo
- check target path for folders, that already exist there and skip them if we hit them in the randomization procedure for the source path
- allow updates of an existing list if parameters were changed
- save button to save current setup to default values in config file
- sanity check for source and target folders (disable prep copy list and copy files if it doesn't make sense)
- save last transfer lists and provide options to avoid duplicates in comparison to old lists
- add console like log
- add bottom status bar to display latest log entry
*/
