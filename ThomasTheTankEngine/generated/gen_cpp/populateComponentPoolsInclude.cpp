	{
		boost::object_pool<GreyBoxComponent>* p = new boost::object_pool<GreyBoxComponent>;
		pool[3] = p;
		destroyers[3] = [p](void * ptr){ 
			p->free((boost::object_pool<GreyBoxComponent>::element_type *) ptr);
		};
		cleanup_callbacks.push_back([p](void) { delete p; });
	}
	{
		boost::object_pool<AABBColliderComponent>* p = new boost::object_pool<AABBColliderComponent>;
		pool[4] = p;
		destroyers[4] = [p](void * ptr){ 
			p->free((boost::object_pool<AABBColliderComponent>::element_type *) ptr);
		};
		cleanup_callbacks.push_back([p](void) { delete p; });
	}
