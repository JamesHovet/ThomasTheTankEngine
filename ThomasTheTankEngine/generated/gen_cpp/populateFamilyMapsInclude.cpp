	{
		std::unordered_map<entityID, DebugPrintableFamily>* m = new std::unordered_map<entityID, DebugPrintableFamily>;
		array[0] = m;
		cleanup_callbacks.push_back([m](void) {delete m;});
		Family<DebugPrintableFamily>::mask.set(2);
	}
	{
		std::unordered_map<entityID, GreyBoxFamily>* m = new std::unordered_map<entityID, GreyBoxFamily>;
		array[1] = m;
		cleanup_callbacks.push_back([m](void) {delete m;});
		Family<GreyBoxFamily>::mask.set(0);
		Family<GreyBoxFamily>::mask.set(3);
	}
	{
		std::unordered_map<entityID, CameraFamily>* m = new std::unordered_map<entityID, CameraFamily>;
		array[2] = m;
		cleanup_callbacks.push_back([m](void) {delete m;});
		Family<CameraFamily>::mask.set(0);
		Family<CameraFamily>::mask.set(4);
	}
