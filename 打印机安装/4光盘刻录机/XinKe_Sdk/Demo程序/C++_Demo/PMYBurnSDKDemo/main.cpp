#include "stdafx.h"
#include "PMYBurnSDKDemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


	if (!SDKFunc().Init())
	{
		QMessageBox::warning(NULL, "error", "Init PMYBurnSvcSdk fail.");
		return 0;
	}

	PMYBurnSdkDemo w;
	w.show();
	return a.exec();
}
