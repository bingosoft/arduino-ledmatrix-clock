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

	T *object;
	Handler handler;
	int interval;
	unsigned long elapsed;

	TimerInfo() { }

	TimerInfo(int interval, T *object, Handler handler, int delay)	:
		interval(interval),
		elapsed(millis()),
		object(object),
		handler(handler)
	{
		if (delay > 0) {
			elapsed -= interval - delay;
		}
	}

	void check(unsigned long time) override
	{
		if ((time - elapsed) > interval) {
			elapsed += interval;

			if (object != 0) {
				(object->*handler)();
			}
		}
	}
};

template <unsigned MAX_TIMERS>
class Timers
{
	int count = 0;
	ITimerInfo* timers[MAX_TIMERS];

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
