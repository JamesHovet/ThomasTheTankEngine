{
if(ECSUtils::doesPassFilter(mask, Family<GreyBoxFamilyStatic>::mask)){
	GreyBoxFamilyStatic family = GreyBoxFamilyStatic(eID, getComponent<TransformComponent>(eID), getComponent<GreyBoxComponent>(eID));
	getFamilyStaticVector<GreyBoxFamilyStatic>().push_back(family);
	}
}
{
if(ECSUtils::doesPassFilter(mask, Family<AABBCollisionFamilyStatic>::mask)){
	AABBCollisionFamilyStatic family = AABBCollisionFamilyStatic(eID, getComponent<TransformComponent>(eID), getComponent<AABBColliderComponent>(eID));
	getFamilyStaticVector<AABBCollisionFamilyStatic>().push_back(family);
	}
}
