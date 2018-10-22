#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QFileInfo>

int main( int argc, char *argv[] )
{
	QApplication a(argc, argv);
	MainWindow w;
	QFile self( argv[0] );

	self.setFileName( "combo.sqlite" );

	w.setup( QFileInfo( self ).absoluteFilePath() );
	w.show();

	int rc = a.exec();

	return rc;
}
