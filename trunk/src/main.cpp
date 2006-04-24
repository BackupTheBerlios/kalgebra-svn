#include "kalgebra.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char description[] = I18N_NOOP("KAlgebra");
static const char version[] = "0.4.1";

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
	KAboutData about("kalgebra", I18N_NOOP("KAlgebra"), version, description, KAboutData::License_GPL_V2, "(C) 2005 - 2006 Aleix Pol i Gonzalez", "http://kalgebra.berlios.de", 0, "aleixpol@gmail.com");
	about.addAuthor( "Aleix Pol i Gonzalez", 0, "aleixpol@gmail.com" );
	about.setHomepage("http://kalgebra.sourceforge.net");
	KCmdLineArgs::init(argc, argv, &about);
	KCmdLineArgs::addCmdLineOptions( options );
	KApplication app;
	KAlgebra *mainWin = 0;
	
	app.dcopClient()->registerAs(app.name(), false);
		
	if (app.isRestored()) {
		RESTORE(KAlgebra);
	} else {
		// no session.. just start up normally
		KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	
		/// @todo do something with the command line args here
	
		mainWin = new KAlgebra();
		app.setMainWidget( mainWin );
		mainWin->show();
	
		args->clear();
	}
	
	return app.exec();
}

