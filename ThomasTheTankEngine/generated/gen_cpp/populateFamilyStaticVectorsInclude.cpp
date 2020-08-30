	{
		std::vector<BasicModelFamilyStatic>* v = new std::vector<BasicModelFamilyStatic>;
		array[1] = v;
		cleanup_callbacks.push_back([v](void) {delete v;});
		Family<BasicModelFamilyStatic>::mask.set(0);
		Family<BasicModelFamilyStatic>::mask.set(3);
	}
	{
		std::vector<GreyBoxFamilyStatic>* v = new std::vector<GreyBoxFamilyStatic>;
		array[2] = v;
		cleanup_callbacks.push_back([v](void) {delete v;});
		Family<GreyBoxFamilyStatic>::mask.set(0);
		Family<GreyBoxFamilyStatic>::mask.set(4);
	}
	{
		std::vector<AABBCollisionFamilyStatic>* v = new std::vector<AABBCollisionFamilyStatic>;
		array[3] = v;
		cleanup_callbacks.push_back([v](void) {delete v;});
		Family<AABBCollisionFamilyStatic>::mask.set(0);
		Family<AABBCollisionFamilyStatic>::mask.set(5);
	}
