#ifndef TIMERS_H
#define TIMERS_H

#include <Arduino.h>

template <class T>
struct TimerInfo
{
	typedef void (T::*TimerFunc)();
	int interval;
	unsigned long elapsed;
	TimerFunc handler;
	T *object;

	TimerInfo() { }

	TimerInfo(int interval, TimerFunc handler, T *object, int delay)	:
		interval(interval),
		elapsed(millis()),
		handler(handler),
		object(object)
	{
		if (delay > 0) {
			elapsed -= interval - delay;
		}
	}
};

template <class T>
class Timers
{
	typedef typename TimerInfo<T>::TimerFunc TimerFunc;
	int count = 0;
	TimerInfo<T> timers[10];

public:
	Timers() { }

	void schedule(int interval, TimerFunc handler, T *object,  int delay = 0)
	{
		timers[count] = TimerInfo<T>(interval, handler, object, delay);
		count++;
	}

	void tick()
	{
		unsigned long time = millis();
		for (int i = 0; i < count; ++i) {
			TimerInfo<T> &t = timers[i];

			if ((time - t.elapsed) > t.interval) {
				t.elapsed += t.interval;

				if (t.object != 0) {
					(t.object->*t.handler)();
				}
			}
		}
	}
};

#endif
