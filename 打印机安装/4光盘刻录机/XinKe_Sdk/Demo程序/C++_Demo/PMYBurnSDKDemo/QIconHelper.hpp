#ifndef ICONHELPER_H
#define ICONHELPER_H

#include <QObject>
#include <QFont>
#include <QFontDatabase>
#include <QMutex>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

class QIconHelper// : public QObject
{
public:
	static int GetFontId()
	{
		static int fontId = -1;
		if (fontId == -1)
		{
			fontId = QFontDatabase::addApplicationFont(":/Skin/fontawesome-webfont.ttf");
		}
		return fontId;
	}

    static void SetIcon(QLabel* lab, QChar c, int size = 10)
	{
		int fontId = QIconHelper::GetFontId();
		if (fontId == -1)
		{
			return;
		}
		QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
		QFont iconFont = QFont(fontName);
		iconFont.setPointSize(size);
		lab->setFont(iconFont);
		lab->setText(c);
	}
	
    static void SetIcon(QAbstractButton* btn, QChar c, int size = 10)
	{
		int fontId = QIconHelper::GetFontId();
		if (fontId == -1)
		{
			return;
		}
		QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
		QFont iconFont = QFont(fontName);
		iconFont.setPointSize(size);
		btn->setFont(iconFont);
		btn->setText(c);
	}

};

#endif // ICONHELPER_H
