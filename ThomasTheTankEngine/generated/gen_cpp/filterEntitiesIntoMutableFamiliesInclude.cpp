{
if(ECSUtils::doesPassFilter(mask, Family<DebugPrintableFamily>::mask)){
	DebugPrintableFamily family = DebugPrintableFamily(eID, getComponent<DebugPrintComponent>(eID));
	getFamilyMap<DebugPrintableFamily>().emplace(std::make_pair(eID, family));
	}
}
{
if(ECSUtils::doesPassFilter(mask, Family<GreyBoxFamily>::mask)){
	GreyBoxFamily family = GreyBoxFamily(eID, getComponent<TransformComponent>(eID), getComponent<GreyBoxComponent>(eID));
	getFamilyMap<GreyBoxFamily>().emplace(std::make_pair(eID, family));
	}
}
