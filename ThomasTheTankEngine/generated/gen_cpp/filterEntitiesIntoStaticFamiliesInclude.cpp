{
if(ECSUtils::doesPassFilter(mask, Family<GreyBoxFamilyStatic>::mask)){
	GreyBoxFamilyStatic family = GreyBoxFamilyStatic(eID, getComponent<TransformComponent>(eID), getComponent<GreyBoxComponent>(eID));
	getFamilyStaticVector<GreyBoxFamilyStatic>().push_back(family);
	}
}
