//
//  LKSensorManager.h
//  LumoKit
//
//  Created by Anders Borch on 7/30/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <LUMOKit/LKSensor.h>

// Sensor manager states
typedef NS_ENUM(NSInteger, LKSensorManagerState) {
    LKSensorManagerStateUnknown,            // The current state of the sensor manager is unknown; an update is imminent.
    LKSensorManagerStateResetting,          // The connection with the system service was momentarily lost; an update is imminent.
    LKSensorManagerStateUnsupported,        // The platform does not support LumoKit.
    LKSensorManagerStateUnauthorized,       // The app is not authorized to use Bluetooth low energy.
    LKSensorManagerStatePoweredOff,         // Sensor manager is currently powered off.
    LKSensorManagerStatePoweredOn           // Sensor manager is currently powered on and available to use.
};

// Sensor manager asynchronous request result
typedef NS_ENUM(NSInteger, LKSensorManagerRequestResult) {
    LKSensorManagerRequestFailedTimeout,
    LKSensorManagerRequestFailedInvalidPassword,
    LKSensorManagerRequestFailedInvalidOwner,
    LKSensorManagerRequestSuccess,
};

/*
 Sensor manager error codes

 These error codes are returned as part of userInfo in exceptions thrown by the sensor manager.
 The LKLKSensorManagerErrorCodeKey key is used to reference error codes in the userInfo dictionary.
 */
typedef NS_ENUM(NSInteger, LKSensorManagerError) {
    LKSensorManagerErrorInvalidAppId = -1,             // Invalid app ID
    LKSensorManagerErrorMissingEnumerationBlock = -2,  // Nil block passed to enumeration method
    LKSensorManagerErrorInvalidSensorUUID = -3         // Unique identifier missing for sensor - connect to sensor first
};

// Possible error codes returned by authorizeOwnSensor:completionBlock:
typedef NS_ENUM(NSInteger, LKSensorManagerOwnResult) {
    LKSensorManagerOwnOK                   = 0,        // Successful own
    LKSensorManagerOwnNetworkUnavailable   = -1001,    // Network error occured
    LKSensorManagerOwnInvalidPassword      = -1002,    // Invalid password
    LKSensorManagerOwnInvalidOwnerName     = -1003,    // Owner name could not be percent escaped
    LKSensorManagerOwnOwnedByOther         = -1004     // Other owner already owns this sensor id
};

// Type of block invoked on completion of authorizeOwnSensor:completionBlock:
typedef void (^LKSensorManagerOwnCompletionBlock)(LKSensorManagerOwnResult result);

// Error code key used to index error codes in userInfo dictionaries sent with exceptions
FOUNDATION_EXPORT NSString* const LKSensorManagerErrorCodeKey;

// Block invoked when state is updated
typedef void (^LKSensorManagerStateUpdateBlock)(LKSensorManagerState state);

// Block invoked if a known sensor is discovered and automatically connected to
typedef void (^LKSensorManagerAutoconnectBlock)(LKSensor *sensor);

/*
 Block invoked with progress indication when downloading firmware from
 the cloud and when uploading firmware to the sensor.
 */
typedef void(^LKSensorManagerFirmwareProgressBlock)(double progress);

// Sensor manager
@interface LKSensorManager : NSObject

@property (nonatomic,readonly) LKSensorManagerState state;
@property (nonatomic,readonly) NSArray *sensors;

/*
 Assign a block to the sensorAutoconnectBlock property to enable autoconnect.
 This block will get invoked if a known sensor is discovered and
 successfully connected to.
 
 The tentativeSensorAutoconnectBlock block will get invoked after connection
 is made but before the owner is logged in to the sensor. This may get called 
 multiple times before login succeeds and sensorAutoconnectBlock is called.
 */
@property (nonatomic, copy) LKSensorManagerAutoconnectBlock sensorAutoconnectBlock;
@property (nonatomic, copy) LKSensorManagerAutoconnectBlock tentativeSensorAutoconnectBlock;

/*
 Initialize with an app ID.
 The app ID is supplied by LUMO BodyTech.

 The version number is a string comprised of three period-separated integers.
 The first integer represents major revisions to the application, such as
 revisions that implement new features or major changes. The second integer
 denotes revisions that implement less prominent features. The third integer
 represents maintenance releases.
 */
- (id)initWithAppId:(NSString*)appId version:(NSString*)version;

// Scanning for sensors
- (void)startScan;
- (void)stopScan;

/*
 Accessing stored owner passwords
 
 Storing last known good passwords allows the sensor manager to automatically
 connect to a sensor when it is discovered if a sensorAutoconnectBlock is specified.
 */
- (NSString*)lastKnownGoodPasswordForOwner:(NSString*)owner;
- (void)storeKnownGoodPassword:(NSString*)password forOwner:(NSString*)owner;

// Send a request to own a sensor to the cloud service
- (void)authorizeOwnSensor:(LKSensor*)sensor
              withPassword:(NSString*)password
           completionBlock:(LKSensorManagerOwnCompletionBlock)block;

/*
 Request information about the latest firmware version available on the server.
 If the latest version available on the server is newer than what is available
 locally, then download the latest version.
 
 Successful download is defined as progressBlock being called with a value of 1.0
 followed by the completion block being called.
 
 Failure is indicated by the completion block being called without first calling the
 progress block with a value of 1.0.
 */
- (void)downloadLatestFirmwareForSensor:(LKSensor*)sensor
                          progressBlock:(LKSensorManagerFirmwareProgressBlock)progressBlock
                        completionBlock:(dispatch_block_t)completionBlock;

/*
 Update firmware on sensor if any firmware is available.
 
 downloadLatestFirmwareForSensor:progressBlock:completionBlock: should be called
 before calling this method to ensure that the latest firmware is available for
 the sensor.
 
 If no new firmware is available for the sensor then the completion block is
 called immediately.
 */
- (void)updateLatestFirmwareOnSensor:(LKSensor*)sensor
                       progressBlock:(LKSensorManagerFirmwareProgressBlock)progressBlock
                     completionBlock:(dispatch_block_t)completionBlock;

@end
