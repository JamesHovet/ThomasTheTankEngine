			else if (componentIt.key() == "DebugPrintComponent"){
				addComponent<DebugPrintComponent>(eID, DebugPrintComponent::deserialize(componentIt.value()));
			}
			else if (componentIt.key() == "GreyBoxComponent"){
				addComponent<GreyBoxComponent>(eID, GreyBoxComponent::deserialize(componentIt.value()));
			}
			else if (componentIt.key() == "AABBColliderComponent"){
				addComponent<AABBColliderComponent>(eID, AABBColliderComponent::deserialize(componentIt.value()));
			}