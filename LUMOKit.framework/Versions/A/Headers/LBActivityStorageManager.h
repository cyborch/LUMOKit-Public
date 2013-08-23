//
//  LBActivityStorageManager.h
//  LumoKit
//
//  Created by Anders Borch on 8/8/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <LUMOKit/LBActivityStorage.h>

@class LBSensor;

/*
 Activity storage manager error codes
 
 These error codes are returned as part of userInfo in exceptions thrown by the user manager.
 The LBActivityStorageManagerErrorCodeKey key is used to reference error codes in the userInfo dictionary.
 */
typedef NS_ENUM(NSInteger, LBActivityStorageManagerError) {
    LBActivityStorageManagerErrorInvalidPassword    = -1,       // Invalid password
    LBActivityStorageManagerErrorInvalidAppId       = -2,       // Invalid App ID
    LBActivityStorageManagerErrorInvalidOwner       = -3        // Malformed owner or owner does not match sensor owner
};

/*
 Type of block invoked whenever a day of data has been recovered from the cloud activity storage
 The date argument is the date of the recovered data.
 */
typedef void (^LBActivityStorageManagerDataRecevoryUpdateBlock)(NSDate *date);

// Error code key used to index error codes in userInfo dictionaries sent with exceptions
FOUNDATION_EXPORT NSString* const LBActivityStorageManagerErrorCodeKey;

@interface LBActivityStorageManager : NSObject

@property (nonatomic,readonly) id<LBActivityStorage> activityStorage;

/*
 Create an instance of a activity storage manager with an owner and an app id.
 Throw an exception in case instatiating the storage manager fails.
 */
- (id)initWithOwner:(NSString*)owner appId:(NSString*)appId;

/*
 Associate a sensor with an activity storage and start storing incomming activities
 Activities 
 An exception will be thrown if the sensor owner does not match the activity storage owner.
 */
- (void)startStoringActivitiesFromSensor:(LBSensor*)sensor;

/*
 Start background recovering activities from cloud for currrent owner with given password
 Data is recovered one day at a time going backwards from the first day of locally stored data.
 */
- (void)startRecoveringActivitiesWithPassword:(NSString*)password
                              withUpdateBlock:(LBActivityStorageManagerDataRecevoryUpdateBlock)block;

@end
