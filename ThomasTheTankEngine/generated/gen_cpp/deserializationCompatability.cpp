			else if (componentIt.key() == "BasicModelComponent"){
				addComponent<BasicModelComponent>(eID, BasicModelComponent::deserialize(componentIt.value()));
			}
			else if (componentIt.key() == "GreyBoxComponent"){
				addComponent<GreyBoxComponent>(eID, GreyBoxComponent::deserialize(componentIt.value()));
			}
			else if (componentIt.key() == "AABBColliderComponent"){
				addComponent<AABBColliderComponent>(eID, AABBColliderComponent::deserialize(componentIt.value()));
			}
