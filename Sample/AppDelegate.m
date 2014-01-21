//
//  AppDelegate.m
//  Sample
//
//  Created by Anders Borch on 8/20/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"
#import <LUMOKit/LUMOKit.h>

@interface AppDelegate () {
    LKSensorManager *_sensorManager;             // Discovery and management of sensors
    LKSensor *_connectedSensor;                  // The currently connected sensor
    LKActivityStorageManager *_storageManager;   // Optional: Accessing historical activity storage
}
@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    self.viewController = [[ViewController alloc] initWithNibName:@"ViewController" bundle:nil];
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
    [self startScanningForSensors];
    return YES;
}

// Start scanning for sensors and observe changes in the sensors array
- (void)startScanningForSensors
{
    _sensorManager = [[LKSensorManager alloc] initWithAppId: <#app id#>
                                                    version: <#app version#>];
    _storageManager = [[LKActivityStorageManager alloc] initWithOwner: <#owner email#>
                                                                appId: <#app id#>
                                                              version: <#app version#>];
    [_sensorManager addObserver: self
                     forKeyPath: @"sensors"
                        options: NSKeyValueObservingOptionNew
                        context: NULL];
    [_sensorManager startScan];
}

- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context
{
    if ([keyPath isEqualToString: @"sensors"]) [self sensorsUpdated];
    if ([keyPath isEqualToString: @"currentActivity"]) [self activityUpdated];
}

- (void)observeActivities
{
    [_connectedSensor addObserver: self
                       forKeyPath: @"currentActivity"
                          options: NSKeyValueObservingOptionNew
                          context: NULL];
    // Tell the storage manager to start storing activities received from a sensor
    [_storageManager startStoringActivitiesFromSensor: _connectedSensor];
    
    // Get notified of historical activity import progress
    __weak AppDelegate *weak = self;
    _connectedSensor.importProgressBlock = ^(int remaining) {
        if (remaining <= 2) {
            [weak requestWalkTimeForToday];
        }
    };
    
    [self requestWalkTimeForToday];
}

- (void)activityUpdated
{
    // Log changes from walking to running
    if ([_connectedSensor.currentActivity isEqualToString: LKSensorCurrentActivityWalk]) {
        NSLog(@"now walking");
    }
    if ([_connectedSensor.currentActivity isEqualToString: LKSensorCurrentActivityRun]) {
        NSLog(@"now running");
    }
}

- (void)sensorsUpdated
{
    static BOOL connecting = NO;
    //NSLog(@"sensors changed!");
    if (!connecting) {
        /*
         The sensors array contains a list of discovered sensors,
         sorted by signal strength with the closest sensor first
         */
        if (_sensorManager.sensors.count) {
            connecting = YES;
            __weak AppDelegate *weak = self;
            LKSensor *sensor = _sensorManager.sensors[0];
            [sensor connectWithOwner: <#owner email#>
                            password: <#owner password#>
                     completionBlock: ^(LKSensorConnectionResult result) {
                         if (result == LKSensorConnectOwned) {
                             NSLog(@"successful connect");
                             _connectedSensor = sensor;
                             [_sensorManager storeKnownGoodPassword: <#owner password#>
                                                           forOwner: <#owner email#>];
                             // Start observing activity changes - see below
                             [weak observeActivities];
                             
                             [_sensorManager stopScan];
                         } else if (result == LKSensorConnectNew) {
                             NSLog(@"new connect");
                             _connectedSensor = sensor;
                             // Register as an owner of a sensor here - see below
                             [weak registerOwner];

                             [_sensorManager stopScan];
                         } else {
                             NSLog(@"failed connect: %d", result);
                             connecting = NO;
                         }
                     }];
        }
    }
}

- (void)requestWalkTimeForToday
{
    NSCalendar *calendar = [NSCalendar currentCalendar];
    NSDateComponents *components = [calendar components: NSYearCalendarUnit | NSMonthCalendarUnit |  NSDayCalendarUnit
                                               fromDate: [NSDate date]];
    
    [_storageManager.activityStorage aggregatedActivitiesForYear: components.year
                                                           month: components.month
                                                             day: components.day
                                                            mask: LKActivityStorageActivityMaskWalk
                                                      usingBlock: ^(LKPeriodActivities *activities, NSError *error) {
                                                          float walkTime = activities.walk * activities.duration;
                                                          NSLog(@"walked for %f hours today", walkTime / 3600.0f);
                                                      }];
}

- (void)registerOwner
{
    [_sensorManager authorizeOwnSensor: _connectedSensor
                          withPassword: <#owner password#>
                       completionBlock: ^(LKSensorManagerOwnResult result) {
                           switch (result) {
                               case LKSensorManagerOwnOK:
                                   NSLog(@"own authorized");
                                   break;
                               case LKSensorManagerOwnNetworkUnavailable:
                                   NSLog(@"network error - please try again later");
                                   break;
                               case LKSensorManagerOwnInvalidPassword:
                                   NSLog(@"invalid password");
                                   break;
                               case LKSensorManagerOwnInvalidOwnerName:
                                   NSLog(@"invalid owner name");
                                   break;
                               case LKSensorManagerOwnOwnedByOther:
                                   NSLog(@"somebody else already owns this sensor");
                                   break;
                           }
                       }];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
