//
//  Vector3f.h
//  TrenchBroom
//
//  Created by Kristian Duske on 15.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

typedef enum {
    VC_X,
    VC_Y,
    VC_Z
} EVectorComponent;

@class Vector3i;

@interface Vector3f : NSObject {
    @private
	float x;
	float y;
	float z;
}

+ (Vector3f *)nullVector;
+ (Vector3f *)xAxisPos;
+ (Vector3f *)xAxisNeg;
+ (Vector3f *)yAxisPos;
+ (Vector3f *)yAxisNeg;
+ (Vector3f *)zAxisPos;
+ (Vector3f *)zAxisNeg;

- (id)initWithFloatVector:(Vector3f *)vector;
- (id)initWithIntVector:(Vector3i *)vector;
- (id)initWithX:(float)xCoord y:(float)yCoord z:(float)zCoord;

- (float)x;
- (float)y;
- (float)z;

- (float)component:(EVectorComponent)index;
- (void)setComponent:(EVectorComponent)index value:(float)value;
- (EVectorComponent)largestComponent;

- (void)setX:(float)xCoord;
- (void)setY:(float)yCoord;
- (void)setZ:(float)zCoord;

- (void)setFloat:(Vector3f *)vector;
- (void)setInt:(Vector3i *)vector;

- (BOOL)isNull;

- (void)add:(Vector3f *)addend;
- (void)addX:(float)xAddend y:(float)yAddend z:(float)zAddend;

- (void)sub:(Vector3f *)subtrahend;
- (void)subX:(float)xSubtrahend y:(float)ySubtrahend z:(float)zSubtrahend;

- (void)cross:(Vector3f *)m;
- (float)dot:(Vector3f *)m;
- (void)scale:(float)f;
- (void)normalize;
- (float)length;
- (float)lengthSquared;

- (NSComparisonResult)compareToVector:(Vector3f *)vector;
- (BOOL)isEqualToVector:(Vector3f *)vector;

@end
