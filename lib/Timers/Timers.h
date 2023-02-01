#ifndef TIMERS_H
#define TIMERS_H

#include <Arduino.h>

struct ITimerInfo
{
	virtual void check(unsigned long time) = 0;
};

template <class T>
struct TimerInfo : ITimerInfo
{
	typedef void (T::*Handler)();
private:
	unsigned long startTime;
	unsigned long interval;
	T *object;
	Handler handler;

public:
	TimerInfo(int interval, T *object, Handler handler, int delay)	:
		interval(interval),
		object(object),
		handler(handler)
	{
		startTime = millis();

		if (delay > 0) {
			startTime -= interval - delay;
		}
	}

	void check(unsigned long time) override
	{
		if ((time - startTime) > interval) {
			startTime += interval;
			(object->*handler)();
		}
	}
};

template <unsigned MAX_TIMERS>
class Timers
{
	int count = 0;
	ITimerInfo* timers[MAX_TIMERS]; // avoid relocations on append

public:
	template <class T>
	void schedule(int interval, T *object, typename TimerInfo<T>::Handler handler, int delay = 0)
	{
		timers[count] = new TimerInfo<T>(interval, object, handler, delay);
		count++;
	}

	void tick()
	{
		unsigned long time = millis();

		for (int i = 0; i < count; ++i) {
			timers[i]->check(time);
		}
	}
};

#endif
