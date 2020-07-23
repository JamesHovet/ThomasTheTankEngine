	{
		std::vector<DebugPrintableFamily>* v = new std::vector<DebugPrintableFamily>;
		array[0] = v;
		cleanup_callbacks.push_back([v](void) {delete v;});
		Family<DebugPrintableFamily>::mask.set(2);
	}
	{
		std::vector<GreyBoxFamily>* v = new std::vector<GreyBoxFamily>;
		array[1] = v;
		cleanup_callbacks.push_back([v](void) {delete v;});
		Family<GreyBoxFamily>::mask.set(0);
		Family<GreyBoxFamily>::mask.set(3);
	}
