
inline std::shared_ptr<EventSignal0<>> DefaultEventFactory::createEvent()
{
	return std::make_shared<EventSignal0<>>();
}

template<class Param1> 
inline std::shared_ptr<EventSignal1<Param1>> DefaultEventFactory::createEvent()
{
	return std::make_shared<EventSignal1<Param1>>();
}

template<class Param1, class Param2> 
inline std::shared_ptr<EventSignal2<Param1,Param2>> DefaultEventFactory::createEvent()
{
	return std::make_shared<EventSignal2<Param1,Param2>>();
}

template<class Param1, class Param2, class Param3> 
inline std::shared_ptr<EventSignal3<Param1,Param2,Param3>> DefaultEventFactory::createEvent()
{
	return std::make_shared<EventSignal3<Param1,Param2,Param3>>();
}

template<class Param1, class Param2, class Param3, class Param4> 
inline std::shared_ptr<EventSignal4<Param1,Param2,Param3,Param4>> DefaultEventFactory::createEvent()
{
	return std::make_shared<EventSignal4<Param1,Param2,Param3,Param4>>();
}

template<class Param1, class Param2, class Param3, class Param4, class Param5> 
inline std::shared_ptr<EventSignal5<Param1,Param2,Param3,Param4,Param5>> DefaultEventFactory::createEvent()
{
	return std::make_shared<EventSignal5<Param1,Param2,Param3,Param4,Param5>>();
}

template<class Param1, class Param2, class Param3, class Param4, class Param5, class Param6> 
inline std::shared_ptr<EventSignal6<Param1,Param2,Param3,Param4,Param5,Param6>> DefaultEventFactory::createEvent()
{
	return std::make_shared<EventSignal6<Param1,Param2,Param3,Param4,Param5,Param6>>();
}

template<class EventFactory>
inline void EventSystem<EventFactory>::sendEvent0(HashedString type, bool requireReceiver)
{
	auto it = events0.find(type.getId());
	if(it == events0.end())
	{
		if(requireReceiver)
			throw std::runtime_error(("Couldn't find event type " + type.getStr() + " in events0 registry!").c_str());
		else
			return;
	}

	std::static_pointer_cast<EventSignal0<>>(it->second)->signal.invoke();
}

template<class EventFactory>
template<class T>
inline void EventSystem<EventFactory>::sendEvent1(HashedString type, T arg0, bool requireReceiver)
{
	auto it = events1.find(type.getId());
	if(it == events1.end())
	{
		if(requireReceiver)
			throw std::runtime_error(("Couldn't find event type " + type.getStr() + " in events1 registry!").c_str());
		else
			return;
	}

#ifdef _DEBUG
	auto signal = std::dynamic_pointer_cast<EventSignal1<T>>(it->second);
	if(signal == nullptr)
		throw std::runtime_error(("Tried to invoke event " + type.getStr() + ", but the argument type didn't match the registered type!").c_str());
	signal->signal.invoke(arg0);
#else
	std::static_pointer_cast<EventSignal1<T>>(it->second)->signal.invoke(arg0);
#endif
}

template<class EventFactory>
template<class T, class U>
inline void EventSystem<EventFactory>::sendEvent2(HashedString type, T arg0, U arg1, bool requireReceiver)
{
	auto it = events2.find(type.getId());
	if(it == events2.end())
	{
		if(requireReceiver)
			throw std::runtime_error(("Couldn't find event type " + type.getStr() + " in events2 registry!").c_str());
		else
			return;
	}

#ifdef _DEBUG
	auto signal = std::dynamic_pointer_cast<EventSignal2<T,U>>(it->second);
	if(signal == nullptr)
		throw std::runtime_error(("Tried to invoke event " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
	signal->signal.invoke(arg0, arg1);
#else
	std::static_pointer_cast<EventSignal2<T,U>>(it->second)->signal.invoke(arg0, arg1);
#endif
}

template<class EventFactory>
template<class T, class U, class V>
inline void EventSystem<EventFactory>::sendEvent3(HashedString type, T arg0, U arg1, V arg2, bool requireReceiver)
{
	auto it = events3.find(type.getId());
	if(it == events3.end())
	{
		if(requireReceiver)
			throw std::runtime_error(("Couldn't find event type " + type.getStr() + " in events3 registry!").c_str());
		else
			return;
	}

#ifdef _DEBUG
	auto signal = std::dynamic_pointer_cast<EventSignal3<T,U,V>>(it->second);
	if(signal == nullptr)
		throw std::runtime_error(("Tried to invoke event " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
	signal->signal.invoke(arg0, arg1, arg2);
#else
	std::static_pointer_cast<EventSignal3<T,U,V>>(it->second)->signal.invoke(arg0, arg1, arg2);
#endif
}

template<class EventFactory>
template<class T, class U, class V, class W>
inline void EventSystem<EventFactory>::sendEvent4(HashedString type, T arg0, U arg1, V arg2, W arg3, bool requireReceiver)
{
	auto it = events4.find(type.getId());
	if(it == events4.end())
	{
		if(requireReceiver)
			throw std::runtime_error(("Couldn't find event type " + type.getStr() + " in events4 registry!").c_str());
		else
			return;
	}

#ifdef _DEBUG
	auto signal = std::dynamic_pointer_cast<EventSignal4<T,U,V,W>>(it->second);
	if(signal == nullptr)
		throw std::runtime_error(("Tried to invoke event " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
	signal->signal.invoke(arg0, arg1, arg2, arg3);
#else
	std::static_pointer_cast<EventSignal4<T,U,V,W>>(it->second)->signal.invoke(arg0, arg1, arg2, arg3);
#endif
}

template<class EventFactory>
template<class T, class U, class V, class W, class X>
inline void EventSystem<EventFactory>::sendEvent5(HashedString type, T arg0, U arg1, V arg2, W arg3, X arg4, bool requireReceiver)
{
	auto it = events5.find(type.getId());
	if(it == events5.end())
	{
		if(requireReceiver)
			throw std::runtime_error(("Couldn't find event type " + type.getStr() + " in events5 registry!").c_str());
		else
			return;
	}

#ifdef _DEBUG
	auto signal = std::dynamic_pointer_cast<EventSignal5<T,U,V,W,X>>(it->second);
	if(signal == nullptr)
		throw std::runtime_error(("Tried to invoke event " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
	signal->signal.invoke(arg0, arg1, arg2, arg3, arg4);
#else
	std::static_pointer_cast<EventSignal5<T,U,V,W,X>>(it->second)->signal.invoke(arg0, arg1, arg2, arg3, arg4);
#endif
}

template<class EventFactory>
template<class T, class U, class V, class W, class X, class Y>
inline void EventSystem<EventFactory>::sendEvent6(HashedString type, T arg0, U arg1, V arg2, W arg3, X arg4, Y arg5, bool requireReceiver)
{
	auto it = events6.find(type.getId());
	if(it == events6.end())
	{
		if(requireReceiver)
			throw std::runtime_error(("Couldn't find event type " + type.getStr() + " in events6 registry!").c_str());
		else
			return;
	}

#ifdef _DEBUG
	auto signal = std::dynamic_pointer_cast<EventSignal6<T,U,V,W,X,Y>>(it->second);
	if(signal == nullptr)
		throw std::runtime_error(("Tried to invoke event " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
	signal->signal.invoke(arg0, arg1, arg2, arg3, arg4, arg5);
#else
	std::static_pointer_cast<EventSignal6<T,U,V,W,X,Y>>(it->second)->signal.invoke(arg0, arg1, arg2, arg3, arg4, arg5);
#endif
}

//------------------------------------------------------------------

template<class EventFactory>
inline sigslot::signal0<> &EventSystem<EventFactory>::registerToEvent0(HashedString type)
{
	unsigned int hash_id = type.getId();
	auto it = events0.find(hash_id);
	if(it == events0.end())
	{
		auto signal = EventFactory::template createEvent();
		events0[hash_id] = signal;
		return signal->signal;
	}
	else
	{
		auto signal = std::static_pointer_cast<EventSignal0<>>(it->second);
		return signal->signal;
	}
}

template<class EventFactory>
template<class T>
inline sigslot::signal1<T> &EventSystem<EventFactory>::registerToEvent1(HashedString type)
{
	auto it = events1.find(type.getId());
	if(it == events1.end())
	{
		auto signal = EventFactory::template createEvent<T>();
		events1[type.getId()] = signal;
		return signal->signal;
	}
	else
	{
#ifdef _DEBUG
		auto signal = std::dynamic_pointer_cast<EventSignal1<T>>(it->second);
		if(signal == nullptr)
			throw std::runtime_error(("Tried to return the event signal " + type.getStr() + ", but the argument type didn't match the registered type!").c_str());
#else
		auto signal = std::static_pointer_cast<EventSignal1<T>>(it->second);
#endif
		return signal->signal;
	}
}

template<class EventFactory>
template<class T, class U>
inline sigslot::signal2<T, U> &EventSystem<EventFactory>::registerToEvent2(HashedString type)
{
	auto it = events2.find(type.getId());
	if(it == events2.end())
	{
		auto signal = EventFactory::template createEvent<T,U>();
		events2[type.getId()] = signal;
		return signal->signal;
	}
	else
	{
#ifdef _DEBUG
		auto signal = std::dynamic_pointer_cast<EventSignal2<T,U>>(it->second);
		if(signal == nullptr)
			throw std::runtime_error(("Tried toreturn the event signal " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
#else
		auto signal = std::static_pointer_cast<EventSignal2<T,U>>(it->second);
#endif
		return signal->signal;
	}
}

template<class EventFactory>
template<class T, class U, class V>
inline sigslot::signal3<T, U, V> &EventSystem<EventFactory>::registerToEvent3(HashedString type)
{
	auto it = events3.find(type.getId());
	if(it == events3.end())
	{
		auto signal = EventFactory::template createEvent<T,U,V>();
		events3[type.getId()] = signal;
		return signal->signal;
	}
	else
	{
#ifdef _DEBUG
		auto signal = std::dynamic_pointer_cast<EventSignal3<T,U,V>>(it->second);
		if(signal == nullptr)
			throw std::runtime_error(("Tried toreturn the event signal " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
#else
		auto signal = std::static_pointer_cast<EventSignal3<T,U,V>>(it->second);
#endif
		return signal->signal;
	}
}

template<class EventFactory>
template<class T, class U, class V, class W>
inline sigslot::signal4<T, U, V, W> &EventSystem<EventFactory>::registerToEvent4(HashedString type)
{
	auto it = events4.find(type.getId());
	if(it == events4.end())
	{
		auto signal = EventFactory::template createEvent<T,U,V,W>();
		events4[type.getId()] = signal;
		return signal->signal;
	}
	else
	{
#ifdef _DEBUG
		auto signal = std::dynamic_pointer_cast<EventSignal4<T,U,V,W>>(it->second);
		if(signal == nullptr)
			throw std::runtime_error(("Tried toreturn the event signal " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
#else
		auto signal = std::static_pointer_cast<EventSignal4<T,U,V,W>>(it->second);
#endif
		return signal->signal;
	}
}

template<class EventFactory>
template<class T, class U, class V, class W, class X>
inline sigslot::signal5<T, U, V, W, X> &EventSystem<EventFactory>::registerToEvent5(HashedString type)
{
	auto it = events5.find(type.getId());
	if(it == events5.end())
	{
		auto signal = EventFactory::template createEvent<T,U,V,W,X>();
		events5[type.getId()] = signal;
		return signal->signal;
	}
	else
	{
#ifdef _DEBUG
		auto signal = std::dynamic_pointer_cast<EventSignal5<T,U,V,W,X>>(it->second);
		if(signal == nullptr)
			throw std::runtime_error(("Tried toreturn the event signal " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
#else
		auto signal = std::static_pointer_cast<EventSignal5<T,U,V,W,X>>(it->second);
#endif
		return signal->signal;
	}
}

template<class EventFactory>
template<class T, class U, class V, class W, class X, class Y>
inline sigslot::signal6<T, U, V, W, X, Y> &EventSystem<EventFactory>::registerToEvent6(HashedString type)
{
	auto it = events6.find(type.getId());
	if(it == events6.end())
	{
		auto signal = EventFactory::template createEvent<T,U,V,W,X,Y>();
		events6[type.getId()] = signal;
		return signal->signal;
	}
	else
	{
#ifdef _DEBUG
		auto signal = std::dynamic_pointer_cast<EventSignal6<T,U,V,W,X,Y>>(it->second);
		if(signal == nullptr)
			throw std::runtime_error(("Tried toreturn the event signal " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
#else
		auto signal = std::static_pointer_cast<EventSignal6<T,U,V,W,X,Y>>(it->second);
#endif
		return signal->signal;
	}
}

//---------------------------------------------------------------------------------------

template<class EventFactory>
bool EventSystem<EventFactory>::hasEvent(const HashedString &id, int num_params)
{
	if(num_params == 0)
	{
		auto it = events0.find(id.getId());
		if(it != events0.end())
			return true;
		else
			return false;
	}
	else if(num_params == 1)
	{
		auto it = events1.find(id.getId());
		if(it != events1.end())
			return true;
		else
			return false;
	}
	else if(num_params == 2)
	{
		auto it = events2.find(id.getId());
		if(it != events2.end())
			return true;
		else
			return false;
	}
	else if(num_params == 3)
	{
		auto it = events3.find(id.getId());
		if(it != events3.end())
			return true;
		else
			return false;
	}
	else if(num_params == 4)
	{
		auto it = events4.find(id.getId());
		if(it != events4.end())
			return true;
		else
			return false;
	}
	else if(num_params == 5)
	{
		auto it = events5.find(id.getId());
		if(it != events5.end())
			return true;
		else
			return false;
	}
	else if(num_params == 6)
	{
		auto it = events6.find(id.getId());
		if(it != events6.end())
			return true;
		else
			return false;
	}
	else
	{
		auto it = events0.find(id.getId());
		if(it != events0.end())
			return true;
		it = events1.find(id.getId());
		if(it != events1.end())
			return true;
		it = events2.find(id.getId());
		if(it != events2.end())
			return true;
		it = events3.find(id.getId());
		if(it != events3.end())
			return true;
		it = events4.find(id.getId());
		if(it != events4.end())
			return true;
		it = events5.find(id.getId());
		if(it != events5.end())
			return true;
		it = events6.find(id.getId());
		if(it != events6.end())
			return true;

		return false;
	}
}
