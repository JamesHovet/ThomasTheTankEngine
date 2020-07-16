	{
		boost::object_pool<DebugPrintComponent>* p = new boost::object_pool<DebugPrintComponent>;
		pool[0] = p;
		destroyers[0] = [p](void * ptr){ 
			p->free((boost::object_pool<DebugPrintComponent>::element_type *) ptr);
		};
		cleanup_callbacks.push_back([p](void) { delete p; });
	}
	{
		boost::object_pool<DebugNameComponent>* p = new boost::object_pool<DebugNameComponent>;
		pool[1] = p;
		destroyers[1] = [p](void * ptr){ 
			p->free((boost::object_pool<DebugNameComponent>::element_type *) ptr);
		};
		cleanup_callbacks.push_back([p](void) { delete p; });
	}
