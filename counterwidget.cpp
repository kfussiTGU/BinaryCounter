#include <QHBoxLayout>
#include "counterwidget.h"
#include "config.h"

CounterWidget::CounterWidget(QWidget *parent)
    : QWidget(parent), m_count(0), m_timer(this), m_gpio(this)
{
    setWindowTitle("BinaryCounter");
    resize(300, 200);
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);

    // QLCDNumber zur Anzeige des Zaehlerstandes dezimal:
    m_lcdout = new QLCDNumber;
    layout->addWidget(m_lcdout);
    m_lcdout->display(m_count);


    connect(&m_timer, &QTimer::timeout, this, &CounterWidget::updateCounter);
    m_timer.start(T_UPDATE);
}

CounterWidget::~CounterWidget()
{

}

// Zaehlerstand im Widget und dual ueber LEDs anzeigen:
void CounterWidget::updateCounter()
{
    if(m_gpio.isActivated(BUTTONS[0]))
        m_count = (m_count-1) & 15;
    if(m_gpio.isActivated(BUTTONS[1]))
        m_count = 0;
    if(m_gpio.isActivated(BUTTONS[2]))
        m_count = (m_count+1) & 15;


    m_lcdout->display(m_count);
    updateLED();
}

//Zählstand der LEDs aktualisieren
void CounterWidget::updateLED()
{
    for(int i = 0; i < 4; i++)
        m_gpio.set(LEDS[i], (m_count >> i) & 1);
}

