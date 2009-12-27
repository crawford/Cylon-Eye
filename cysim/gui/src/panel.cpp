#include "Panel.h"

Panel::Panel(unsigned long long ZID, short PID) : QGraphicsItem(), _size(0, 0, 230, 50) {
	_ZID = ZID;
	_PID = PID;

	for(int i = 0; i < 16; i++)
		_intensities[i] = 0;

	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
}

QRectF Panel::boundingRect() const {
	return _size.adjusted(-5, -5, 5, 5);
}

void Panel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *parent) {
	//Draw frame
	if(isSelected()) {
		painter->setPen(QColor(78, 118, 168));
		painter->drawRoundedRect(_size.adjusted(-5, -5, 5, 5), 10, 10);
	}
	painter->setPen(QColor(0, 0, 0));
	painter->setBrush(QColor(50, 50, 50));
	painter->drawRoundedRect(_size, 6, 6);

	//Draw leds
	for(int i = 0; i < 15; i++) {
		painter->setPen(QPen(QColor(100, 100, 100), 1));
		painter->setBrush(QColor(100 + _intensities[i]*22, 0, 0));
		painter->drawRoundRect(i*15 + 5, 5, 10, 40, 2, 4);
	}
}

unsigned long long Panel::getZID() {
	return _ZID;
}

short Panel::getPID() {
	return _PID;
}

short Panel::getIntensity(short channel) {
	return _intensities[channel];
}

void Panel::setIntensity(short channel, short value) {
	_intensities[channel] = value;
}

