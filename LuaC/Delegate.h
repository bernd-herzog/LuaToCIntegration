#pragma once

#include <vector>

using namespace std;

template<class T>
class EventArgs{
public:
	T arg1;
	EventArgs(T arg1):arg1(arg1){
	}
};

template<class T, class X>
class DelegateBase{
public:
	virtual void invoke(X *sender, EventArgs<T> args)=0;

};

template <class ClassWithEvent, class Argument1Type>
class Event{
public:
	typedef DelegateBase<Argument1Type, ClassWithEvent> EventBaseType;
	typedef EventArgs<Argument1Type> EventArgsType;

	vector<EventBaseType *> m_spawns;

	template<class T, void (T::*TMethod)(ClassWithEvent *, EventArgsType)>
	void attach(T *theObject){

		class Delegate : public EventBaseType{
		public:
			T *theObject;
			void invoke(ClassWithEvent *sender, EventArgsType args){ //virtual
				(theObject->*TMethod)(sender, args);
			}
		};

		Delegate *e = new Delegate();
		e->theObject = theObject;
		m_spawns.push_back(e);
	}

	void OnEvent(ClassWithEvent *sender,  EventArgsType args)
	{
		for(vector<EventBaseType *>::iterator iter = m_spawns.begin(); iter != m_spawns.end(); iter++){
			(*iter)->invoke(sender, args);
		}
	}
};

/* benötigt Visual Studio 2013 und wird noch nicht benutzt
template <typename TSignature>
class delegate;

template<class ReturnType, class... Params>
class delegate<ReturnType(Params...)>
{
public:
	delegate()
		: object_ptr(0)
		, stub_ptr(0)
	{}

	template <class T, ReturnType(T::*TMethod)(Params...)>
	static delegate from_method(T* object_ptr)
	{
		delegate d;
		d.object_ptr = object_ptr;
		d.stub_ptr = &method_stub<T, TMethod>; // #1
		return d;
	}

	ReturnType operator()(Params... a1) const
	{
		return (*stub_ptr)(object_ptr, a1...);
	}

private:
	typedef ReturnType(*stub_type)(void* object_ptr, Params...);

	void* object_ptr;
	stub_type stub_ptr;

	template <class T, ReturnType(T::*TMethod)(Params...)>
	static ReturnType method_stub(void* object_ptr, Params... a1)
	{
		T* p = static_cast<T*>(object_ptr);
		return (p->*TMethod)(a1...); // #2
	}
};


*/