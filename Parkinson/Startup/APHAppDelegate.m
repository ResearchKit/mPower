//
//  APHAppDelegate.m
//  mPower
//
// Copyright (c) 2015, Sage Bionetworks. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1.  Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2.  Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.
//
// 3.  Neither the name of the copyright holder(s) nor the names of any contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission. No license is granted to the trademarks of
// the copyright holders even if such marks are included in this software.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

@import APCAppCore;
#import "APHAppDelegate.h"
#import "APHProfileExtender.h"

static NSString *const kWalkingActivitySurveyIdentifier             = @"4-APHTimedWalking-80F09109-265A-49C6-9C5D-765E49AAF5D9";
static NSString *const kVoiceActivitySurveyIdentifier               = @"3-APHPhonation-C614A231-A7B7-4173-BDC8-098309354292";
static NSString *const kTappingActivitySurveyIdentifier             = @"2-APHIntervalTapping-7259AC18-D711-47A6-ADBD-6CFCECDED1DF";
static NSString *const kMemoryActivitySurveyIdentifier              = @"7-APHSpatialSpanMemory-4A04F3D0-AC05-11E4-AB27-0800200C9A66";
static NSString *const kMyThoughtsSurveyIdentifier                  = @"8-MyThoughts-12ffde40-1551-4b48-aae2-8fef38d61b61";

/*********************************************************************************/
#pragma mark - Initializations Options
/*********************************************************************************/
static NSString *const kStudyIdentifier             = @"studyname";
static NSString *const kAppPrefix                   = @"studyname";
static NSString* const  kConsentPropertiesFileName  = @"APHConsentSection";

static NSString *const kVideoShownKey = @"VideoShown";


static NSString *const kJsonScheduleStringKey           = @"scheduleString";
static NSString *const kJsonTasksKey                    = @"tasks";
static NSString *const kJsonScheduleTaskIDKey           = @"taskID";
static NSString *const kJsonSchedulesKey                = @"schedules";

@interface APHAppDelegate ()

@property  (nonatomic, strong)  APHProfileExtender* profileExtender;
@property  (nonatomic, assign)  NSInteger           environment;

@end

@implementation APHAppDelegate

- (BOOL)application:(UIApplication*) __unused application willFinishLaunchingWithOptions:(NSDictionary*) __unused launchOptions
{
    [super application:application willFinishLaunchingWithOptions:launchOptions];

    [self enableBackgroundDeliveryForHealthKitTypes];

    return YES;
}

- (void)enableBackgroundDeliveryForHealthKitTypes
{
    NSArray* dataTypesWithReadPermission = [self healthKitQuantityTypesToRead];
    
    if (dataTypesWithReadPermission)
    {
        for (id dataType in dataTypesWithReadPermission)
        {
            HKObjectType*   sampleType  = nil;

            if ([dataType isKindOfClass:[NSDictionary class]])
            {
                NSDictionary* categoryType = (NSDictionary*) dataType;

                //Distinguish
                if (categoryType[kHKWorkoutTypeKey])
                {
                    sampleType = [HKObjectType workoutType];
                }
                else if (categoryType[kHKCategoryTypeKey])
                {
                    sampleType = [HKObjectType categoryTypeForIdentifier:categoryType[kHKCategoryTypeKey]];
                }
            }
            else
            {
                sampleType = [HKObjectType quantityTypeForIdentifier:dataType];
            }

            if (sampleType)
            {
                [self.dataSubstrate.healthStore enableBackgroundDeliveryForType:sampleType
                                                                      frequency:HKUpdateFrequencyImmediate
                                                                 withCompletion:^(BOOL success, NSError *error)
                 {
                     if (!success)
                     {
                         if (error)
                         {
                             APCLogError2(error);
                         }
                     }
                     else
                     {
                         APCLogDebug(@"Enabling background delivery for healthkit");
                     }
                 }];
            }
        }
    }
}

- (void)setUpInitializationOptions
{
    
    NSMutableDictionary * dictionary = [super defaultInitializationOptions];
#ifdef DEBUG
    self.environment = SBBEnvironmentStaging;
#else
    self.environment = SBBEnvironmentProd;
#endif

    dictionary = [self updateOptionsFor5OrOlder:dictionary];
    [dictionary addEntriesFromDictionary:@{
                                           kStudyIdentifierKey                  : kStudyIdentifier,
                                           kAppPrefixKey                        : kAppPrefix,
                                           kBridgeEnvironmentKey                : @(self.environment),
                                           kShareMessageKey : NSLocalizedString(@"Please take a look at Parkinson mPower, a research study app about Parkinson Disease.  Download it for iPhone at http://apple.co/1FO7Bsi", nil)
                                           }];

    self.initializationOptions = dictionary;

    self.profileExtender = [[APHProfileExtender alloc] init];
}

- (NSDictionary*)researcherSpecifiedUnits
{
    NSDictionary* hkUnits =
  @{
    HKQuantityTypeIdentifierStepCount               : [HKUnit countUnit],
    HKQuantityTypeIdentifierBodyMass                : [HKUnit gramUnitWithMetricPrefix:HKMetricPrefixKilo],
    HKQuantityTypeIdentifierHeight                  : [HKUnit meterUnit],
    HKQuantityTypeIdentifierDistanceCycling         : [HKUnit meterUnit],
    HKQuantityTypeIdentifierDistanceWalkingRunning  : [HKUnit meterUnit],
    HKQuantityTypeIdentifierFlightsClimbed          : [HKUnit countUnit]
    };
    return hkUnits;
}

- (void)setUpTasksReminder
{
    APCTaskReminder *walkingActivityReminder = [[APCTaskReminder alloc] initWithTaskID:kWalkingActivitySurveyIdentifier reminderBody:NSLocalizedString(@"Walking Activity", nil)];
    APCTaskReminder *voiceActivityReminder = [[APCTaskReminder alloc] initWithTaskID:kVoiceActivitySurveyIdentifier reminderBody:NSLocalizedString(@"Voice Activity", nil)];
    APCTaskReminder *tappingActivityReminder = [[APCTaskReminder alloc] initWithTaskID:kTappingActivitySurveyIdentifier reminderBody:NSLocalizedString(@"Tapping Activity", nil)];
    APCTaskReminder *memoryActivityReminder = [[APCTaskReminder alloc] initWithTaskID:kMemoryActivitySurveyIdentifier reminderBody:NSLocalizedString(@"Memory Activity", nil)];
    APCTaskReminder *pdSurveyReminder = [[APCTaskReminder alloc] initWithTaskID:kStudyIdentifier reminderBody:NSLocalizedString(@"PD Survey", nil)];
    APCTaskReminder *myThoughtsSurveyReminder = [[APCTaskReminder alloc] initWithTaskID:kMyThoughtsSurveyIdentifier reminderBody:NSLocalizedString(@"My Thoughts", nil)];

    [self.tasksReminder.reminders removeAllObjects];
    [self.tasksReminder manageTaskReminder:walkingActivityReminder];
    [self.tasksReminder manageTaskReminder:voiceActivityReminder];
    [self.tasksReminder manageTaskReminder:tappingActivityReminder];
    [self.tasksReminder manageTaskReminder:memoryActivityReminder];
    [self.tasksReminder manageTaskReminder:pdSurveyReminder];
    [self.tasksReminder manageTaskReminder:myThoughtsSurveyReminder];

    if ([self doesPersisteStoreExist] == NO)
    {
        APCLogEvent(@"This app is being launched for the first time. Turn all reminders on");
        for (APCTaskReminder *reminder in self.tasksReminder.reminders) {
            [[NSUserDefaults standardUserDefaults] setObject:reminder.reminderBody forKey:reminder.reminderIdentifier];
        }

        if ([[UIApplication sharedApplication] currentUserNotificationSettings].types != UIUserNotificationTypeNone) {
            [self.tasksReminder setReminderOn:@YES];
        }
    }
}

- (void)setUpAppAppearance
{
    [APCAppearanceInfo setAppearanceDictionary:@{
                                                 kPrimaryAppColorKey : [UIColor colorWithRed:255 / 255.0f green:0.0 blue:56 / 255.0f alpha:1.000],
                                                 @"2-APHIntervalTapping-7259AC18-D711-47A6-ADBD-6CFCECDED1DF" : [UIColor appTertiaryPurpleColor],
                                                 @"7-APHSpatialSpanMemory-4A04F3D0-AC05-11E4-AB27-0800200C9A66" : [UIColor appTertiaryRedColor],
                                                 @"3-APHPhonation-C614A231-A7B7-4173-BDC8-098309354292" : [UIColor appTertiaryBlueColor],
                                                 @"4-APHTimedWalking-80F09109-265A-49C6-9C5D-765E49AAF5D9" : [UIColor appTertiaryYellowColor],
                                                 @"1-EnrollmentSurvey-20EF83D2-E461-4C20-9024-F43FCAAAF4C3": [UIColor lightGrayColor],
                                                 @"8-MyThoughts-12ffde40-1551-4b48-aae2-8fef38d61b61": [UIColor lightGrayColor],
                                                 @"9-Feedback-394348ce-ca4f-4abe-b97e-fedbfd7ffb8e": [UIColor lightGrayColor],
                                                 @"a-APHMedicationTracker-20EF8ED2-E461-4C20-9024-F43FCAAAF4C3": [UIColor colorWithRed:0.933
                                                                                                                               green:0.267
                                                                                                                                blue:0.380
                                                                                                                               alpha:1.000]
                                                 }];
    [[UINavigationBar appearance] setTintColor:[UIColor appPrimaryColor]];
    [[UINavigationBar appearance] setTitleTextAttributes: @{
                                                            NSForegroundColorAttributeName : [UIColor appSecondaryColor2],
                                                            NSFontAttributeName : [UIFont appNavBarTitleFont]
                                                            }];
    [[UIView appearance] setTintColor:[UIColor appPrimaryColor]];

    self.dataSubstrate.parameters.bypassServer = YES;
    self.dataSubstrate.parameters.hideExampleConsent = NO;
}

- (id <APCProfileViewControllerDelegate>)profileExtenderDelegate
{
    return self.profileExtender;
}

- (void)showOnBoarding
{
    [super showOnBoarding];

    [self showStudyOverview];
}

- (void)showStudyOverview
{
    APCStudyOverviewViewController *studyController = [[UIStoryboard storyboardWithName:@"APCOnboarding" bundle:[NSBundle appleCoreBundle]] instantiateViewControllerWithIdentifier:@"StudyOverviewVC"];
    [self setUpRootViewController:studyController];
}

- (BOOL)isVideoShown
{
    return [[NSUserDefaults standardUserDefaults] boolForKey:kVideoShownKey];
}

- (NSMutableDictionary *)updateOptionsFor5OrOlder:(NSMutableDictionary *)initializationOptions
{
    if (![APCDeviceHardware isiPhone5SOrNewer]) {
        [initializationOptions setValue:@"APHTasksAndSchedules_NoM7" forKey:kTasksAndSchedulesJSONFileNameKey];
    }
    return initializationOptions;
}

- (NSArray *)allSetTextBlocks
{
    NSArray *allSetBlockOfText = nil;

    NSString *activitiesAdditionalText = NSLocalizedString(@"Please perform the activites each day when you are at your lowest before you take your Parkinson medications, after your medications take effect, and then a third time during the day.", nil);
    allSetBlockOfText = @[@{kAllSetActivitiesTextAdditional: activitiesAdditionalText}];

    return allSetBlockOfText;
}

/*********************************************************************************/
#pragma mark - Helper Method for Datasubstrate Delegate Methods
/*********************************************************************************/

static NSDate *DetermineConsentDate(id object)
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSString      *filePath    = [[object applicationDocumentsDirectory] stringByAppendingPathComponent:@"db.sqlite"];
    NSDate        *consentDate = nil;

    if ([fileManager fileExistsAtPath:filePath]) {
        NSError      *error      = nil;
        NSDictionary *attributes = [fileManager attributesOfItemAtPath:filePath error:&error];
        
        if (error != nil) {
            APCLogError2(error);
            consentDate = [[NSDate date] startOfDay];
        } else {
            consentDate = [attributes fileCreationDate];
        }
    }
    return consentDate;
}

/*********************************************************************************/
#pragma mark - Datasubstrate Delegate Methods
/*********************************************************************************/

- (void)setUpCollectors
{
    if (self.dataSubstrate.currentUser.userConsented)
    {
        if (!self.passiveDataCollector)
        {
            self.passiveDataCollector = [[APCPassiveDataCollector alloc] init];
        }

        [self configureDisplacementTracker];
        [self configureObserverQueries];
        [self configureMotionActivityObserver];
    }
}

- (void)configureMotionActivityObserver
{
    NSString*(^CoreMotionDataSerializer)(id) = ^NSString *(id dataSample)
    {
        CMMotionActivity* motionActivitySample  = (CMMotionActivity*)dataSample;
        NSString* motionActivity                = [CMMotionActivity activityTypeName:motionActivitySample];
        NSNumber* motionConfidence              = @(motionActivitySample.confidence);
        NSString* stringToWrite                 = [NSString stringWithFormat:@"%@,%@,%@\n",
                                                   motionActivitySample.startDate.toStringInISO8601Format,
                                                   motionActivity,
                                                   motionConfidence];
        return stringToWrite;
    };

    NSDate* (^LaunchDate)() = ^
    {
        APCUser*    user        = ((APCAppDelegate *)[UIApplication sharedApplication].delegate).dataSubstrate.currentUser;
        NSDate*     consentDate = nil;

        if (user.consentSignatureDate)
        {
            consentDate = user.consentSignatureDate;
        }
        else
        {
            consentDate = DetermineConsentDate(self);
                }
        return consentDate;
    };

    APCCoreMotionBackgroundDataCollector *motionCollector = [[APCCoreMotionBackgroundDataCollector alloc] initWithIdentifier:@"motionActivityCollector"
                                                                                                              dateAnchorName:@"APCCoreMotionCollectorAnchorName"
                                                                                                            launchDateAnchor:LaunchDate];

    NSArray*            motionColumnNames   = @[@"startTime",@"activityType",@"confidence"];
    APCPassiveDataSink* receiver            = [[APCPassiveDataSink alloc] initWithIdentifier:@"motionActivityCollector"
                                                                                 columnNames:motionColumnNames
                                                                          operationQueueName:@"APCCoreMotion Activity Collector"
                                                                               dataProcessor:CoreMotionDataSerializer
                                                                           fileProtectionKey:NSFileProtectionCompleteUntilFirstUserAuthentication];
    [motionCollector setReceiver:receiver];
    [motionCollector setDelegate:receiver];
    [motionCollector start];
    [self.passiveDataCollector addDataSink:motionCollector];
}

- (void)configureDisplacementTracker
{
    APCDisplacementTrackingCollector*           locationCollector   = [[APCDisplacementTrackingCollector alloc]
                                                                       initWithIdentifier:@"locationCollector"
                                                                       deferredUpdatesTimeout:60.0 * 60.0];
    NSArray*                                    locationColumns     = @[@"timestamp",
                                                                        @"distanceFromPreviousLocation",
                                                                        @"distanceUnit",
                                                                        @"direction",
                                                                        @"directionUnit",
                                                                        @"speed",
                                                                        @"speedUnit",
                                                                        @"floor",
                                                                        @"altitude",
                                                                        @"altitudeUnit",
                                                                        @"horizontalAccuracy",
                                                                        @"horizontalAccuracyUnit",
                                                                        @"verticalAccuracy",
                                                                        @"verticalAccuracyUnit"];
    APCPassiveDisplacementTrackingDataUploader* displacementSinker  = [[APCPassiveDisplacementTrackingDataUploader alloc]
                                                                       initWithIdentifier:@"displacementCollector"
                                                                       columnNames:locationColumns
                                                                       operationQueueName:@"APCDisplacement Tracker Sink"
                                                                       dataProcessor:nil
                                                                       fileProtectionKey:NSFileProtectionCompleteUntilFirstUserAuthentication];
    [locationCollector setReceiver:displacementSinker];
    [locationCollector setDelegate:displacementSinker];
    [locationCollector start];
    [self.passiveDataCollector addDataSink:locationCollector];
}

- (void)configureObserverQueries
{
    NSDate* (^LaunchDate)() = ^
    {
        APCUser*    user        = ((APCAppDelegate *)[UIApplication sharedApplication].delegate).dataSubstrate.currentUser;
        NSDate*     consentDate = nil;

        if (user.consentSignatureDate)
        {
            consentDate = user.consentSignatureDate;
        }
        else
        {
            consentDate = DetermineConsentDate(self);
        }
        return consentDate;
    };

    NSString *(^determineQuantitySource)(NSString *) = ^(NSString  *source)
            {
        NSString  *answer = nil;
        if (source == nil) {
            answer = @"not available";
        } else if ([UIDevice.currentDevice.name isEqualToString:source] == YES) {
            if ([APCDeviceHardware platformString] != nil) {
                answer = [APCDeviceHardware platformString];
            } else {
                answer = @"iPhone";    //    theoretically should not happen
                }
                }
        return answer;
    };

    NSString*(^QuantityDataSerializer)(id, HKUnit*) = ^NSString*(id dataSample, HKUnit* unit)
    {
        HKQuantitySample*   qtySample           = (HKQuantitySample *)dataSample;
        NSString*           startDateTimeStamp  = [qtySample.startDate toStringInISO8601Format];
        NSString*           endDateTimeStamp    = [qtySample.endDate toStringInISO8601Format];
        NSString*           healthKitType       = qtySample.quantityType.identifier;
        NSString*           quantityValueRep    = [NSString stringWithFormat:@"%@", qtySample.quantity];
        NSArray*            valueSplit          = [quantityValueRep componentsSeparatedByString:@" "];
        NSNumber*           quantityValue       = @([qtySample.quantity doubleValueForUnit:unit]);
        NSString*           quantityUnit        = unit.unitString;
        NSString*           sourceIdentifier    = qtySample.source.bundleIdentifier;
        NSString*           quantitySource      = qtySample.source.name;

        quantitySource = determineQuantitySource(quantitySource);

        NSString *stringToWrite = [NSString stringWithFormat:@"%@,%@,%@,%@,%@,%@,%@\n",
                                   startDateTimeStamp,
                                   endDateTimeStamp,
                                   healthKitType,
                                   quantityValue,
                                   quantityUnit,
                                   quantitySource,
                                   sourceIdentifier];
        return stringToWrite;
    };

    NSString*(^WorkoutDataSerializer)(id) = ^(id dataSample)
    {
        HKWorkout*  sample                      = (HKWorkout*)dataSample;
        NSString*   startDateTimeStamp          = [sample.startDate toStringInISO8601Format];
        NSString*   endDateTimeStamp            = [sample.endDate toStringInISO8601Format];
        NSString*   healthKitType               = sample.sampleType.identifier;
        NSString*   activityType                = [HKWorkout apc_workoutActivityTypeStringRepresentation:(int)sample.workoutActivityType];
        double      energyConsumedValue         = [sample.totalEnergyBurned doubleValueForUnit:[HKUnit kilocalorieUnit]];
        NSString*   energyConsumed              = [NSString stringWithFormat:@"%f", energyConsumedValue];
        NSString*   energyUnit                  = [HKUnit kilocalorieUnit].description;
        double      totalDistanceConsumedValue  = [sample.totalDistance doubleValueForUnit:[HKUnit meterUnit]];
        NSString*   totalDistance               = [NSString stringWithFormat:@"%f", totalDistanceConsumedValue];
        NSString*   distanceUnit                = [HKUnit meterUnit].description;
        NSString*   sourceIdentifier            = sample.source.bundleIdentifier;
        NSString*   quantitySource              = sample.source.name;

        quantitySource = determineQuantitySource(quantitySource);

        NSError*    error                       = nil;
        NSString*   metaData                    = [NSDictionary apc_stringFromDictionary:sample.metadata error:&error];

        if (!metaData)
        {
            if (error)
            {
                APCLogError2(error);
            }

            metaData = @"";
        }

        NSString*   metaDataStringified         = [NSString stringWithFormat:@"\"%@\"", metaData];
        NSString*   stringToWrite               = [NSString stringWithFormat:@"%@,%@,%@,%@,%@,%@,%@,%@,%@,%@,%@\n",
                                                   startDateTimeStamp,
                                                   endDateTimeStamp,
                                                   healthKitType,
                                                   activityType,
                                                   totalDistance,
                                                   distanceUnit,
                                                   energyConsumed,
                                                   energyUnit,
                                                   quantitySource,
                                                   sourceIdentifier,
                                                   metaDataStringified];
        return stringToWrite;
    };

    NSString*(^CategoryDataSerializer)(id) = ^NSString*(id dataSample)
    {
        HKCategorySample*   catSample       = (HKCategorySample *)dataSample;
        NSString*           stringToWrite   = nil;

        if ([catSample.categoryType.identifier isEqualToString:@"HKCategoryTypeIdentifierSleepAnalysis"])
        {
            NSString*           startDateTime   = [catSample.startDate toStringInISO8601Format];
            NSString*           healthKitType   = catSample.sampleType.identifier;
            NSString*           categoryValue   = nil;

            if (catSample.value == HKCategoryValueSleepAnalysisAsleep)
            {
                categoryValue = @"HKCategoryValueSleepAnalysisAsleep";
            }
            else
            {
                categoryValue = @"HKCategoryValueSleepAnalysisInBed";
            }

            NSString*           quantityUnit        = [[HKUnit secondUnit] unitString];
            NSString*           sourceIdentifier    = catSample.source.bundleIdentifier;
            NSString*           quantitySource      = catSample.source.name;

            quantitySource = determineQuantitySource(quantitySource);

            // Get difference in seconds between start and end date for sample
            NSTimeInterval secondsSpentInBedOrAsleep = [catSample.endDate timeIntervalSinceDate:catSample.startDate];
            NSString*           quantityValue   = [NSString stringWithFormat:@"%ld", (long)secondsSpentInBedOrAsleep];

            stringToWrite = [NSString stringWithFormat:@"%@,%@,%@,%@,%@,%@,%@\n",
                             startDateTime,
                             healthKitType,
                             categoryValue,
                             quantityValue,
                             quantityUnit,
                             sourceIdentifier,
                             quantitySource];
        }
        return stringToWrite;
    };
    
    NSArray* dataTypesWithReadPermission = [self healthKitQuantityTypesToRead];
    
    if (!self.passiveDataCollector)
    {
        self.passiveDataCollector = [[APCPassiveDataCollector alloc] init];
    }

    // Just a note here that we are using n collectors to 1 data sink for quantity sample type data.
    NSArray*                    quantityColumnNames = @[@"startTime,endTime,type,value,unit,source,sourceIdentifier"];
    APCPassiveDataSink*         quantityreceiver    =[[APCPassiveDataSink alloc] initWithQuantityIdentifier:@"HealthKitDataCollector"
                                                                                                columnNames:quantityColumnNames
                                                                                         operationQueueName:@"APCHealthKitQuantity Activity Collector"
                                                                                              dataProcessor:QuantityDataSerializer
                                                                                          fileProtectionKey:NSFileProtectionCompleteUnlessOpen];
    NSArray*                    workoutColumnNames  = @[@"startTime,endTime,type,workoutType,total distance,unit,energy consumed,unit,source,sourceIdentifier,metadata"];
    APCPassiveDataSink*         workoutReceiver     = [[APCPassiveDataSink alloc] initWithIdentifier:@"HealthKitWorkoutCollector"
                                                                                         columnNames:workoutColumnNames
                                                                                  operationQueueName:@"APCHealthKitWorkout Activity Collector"
                                                                                       dataProcessor:WorkoutDataSerializer
                                                                                   fileProtectionKey:NSFileProtectionCompleteUnlessOpen];
    NSArray*                    categoryColumnNames = @[@"startTime,type,category value,value,unit,source,sourceIdentifier"];
    APCPassiveDataSink*         sleepReceiver       = [[APCPassiveDataSink alloc] initWithIdentifier:@"HealthKitSleepCollector"
                                                                                         columnNames:categoryColumnNames
                                                                                  operationQueueName:@"APCHealthKitSleep Activity Collector"
                                                                                       dataProcessor:CategoryDataSerializer
                                                                                   fileProtectionKey:NSFileProtectionCompleteUnlessOpen];

    if (dataTypesWithReadPermission)
    {
        for (id dataType in dataTypesWithReadPermission)
        {
            HKSampleType* sampleType = nil;

            if ([dataType isKindOfClass:[NSDictionary class]])
            {
                NSDictionary* categoryType = (NSDictionary *) dataType;

                //Distinguish
                if (categoryType[kHKWorkoutTypeKey])
                {
                    sampleType = [HKObjectType workoutType];
                }
                else if (categoryType[kHKCategoryTypeKey])
                {
                    sampleType = [HKObjectType categoryTypeForIdentifier:categoryType[kHKCategoryTypeKey]];
                }
            }
            else
            {
                sampleType = [HKObjectType quantityTypeForIdentifier:dataType];
            }

            if (sampleType)
            {
                // This is really important to remember that we are creating as many user defaults as there are healthkit permissions here.
                NSString*                               uniqueAnchorDateName    = [NSString stringWithFormat:@"APCHealthKit%@AnchorDate", dataType];
                APCHealthKitBackgroundDataCollector*    collector               = nil;
                APCPassiveDataSink*                     receiver                = nil;

                //If the HKObjectType is a HKWorkoutType then set a different receiver/data sink.
                if (([sampleType isKindOfClass:[HKWorkoutType class]]) || ([sampleType isKindOfClass:[HKCategoryType class]]))
                {
                    collector = [[APCHealthKitBackgroundDataCollector alloc] initWithIdentifier:sampleType.identifier
                                                                                     sampleType:sampleType anchorName:uniqueAnchorDateName
                                                                               launchDateAnchor:LaunchDate
                                                                                    healthStore:self.dataSubstrate.healthStore];
                    
                    receiver = [sampleType isKindOfClass:[HKWorkoutType class]] ? workoutReceiver : sleepReceiver;
                }
                else
                {
                    NSDictionary* hkUnitKeysAndValues = [self researcherSpecifiedUnits];

                    collector = [[APCHealthKitBackgroundDataCollector alloc] initWithQuantityTypeIdentifier:sampleType.identifier
                                                                                                 sampleType:sampleType anchorName:uniqueAnchorDateName
                                                                                           launchDateAnchor:LaunchDate
                                                                                                healthStore:self.dataSubstrate.healthStore
                                                                                                       unit:[hkUnitKeysAndValues objectForKey:sampleType.identifier]];
                    receiver = quantityreceiver;
                }
                [collector setReceiver:receiver];
                [collector setDelegate:receiver];

                [collector start];
                [self.passiveDataCollector addDataSink:collector];
            }
        }
    }
}

/*********************************************************************************/
#pragma mark - APCOnboardingManagerProvider Methods
/*********************************************************************************/

- (APCScene *)inclusionCriteriaSceneForOnboarding:(APCOnboarding *) __unused onboarding
{
    APCScene *scene = [APCScene new];
    scene.name = @"APHInclusionCriteriaViewController";
    scene.storyboardName = @"APHOnboarding";
    scene.bundle = [NSBundle mainBundle];

    return scene;
}

-(APCPermissionsManager * __nonnull)permissionsManager
{
    return [[APCPermissionsManager alloc] initWithHealthKitCharacteristicTypesToRead:[self healthKitCharacteristicTypesToRead]
                                                        healthKitQuantityTypesToRead:[self healthKitQuantityTypesToRead]
                                                       healthKitQuantityTypesToWrite:[self healthKitQuantityTypesToWrite]
                                                                   userInfoItemTypes:[self userInfoItemTypes]
                                                               signUpPermissionTypes:[self signUpPermissionsTypes]];
}

- (NSArray *)healthKitCharacteristicTypesToRead
{
    return @[
             HKCharacteristicTypeIdentifierBiologicalSex,
             HKCharacteristicTypeIdentifierDateOfBirth
             ];
}

- (NSArray *)healthKitQuantityTypesToWrite
{
    return @[];
}

- (NSArray *)healthKitQuantityTypesToRead
{
    return @[
             HKQuantityTypeIdentifierBodyMass,
             HKQuantityTypeIdentifierHeight,
             HKQuantityTypeIdentifierStepCount,
             HKQuantityTypeIdentifierDistanceCycling,
             HKQuantityTypeIdentifierDistanceWalkingRunning,
             HKQuantityTypeIdentifierFlightsClimbed,
             @{kHKWorkoutTypeKey  : HKWorkoutTypeIdentifier},
             @{kHKCategoryTypeKey : HKCategoryTypeIdentifierSleepAnalysis}
             ];
}

- (NSArray *)signUpPermissionsTypes
{
    return @[
             @(kAPCSignUpPermissionsTypeLocation),
             @(kAPCSignUpPermissionsTypeCoremotion),
             @(kAPCSignUpPermissionsTypeMicrophone),
             @(kAPCSignUpPermissionsTypeLocalNotifications)
             ];
}

- (NSArray *)userInfoItemTypes
{
    return  @[
              @(kAPCUserInfoItemTypeEmail),
              @(kAPCUserInfoItemTypeDateOfBirth),
              @(kAPCUserInfoItemTypeBiologicalSex),
              @(kAPCUserInfoItemTypeHeight),
              @(kAPCUserInfoItemTypeWeight),
              @(kAPCUserInfoItemTypeWakeUpTime),
              @(kAPCUserInfoItemTypeSleepTime),
              ];
}

/*********************************************************************************/
#pragma mark - Consent
/*********************************************************************************/

- (ORKTaskViewController *)consentViewController
{
    APCConsentTask*         task = [[APCConsentTask alloc] initWithIdentifier:@"Consent"
                                                           propertiesFileName:kConsentPropertiesFileName];
    ORKTaskViewController*  consentVC = [[ORKTaskViewController alloc] initWithTask:task
                                                                        taskRunUUID:[NSUUID UUID]];
    return consentVC;
}

@end
