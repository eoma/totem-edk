#pragma once

#include "../HashedString.h"

#include <sigslot.h>
#include <unordered_map>
#include <memory>

namespace Totem {
template<class ComponentType, class UserData> class Component;

class IEventSignal { public: IEventSignal() {} virtual ~IEventSignal() {} };
template<class DefaultParam = void*> class EventSignal0 : public IEventSignal { public: sigslot::signal0<> signal; };
template<class T> class EventSignal1 : public IEventSignal { public: sigslot::signal1<T> signal; };
template<class T, class U> class EventSignal2 : public IEventSignal { public: sigslot::signal2<T, U> signal; };
template<class T, class U, class V> class EventSignal3 : public IEventSignal { public: sigslot::signal3<T, U, V> signal; };
template<class T, class U, class V, class W> class EventSignal4 : public IEventSignal{ public: sigslot::signal4<T, U, V, W> signal; };
template<class T, class U, class V, class W, class X> class EventSignal5 : public IEventSignal { public: sigslot::signal5<T, U, V, W, X> signal; };
template<class T, class U, class V, class W, class X, class Y> class EventSignal6 : public IEventSignal { public: sigslot::signal6<T, U, V, W, X, Y> signal; };

class DefaultEventFactory
{
public:
	static std::shared_ptr<EventSignal0<>> createEvent();
	template<class Param1> static std::shared_ptr<EventSignal1<Param1>> createEvent();
	template<class Param1, class Param2> static std::shared_ptr<EventSignal2<Param1,Param2>> createEvent();
	template<class Param1, class Param2, class Param3> static std::shared_ptr<EventSignal3<Param1,Param2,Param3>> createEvent();
	template<class Param1, class Param2, class Param3, class Param4> static std::shared_ptr<EventSignal4<Param1,Param2,Param3,Param4>> createEvent();
	template<class Param1, class Param2, class Param3, class Param4, class Param5> static std::shared_ptr<EventSignal5<Param1,Param2,Param3,Param4,Param5>> createEvent();
	template<class Param1, class Param2, class Param3, class Param4, class Param5, class Param6> static std::shared_ptr<EventSignal6<Param1,Param2,Param3,Param4,Param5,Param6>> createEvent();
};

template<class EventFactory = DefaultEventFactory>
class EventSystem : public sigslot::has_slots<>
{
public:
	virtual ~EventSystem() {}

	void sendEvent0(HashedString type, bool requireReceiver = true);
	template<class T> void sendEvent1(HashedString type, T arg0, bool requireReceiver = true);
	template<class T, class U> void sendEvent2(HashedString type, T arg0, U arg1, bool requireReceiver = true);
	template<class T, class U, class V> void sendEvent3(HashedString type, T arg0, U arg1, V arg2, bool requireReceiver = true);
	template<class T, class U, class V, class W> void sendEvent4(HashedString type, T arg0, U arg1, V arg2, W arg3, bool requireReceiver = true);
	template<class T, class U, class V, class W, class X> void sendEvent5(HashedString type, T arg0, U arg1, V arg2, W arg3, X arg4, bool requireReceiver = true);
	template<class T, class U, class V, class W, class X, class Y> void sendEvent6(HashedString type, T arg0, U arg1, V arg2, W arg3, X arg4, Y arg5, bool requireReceiver = true);

	sigslot::signal0<> &registerToEvent0(HashedString type);
	template<class T> sigslot::signal1<T> &registerToEvent1(HashedString type);
	template<class T, class U> sigslot::signal2<T, U> &registerToEvent2(HashedString type);
	template<class T, class U, class V> sigslot::signal3<T, U, V> &registerToEvent3(HashedString type);
	template<class T, class U, class V, class W> sigslot::signal4<T, U, V, W> &registerToEvent4(HashedString type);
	template<class T, class U, class V, class W, class X> sigslot::signal5<T, U, V, W, X> &registerToEvent5(HashedString type);
	template<class T, class U, class V, class W, class X, class Y> sigslot::signal6<T, U, V, W, X, Y> &registerToEvent6(HashedString type);

	bool hasEvent(const HashedString &id, int num_params = -1);

protected:
	std::unordered_map<unsigned int, std::shared_ptr<IEventSignal>> events0;
	std::unordered_map<unsigned int, std::shared_ptr<IEventSignal>> events1;
	std::unordered_map<unsigned int, std::shared_ptr<IEventSignal>> events2;
	std::unordered_map<unsigned int, std::shared_ptr<IEventSignal>> events3;
	std::unordered_map<unsigned int, std::shared_ptr<IEventSignal>> events4;
	std::unordered_map<unsigned int, std::shared_ptr<IEventSignal>> events5;
	std::unordered_map<unsigned int, std::shared_ptr<IEventSignal>> events6;
};

#include "EventSystem.inl"

} //namespace Totem
