#include "pch.h"
#include "Dispatcher.h"


namespace Yugo
{

	std::map<std::type_index, Dispatcher::ListenerList> Dispatcher::m_Subscribers;

}