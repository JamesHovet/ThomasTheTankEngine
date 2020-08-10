//
//  TypeSerializationTests.m
//  ThomasTheTankEngineTests
//
//  Created by James Hovet on 8/10/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "SerializationUtils.hpp"

@interface TypeSerializationTests : XCTestCase

@end

@implementation TypeSerializationTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testSerializeVec3 {
    glm::vec3 cases[] = {
        glm::vec3(4.2, 69105, -3.14),
        glm::vec3(0.0, 0.0, 0.0),
    };
    
    for(glm::vec3 testcase : cases){
        XCTAssert(testcase == SerializationUtils::deserializeVec3(SerializationUtils::serializeVec3(testcase)));
    }
}

- (void)testSerializeVec4 {
    glm::vec4 cases[] = {
        glm::vec4(4.2, 69105, -3.14, 6.283185307),
        glm::vec4(0.0, 0.0, 0.0, 0.0),
    };
    
    for(glm::vec4 testcase : cases){
        XCTAssert(testcase == SerializationUtils::deserializeVec4(SerializationUtils::serializeVec4(testcase)));
    }
}

- (void)testSerializeQuat {
    glm::quat cases[] = {
        glm::quat(4.2, 69105, -3.14, 6.283185307),
        glm::quat(0.0, 0.0, 0.0, 0.0),
    };
    
    for(glm::quat testcase : cases){
        XCTAssert(testcase == SerializationUtils::deserializeQuat(SerializationUtils::serializeQuat(testcase)));
    }
}

- (void)testSerialzeAABB {
    AABB cases[] = {
        {glm::vec3(0.5, 0.5, 0.5), glm::vec3(-0.5, -0.5, -0.5)},
        {glm::vec3(4.2, 69105, -3.14), glm::vec3(-69105, -3.14, -4.2)}
    };
    
    for(AABB testcase : cases){
        XCTAssert(testcase.min == SerializationUtils::deserializeAABB(SerializationUtils::serializeAABB(testcase)).min);
        XCTAssert(testcase.max == SerializationUtils::deserializeAABB(SerializationUtils::serializeAABB(testcase)).max);
    }
}
@end
