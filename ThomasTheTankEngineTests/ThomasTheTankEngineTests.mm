//
//  ThomasTheTankEngineTests.m
//  ThomasTheTankEngineTests
//
//  Created by James Hovet on 7/26/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include "main.h"
#include "EntityAdmin.hpp"
#include "TransformComponent.hpp"
#include "DebugNameComponent.hpp"
#pragma clang diagnostic pop

#import <XCTest/XCTest.h>

EntityAdmin* g_admin;
using json = nlohmann::json;

@interface ThomasTheTankEngineTests : XCTestCase

@end

@implementation ThomasTheTankEngineTests

- (void)setUp {
    g_admin = new EntityAdmin();
    g_admin->setup();
}

- (void)tearDown {
    g_admin->teardown();
    delete g_admin;
}

- (void)testCreateEntity {
    XCTAssert(g_admin->tryGetEntity(0) == nullptr);
    entityID eID = g_admin->createEntity();
    XCTAssert(g_admin->tryGetEntity(eID) != nullptr);
}

- (void)testCreateEntityWithID {
    entityID eID = 42;
    g_admin->tryCreateEntity(42);
    XCTAssert(g_admin->tryGetEntity(42) != nullptr);
    XCTAssert(g_admin->tryCreateEntity(42) == nullptr);
}

- (void)testDestroyEntity {
    entityID eID = g_admin->createEntity();
    XCTAssert(g_admin->tryGetEntity(eID) != nullptr);
    g_admin->destroyEntity(eID);
    XCTAssert(g_admin->tryGetEntity(eID) == nullptr);
}

- (void)testDestroyEntityWithComponent {
    entityID eID = g_admin->createEntity();
    g_admin->addComponent<TransformComponent>(eID);
    XCTAssert(g_admin->tryGetEntity(eID) != nullptr);
    XCTAssert(g_admin->tryGetComponent<TransformComponent>(eID) != nullptr);
    g_admin->destroyEntity(eID);
    XCTAssert(g_admin->tryGetEntity(eID) == nullptr);
    XCTAssert(g_admin->tryGetComponent<TransformComponent>(eID) == nullptr);
}

- (void)testDestroyAllEntities {
    for(int i = 0; i < 3; i++){
        g_admin->tryCreateEntity(i);
        g_admin->addComponent<TransformComponent>(i);
    }
    for(int i = 0; i < 3; i++){
        XCTAssert(g_admin->tryGetEntity(i) != nullptr);
        XCTAssert(g_admin->tryGetComponent<TransformComponent>(i) != nullptr);
    }
    g_admin->destroyAllEntities();
    for(int i = 0; i < 3; i++){
        XCTAssert(g_admin->tryGetEntity(i) == nullptr);
        XCTAssert(g_admin->tryGetComponent<TransformComponent>(i) == nullptr);
    }
}

- (void)testAddComponent {
    entityID eID = g_admin->createEntity();
    TransformComponent& trans = g_admin->addComponent<TransformComponent>(eID);
    trans.m_position = glm::vec3(42.0, 69105.0, -3.14);
    XCTAssert(trans.m_position == g_admin->getComponent<TransformComponent>(eID).m_position);
}

- (void)testAddComponentFromCopy {
    TransformComponent transform;
    transform.m_position = glm::vec3(42.0, 69105.0, -3.14);
    entityID eID = g_admin->createEntity();
    TransformComponent& created = g_admin->addComponent<TransformComponent>(eID, transform);
    
    XCTAssert(created.m_position == transform.m_position);
}

- (void)testRemoveComponent {
    entityID eID = g_admin->createEntity();
    g_admin->addComponent<TransformComponent>(eID);
    g_admin->removeComponent<TransformComponent>(eID);
    XCTAssert(g_admin->tryGetComponent<TransformComponent>(eID) == nullptr);
}

- (void)testComponentIter {
    entityID eID = g_admin->createEntity();
    ECSComponent* c1 = &g_admin->addComponent<TransformComponent>(eID);
    ECSComponent* c2 = &g_admin->addComponent<DebugNameComponent>(eID);
    
    int count = 0;
    
    for(auto it = g_admin->componentsBegin(eID); it != g_admin->componentsEnd(eID); ++it){
        XCTAssert(*it == c1 or *it == c2);
        count++;
    }
    XCTAssert(count == 2);
    
}

- (void)testFilterFamily {
    entityID eID = g_admin->createEntity();
    CameraComponent& cam = g_admin->addComponent<CameraComponent>(eID);
    TransformComponent& trans = g_admin->addComponent<TransformComponent>(eID);
    g_admin->filterIfNeeded();
    auto cameraFamilyMap = g_admin->getFamilyMap<CameraFamily>();
    XCTAssert(cameraFamilyMap.size() == 1);
    XCTAssert(cameraFamilyMap.count(eID) == 1);
    CameraFamily f = cameraFamilyMap.at(eID);
    XCTAssert(&cam == &f.m_CameraComponent);
    XCTAssert(&trans == &f.m_TransformComponent);
}

- (void)testFilterFamilyStatic {
    entityID eID = g_admin->createEntity();
    CameraComponent& cam = g_admin->addComponent<CameraComponent>(eID);
    TransformComponent& trans = g_admin->addComponent<TransformComponent>(eID);
    g_admin->copyToRenderBuffer();
    auto cameraFamilyStaticVector = g_admin->getFamilyStaticVector<CameraFamilyStatic>();
    XCTAssert(cameraFamilyStaticVector.size() == 1);
    auto item = cameraFamilyStaticVector.at(0);
    XCTAssert(item.m_TransformComponent.serialize() == trans.serialize());
    XCTAssert(item.m_CameraComponent.serialize() == cam.serialize());
}

- (void)testSerializeByEntityInternal {
    for(int i = 0; i < 3; i++){
        g_admin->tryCreateEntity(i);
        g_admin->addComponent<TransformComponent>(i);
        DebugNameComponent& name = g_admin->addComponent<DebugNameComponent>(i);
        name.m_name = std::to_string(i);
    }
    
    json::object_t world_obj = g_admin->serializeByEntityInternal();
    
    EntityAdmin* new_world = new EntityAdmin();
    
    new_world->deserializeByEntityInternal(world_obj);
    
    for(int i = 0; i < 3; i++){
        XCTAssert(new_world->tryGetEntity(i) != nullptr);
        XCTAssert(new_world->tryGetComponent<TransformComponent>(i) != nullptr);
        XCTAssert(new_world->tryGetComponent<DebugNameComponent>(i) != nullptr);
        XCTAssert(new_world->tryGetComponent<TransformComponent>(i)->m_position == g_admin->tryGetComponent<TransformComponent>(i)->m_position);
        XCTAssert(new_world->tryGetComponent<DebugNameComponent>(i)->m_name     == g_admin->tryGetComponent<DebugNameComponent>(i)->m_name);
    }
    
    delete new_world;
}

@end

//- (void)testPerformanceExample {
//    // This is an example of a performance test case.
//    [self measureBlock:^{
//        // Put the code you want to measure the time of here.
//    }];
//}
