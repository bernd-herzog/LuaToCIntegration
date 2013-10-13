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