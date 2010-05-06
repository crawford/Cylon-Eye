#ifndef PANEL_H
#define PANEL_H

#include <QGraphicsItem>
#include <QPainter>

class Panel : public QGraphicsItem {
public:
	Panel(unsigned long long ZID, short PID);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *parent = 0);
	unsigned long long getZID();
	short getPID();
	short getIntensity(short channel);
	void setIntensity(short channel, short value);
	void setAuxLED(bool val);

private:
	QRectF _size;
	short _intensities[16];
	unsigned long long _ZID;
	short _PID;
	bool _auxLED;
};

#endif

