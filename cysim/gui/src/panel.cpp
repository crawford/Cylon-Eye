#include "Panel.h"
#define PADDING 5

Panel::Panel(unsigned long long ZID, short PID) : QGraphicsItem(), _size(0, 0, 245, 50) {
	_ZID = ZID;
	_PID = PID;
	_auxLED = false;

	for(int i = 0; i < 16; i++)
		_intensities[i] = 0;

	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
}

QRectF Panel::boundingRect() const {
	return _size.adjusted(-PADDING, -PADDING, PADDING, PADDING);
}

void Panel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *parent) {
	//Draw frame
	if(isSelected()) {
		painter->setPen(QColor(78, 118, 168));
		painter->drawRoundedRect(boundingRect(), 10, 10);
	}
	painter->setPen(QColor(0, 0, 0));
	painter->setBrush(QColor(50, 50, 50));
	painter->drawRoundedRect(_size, 6, 6);

	//Draw leds
	double ledWidth = (_size.width() - PADDING)/16;
	for(int i = 0; i < 16; i++) {
		painter->setPen(QPen(QColor(100, 100, 100), 1));
		painter->setBrush(QColor(100 + _intensities[i]*22, 0, 0));
		painter->drawRoundRect(ledWidth*i + PADDING, PADDING, ledWidth - PADDING, _size.height() - 2*PADDING, 2, 4);
	}

	//Draw aux led
	if(_auxLED) {
		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor(255, 255, 0));
		painter->drawEllipse(PADDING/2 - 1, _size.height()/2, 2, 2);
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

void Panel::setAuxLED(bool val) {
	_auxLED = val;
}
