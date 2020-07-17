	{
		std::vector<DebugPrintableFamily>* v = new std::vector<DebugPrintableFamily>;
		array[0] = v;
		cleanup_callbacks.push_back([v](void) {delete v;});
	}
	{
		std::vector<GreyBoxFamily>* v = new std::vector<GreyBoxFamily>;
		array[1] = v;
		cleanup_callbacks.push_back([v](void) {delete v;});
	}
