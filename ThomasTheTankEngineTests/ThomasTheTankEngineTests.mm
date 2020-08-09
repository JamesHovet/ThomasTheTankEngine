//
//  ThomasTheTankEngineTests.m
//  ThomasTheTankEngineTests
//
//  Created by James Hovet on 7/26/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//



#include "main.h"
#include "EntityAdmin.hpp"

#import <XCTest/XCTest.h>

EntityAdmin* g_admin;

@interface ThomasTheTankEngineTests : XCTestCase

@end

@implementation ThomasTheTankEngineTests

- (void)setUp {
    g_admin = new EntityAdmin();
}

- (void)tearDown {
    delete g_admin;
}

- (void)testExample {
    g_admin->setup();
    entityID e = g_admin->createEntity();
    TransformComponent& trans = g_admin->addComponent<TransformComponent>(e);
    trans.m_position = glm::vec3(42.0, 69105.0, -3.14);
    XCTAssert(trans.m_position == g_admin->getComponent<TransformComponent>(e).m_position);
}


@end
