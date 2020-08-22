{
if(ECSUtils::doesPassFilter(mask, Family<GreyBoxFamily>::mask)){
	GreyBoxFamily family = GreyBoxFamily(eID, getComponent<TransformComponent>(eID), getComponent<GreyBoxComponent>(eID));
	getFamilyMap<GreyBoxFamily>().emplace(std::make_pair(eID, family));
	}
}
{
if(ECSUtils::doesPassFilter(mask, Family<AABBCollisionFamily>::mask)){
	AABBCollisionFamily family = AABBCollisionFamily(eID, getComponent<TransformComponent>(eID), getComponent<AABBColliderComponent>(eID));
	getFamilyMap<AABBCollisionFamily>().emplace(std::make_pair(eID, family));
	}
}
