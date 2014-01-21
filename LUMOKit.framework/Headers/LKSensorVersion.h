//
//  LKSensorVersion.h
//  LumoKit
//
//  Created by Anders Borch on 8/16/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LKSensorVersion : NSObject

@property (nonatomic,readonly) NSUInteger major;
@property (nonatomic,readonly) NSUInteger minor;
@property (nonatomic,readonly) NSUInteger revision;

@end
