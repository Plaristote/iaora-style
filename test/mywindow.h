#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>

class MyWindow : public QMainWindow
{
	Q_OBJECT
public:
	MyWindow();
	~MyWindow();

public slots:
	void slotSetStyle(const QString &style);
	void slotContextMenu(const QPoint &pos);
};

#endif
