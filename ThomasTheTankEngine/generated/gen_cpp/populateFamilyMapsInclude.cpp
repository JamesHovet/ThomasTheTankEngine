	{
		std::unordered_map<entityID, BasicModelFamily>* m = new std::unordered_map<entityID, BasicModelFamily>;
		array[1] = m;
		cleanup_callbacks.push_back([m](void) {delete m;});
		Family<BasicModelFamily>::mask.set(0);
		Family<BasicModelFamily>::mask.set(3);
	}
	{
		std::unordered_map<entityID, GreyBoxFamily>* m = new std::unordered_map<entityID, GreyBoxFamily>;
		array[2] = m;
		cleanup_callbacks.push_back([m](void) {delete m;});
		Family<GreyBoxFamily>::mask.set(0);
		Family<GreyBoxFamily>::mask.set(4);
	}
	{
		std::unordered_map<entityID, AABBCollisionFamily>* m = new std::unordered_map<entityID, AABBCollisionFamily>;
		array[3] = m;
		cleanup_callbacks.push_back([m](void) {delete m;});
		Family<AABBCollisionFamily>::mask.set(0);
		Family<AABBCollisionFamily>::mask.set(5);
	}
