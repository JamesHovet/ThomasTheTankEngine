//
//  IntersectionTests.m
//  ThomasTheTankEngineTests
//
//  Created by James Hovet on 8/10/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#import <XCTest/XCTest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "IntersectionUtils.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define DIR_V(X, Y, Z) glm::vec4(X, Y, Z, 0.0f)
#define POS_V(X, Y, Z) glm::vec4(X, Y, Z, 1.0f)

@interface IntersectionTests : XCTestCase

@end

@implementation IntersectionTests
using v3 = glm::vec3;
using v4 = glm::vec4;
AABB defaultBox = {v3(0.5, 0.5, 0.5), v3(-0.5, -0.5, -0.5)};
Cylinder defaultCyl = {POS_V(0.0, -1.0, 0.0), POS_V(0.0, 1.0, 0.0), 1.0f};
glm::mat4 identity = glm::mat4(1.0f);
glm::mat4 scale311Trans314 = glm::translate(glm::scale(identity, v3(3.0, 1.0, 1.0)), v3(3.0, 1.0, 4.0));


- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testRayAABBAbsolute {
    // From +x to -x
        XCTAssert( Intersection::RayAABBAbsolute({DIR_V( 2.0,  0.0,  0.0),  DIR_V(-1.0,  0.0,  0.0)}, defaultBox));
        XCTAssert(!Intersection::RayAABBAbsolute({DIR_V( 2.0,  0.0,  0.0),  DIR_V( 1.0,  0.0,  0.0)}, defaultBox));
        // From -x to +x
        XCTAssert( Intersection::RayAABBAbsolute({DIR_V(-2.0,  0.0,  0.0),  DIR_V( 1.0,  0.0,  0.0)}, defaultBox));
        XCTAssert(!Intersection::RayAABBAbsolute({DIR_V(-2.0,  0.0,  0.0),  DIR_V(-1.0,  0.0,  0.0)}, defaultBox));
        // From +y to -y
        XCTAssert( Intersection::RayAABBAbsolute({DIR_V( 0.0,  2.0,  0.0),  DIR_V( 0.0, -1.0,  0.0)}, defaultBox));
        XCTAssert(!Intersection::RayAABBAbsolute({DIR_V( 0.0,  2.0,  0.0),  DIR_V( 0.0,  1.0,  0.0)}, defaultBox));
        // From -y to +y
        XCTAssert( Intersection::RayAABBAbsolute({DIR_V( 0.0, -2.0,  0.0),  DIR_V( 0.0,  1.0,  0.0)}, defaultBox));
        XCTAssert(!Intersection::RayAABBAbsolute({DIR_V( 0.0, -2.0,  0.0),  DIR_V( 0.0, -1.0,  0.0)}, defaultBox));
        // From +z to -z
        XCTAssert( Intersection::RayAABBAbsolute({DIR_V( 0.0,  0.0,  2.0),  DIR_V( 0.0,  0.0, -1.0)}, defaultBox));
        XCTAssert(!Intersection::RayAABBAbsolute({DIR_V( 0.0,  0.0,  2.0),  DIR_V( 0.0,  0.0,  1.0)}, defaultBox));
        // From -z to +z
        XCTAssert( Intersection::RayAABBAbsolute({DIR_V( 0.0,  0.0, -2.0),  DIR_V( 0.0,  0.0,  1.0)}, defaultBox));
        XCTAssert(!Intersection::RayAABBAbsolute({DIR_V( 0.0,  0.0, -2.0),  DIR_V( 0.0,  0.0,  -1.0)}, defaultBox));
        // Internal
        XCTAssert(Intersection::RayAABBAbsolute({DIR_V( 0.0,  0.0,  0.0),  DIR_V(-1.0,  0.0,  0.0)}, defaultBox));
        // Angled
        XCTAssert(Intersection::RayAABBAbsolute({DIR_V( 1.0,  0.0,  0.0),  DIR_V(-1.0,  0.25, 0.25)}, defaultBox));
    }

    - (void)testRayAABB {
        // identity
        XCTAssert( Intersection::RayAABB({DIR_V(2.0, 0.0, 0.0), DIR_V(-1.0, 0.0, 0.0)}, defaultBox, identity));
        // translate out of the way
        XCTAssert(!Intersection::RayAABB({DIR_V(2.0, 0.0, 0.0), DIR_V(-1.0, 0.0, 0.0)}, defaultBox, glm::translate(identity, v3(0.0, 2.0, 0.0))));
        // scale into the way
        XCTAssert( Intersection::RayAABB({DIR_V(2.0, 2.0, 0.0), DIR_V(-1.0, 0.0, 0.0)}, defaultBox, glm::scale(identity, v3(1.0, 8.0, 1.0))));
        // scale and translate into the way
        XCTAssert( Intersection::RayAABB({DIR_V(2.0, 2.0, 2.0), DIR_V(-1.0, 0.0, 0.0)}, defaultBox, glm::scale(glm::translate(identity, v3(0.0, 0.0, 2.0)), v3(1.0, 8.0, 1.0))));
    }
        
    - (void)testRayOBB {
        // identity
        XCTAssert( Intersection::RayOBB({DIR_V( 2.0,  0.0,  0.0),  DIR_V(-1.0,  0.0,  0.0)}, defaultBox, identity));
        XCTAssert(!Intersection::RayOBB({DIR_V( 2.0,  0.0,  0.0),  DIR_V( 1.0,  0.0,  0.0)}, defaultBox, identity));
        // rotate into
        XCTAssert( Intersection::RayOBB({DIR_V(2.0, 0.0, .5 + .1), DIR_V(-1.0, 0.0, 0.0)}, defaultBox, glm::rotate(identity, glm::degrees(45.0f), v3(1.0, 0.0, 0.0))));
        XCTAssert(!Intersection::RayOBB({DIR_V(2.0, 0.0, .5 + .1), DIR_V(-1.0, 0.0, 0.0)}, defaultBox, identity));
        // rotate and scale into
        XCTAssert( Intersection::RayOBB({DIR_V(2.0, 0.0, 1 + .1), DIR_V(-1.0, 0.0, 0.0)}, defaultBox, glm::rotate(glm::scale(identity, v3(2.0, 2.0, 2.0)), glm::degrees(45.0f), v3(1.0, 0.0, 0.0))));
        XCTAssert(!Intersection::RayOBB({DIR_V(2.0, 0.0, 1 + .1), DIR_V(-1.0, 0.0, 0.0)}, defaultBox, identity));
    }

    - (void)testRayCyl {
        // from X
        XCTAssert( Intersection::RayCylAbsolute({DIR_V(2.0, 0.0, 0.0), DIR_V(-2.0, 0.0, 0.0)}, defaultCyl));
        // !from X
        XCTAssert(!Intersection::RayCylAbsolute({DIR_V(2.0, 0.0, 0.0), DIR_V( 2.0, 0.0, 0.0)}, defaultCyl));
        
        // from Y
        XCTAssert( Intersection::RayCylAbsolute({DIR_V(0.0f, 2.0f, 0.0f), DIR_V(0.00f, -2.0f, 0.0f)}, defaultCyl));
        // !from Y
        XCTAssert(!Intersection::RayCylAbsolute({DIR_V(0.0f, 2.0f, 0.0f), DIR_V(0.00f,  2.0f, 0.0f)}, defaultCyl));
        
        // from Z
        XCTAssert( Intersection::RayCylAbsolute({DIR_V(0.0, 0.0, 2.0), DIR_V(0.0, 0.0, -2.0)}, defaultCyl));
        // !from Z
        XCTAssert(!Intersection::RayCylAbsolute({DIR_V(0.0, 0.0, 2.0), DIR_V(0.0, 0.0,  2.0)}, defaultCyl));
        
        // Move out of the way
        XCTAssert(!Intersection::RayCyl({DIR_V(2.0, 0.0, 0.0), DIR_V(-2.0, 0.0, 0.0)}, defaultCyl, glm::translate(v3(5.0f))));
        // Move into the way
        XCTAssert( Intersection::RayCyl({DIR_V(2.0, 0.0, 0.0), DIR_V(-2.0, 0.0, 0.0)}, {DIR_V(-5.0, -6.0, -5.0), DIR_V(-5.0, -4.0, -5.0), 1.0f}, glm::translate(v3(5.0f))));
        
//        Rotate into the way
        XCTAssert(!Intersection::RayCyl({DIR_V(2.0, 2.0, 0.0), DIR_V(-2.0, 0.0, 0.0)}, {POS_V(0.0, 0.0, 0.0), POS_V(0.0, 1.0f, 0.0), 5.0f}, glm::mat4(1.0f)));
        XCTAssert( Intersection::RayCyl({DIR_V(2.0, 2.0, 0.0), DIR_V(-2.0, 0.0, 0.0)}, {POS_V(0.0, 0.0, 0.0), POS_V(0.0, 1.0f, 0.0), 5.0f}, glm::rotate(90.0f, v3(0.0f, 0.0f, 1.0f))));
    }

@end
