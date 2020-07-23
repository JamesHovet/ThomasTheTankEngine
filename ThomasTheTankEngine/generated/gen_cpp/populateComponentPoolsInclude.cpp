	{
		boost::object_pool<DebugPrintComponent>* p = new boost::object_pool<DebugPrintComponent>;
		pool[2] = p;
		destroyers[2] = [p](void * ptr){ 
			p->free((boost::object_pool<DebugPrintComponent>::element_type *) ptr);
		};
		cleanup_callbacks.push_back([p](void) { delete p; });
	}
	{
		boost::object_pool<GreyBoxComponent>* p = new boost::object_pool<GreyBoxComponent>;
		pool[3] = p;
		destroyers[3] = [p](void * ptr){ 
			p->free((boost::object_pool<GreyBoxComponent>::element_type *) ptr);
		};
		cleanup_callbacks.push_back([p](void) { delete p; });
	}