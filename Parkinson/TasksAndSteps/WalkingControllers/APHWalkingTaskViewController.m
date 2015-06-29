// 
//  APHWalkingTaskViewController.m 
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
 
#import "APHWalkingTaskViewController.h"
#import <HealthKit/HealthKit.h>
#import <AVFoundation/AVFoundation.h>
#import "ConverterForPDScores.h"
#import <PDScores/PDScores.h>
#import "APHAppDelegate.h"

    //
    //        Step Identifiers
    //
static NSString* const  kFileResultsKey                       = @"items";
static NSString* const  kNumberOfStepsTotalOnReturn           = @"numberOfSteps";
static NSString* const  kNumberOfStepsTotalOnReturnKey        = @"numberOfSteps";
static NSString* const  kPedometerPrefixFileIdentifier        = @"pedometer";

static  NSString       *kWalkingActivityTitle                 = @"Walking Activity";

static  NSUInteger      kNumberOfStepsPerLeg                  = 20;
static  NSTimeInterval  kStandStillDuration                   = 30.0;

    //
    //    Walking Activity Step Identifier Keys
    //        depending on Research Kit not to change the Identifier Keys
    //
static  NSString * const kInformationalStepIdentifier         = @"instruction";
static  NSString * const kInstructionalStepIdentifier         = @"instruction1";
static  NSString * const kCountdownStepIdentifier             = @"countdown";
static  NSString * const kWalkingOutboundStepIdentifier       = @"walking.outbound";
static  NSString * const kWalkingReturnStepIdentifier         = @"walking.return";
static  NSString * const kWalkingRestStepIdentifier           = @"walking.rest";
static  NSString * const kConclusionStepIdentifier            = @"conclusion";

static  NSString       *kScoreForwardGainRecordsKey           = @"ScoreForwardGainRecords";
static  NSString       *kScorePostureRecordsKey               = @"ScorePostureRecords";

        NSString * const kGaitScoreKey                        = @"GaitScoreKey";

@interface APHWalkingTaskViewController  ( )

@end

@implementation APHWalkingTaskViewController

#pragma  mark  -  Initialisation

+ (ORKOrderedTask *)createTask:(APCScheduledTask *) __unused scheduledTask
{
    ORKOrderedTask  *task = [ORKOrderedTask shortWalkTaskWithIdentifier:kWalkingActivityTitle
                                                 intendedUseDescription:nil
                                                    numberOfStepsPerLeg:kNumberOfStepsPerLeg
                                                           restDuration:kStandStillDuration
                                                                options:ORKPredefinedTaskOptionNone];
    
        //
        //    replace various step titles and details with our own verbiage
        //
    [task.steps[0] setText:@"This activity measures your gait (walk) and balance, which can be affected by Parkinson disease."];
    [task.steps[0] setDetailText:@"Please do not continue if you cannot safely walk unassisted."];
    
    NSString  *titleString = [NSString stringWithFormat:@"Turn around and stand still for %.0f seconds", kStandStillDuration];
    NSString  *spokenInstructionString = [NSString stringWithFormat:@"Turn around and stand still for %.0f seconds", kStandStillDuration];
    
    [task.steps[5] setTitle:NSLocalizedString(titleString, nil)];
    [task.steps[5] setSpokenInstruction:NSLocalizedString(spokenInstructionString, nil)];
    
    [task.steps[6] setTitle:NSLocalizedString(kConclusionStepThankYouTitle, nil)];
    [task.steps[6] setText:NSLocalizedString(kConclusionStepViewDashboard, nil)];
    
        //
        //    remove the return walking step
        //
    BOOL        foundReturnStepIdentifier = NO;
    NSUInteger  indexOfReturnStep = 0;
    
    for (ORKStep *step  in  task.steps) {
        if ([step.identifier isEqualToString:kWalkingReturnStepIdentifier] == YES) {
            foundReturnStepIdentifier = YES;
            break;
        }
        indexOfReturnStep = indexOfReturnStep + 1;
    }
    NSMutableArray  *copyOfTaskSteps = [task.steps mutableCopy];
    if (foundReturnStepIdentifier == YES) {
        [copyOfTaskSteps removeObjectAtIndex:indexOfReturnStep];
    }
    task = [[ORKOrderedTask alloc] initWithIdentifier:kWalkingActivityTitle steps:copyOfTaskSteps];

    ORKOrderedTask  *replacementTask = [self modifyTaskWithPreSurveyStepIfRequired:task andTitle:(NSString *)kWalkingActivityTitle];
    return  replacementTask;
}

#pragma  mark  -  Create Dashboard Summary Results

- (NSString *)createResultSummary
{
    ORKTaskResult  *taskResults = self.result;
    
    APHWalkingTaskViewController  *weakSelf = self;
    
    self.createResultSummaryBlock = ^(NSManagedObjectContext * context) {
        BOOL  found = NO;
        NSURL  *urlGaitForward = nil;
        NSURL  *urlPosture     = nil;
        for (ORKStepResult  *stepResult  in  taskResults.results) {
            if (stepResult.results.count > 0) {
                for (id  object  in  stepResult.results) {
                    if ([object isKindOfClass:[ORKFileResult class]] == YES) {
                        ORKFileResult  *fileResult = object;
                        if ([fileResult.fileURL.absoluteString.lastPathComponent hasPrefix: @"accel_walking.outbound"]) {
                            urlGaitForward = fileResult.fileURL;
                        } else if ([fileResult.fileURL.absoluteString.lastPathComponent hasPrefix: @"accel_walking.rest"]) {
                            urlPosture = fileResult.fileURL;
                        }
                        found = YES;
                        fileResult = object;
                    }
                }
            }
        }
        
        NSArray  *forwardSteps = [ConverterForPDScores convertPostureOrGain:urlGaitForward];
        double  forwardScores = 0.0;
        if (forwardSteps != nil) {
            forwardScores = [PDScores scoreFromGaitTest: forwardSteps];
        }
        forwardScores = isnan(forwardScores) ? 0.0 : forwardScores;
        
        NSArray  *posture = [ConverterForPDScores convertPostureOrGain:urlPosture];
        double  postureScores = 0.0;
        if (posture != nil) {
            postureScores = [PDScores scoreFromPostureTest: posture];
        }
        postureScores = isnan(postureScores) ? 0.0 : postureScores;
        
        double  avgScores = (forwardScores + postureScores) / 2.0;
        
        /************/
        
        NSDictionary   *walkingResults = nil;
        ORKStepResult  *stepResult = (ORKStepResult *)[weakSelf.result resultForIdentifier:kWalkingOutboundStepIdentifier];
        
        for (ORKFileResult *fileResult in stepResult.results) {
            NSString  *fileString = [fileResult.fileURL lastPathComponent];
            NSArray   *nameComponents = [fileString componentsSeparatedByString:@"_"];
            if ([[nameComponents objectAtIndex:0] isEqualToString:kPedometerPrefixFileIdentifier]) {
                walkingResults = [weakSelf computeTotalDistanceForDashboardItem:fileResult.fileURL];
            }
        }

        /***********/
        
        NSDictionary  *summary = @{
                                   kGaitScoreKey                  : @(avgScores),
                                   kScoreForwardGainRecordsKey    : @(forwardScores),
                                   kScorePostureRecordsKey        : @(postureScores),
                                   kNumberOfStepsTotalOnReturnKey : walkingResults == nil ? @0 : [walkingResults objectForKey:kNumberOfStepsTotalOnReturn]
                                  };
        
        NSError  *error = nil;
        NSData  *data = [NSJSONSerialization dataWithJSONObject:summary options:0 error:&error];
        NSString  *contentString = nil;
        if (data == nil) {
            APCLogError2 (error);
        } else {
            contentString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
        }
        if (contentString.length > 0) {
            [APCResult updateResultSummary:contentString forTaskResult:taskResults inContext:context];
        }
    };
    return  nil;
}

#pragma  mark  -  Task View Controller Delegate Methods

- (void)taskViewController:(ORKTaskViewController *) __unused taskViewController stepViewControllerWillAppear:(ORKStepViewController *)stepViewController
{
    if ([stepViewController.step.identifier isEqualToString:kConclusionStepIdentifier]) {
        
        [[UIView appearance] setTintColor:[UIColor appTertiaryColor1]];
    }
    if ([stepViewController.step.identifier isEqualToString: kConclusionStepIdentifier]) {
        AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
        AVSpeechUtterance  *talk = [AVSpeechUtterance
                                    speechUtteranceWithString:NSLocalizedString(@"You have completed the activity.", @"You have completed the activity.")];
        AVSpeechSynthesizer  *synthesiser = [[AVSpeechSynthesizer alloc] init];
        talk.rate = 0.1;
        [synthesiser speakUtterance:talk];
    }
}

- (void)taskViewController:(ORKTaskViewController *)taskViewController didFinishWithReason:(ORKTaskViewControllerFinishReason)reason error:(NSError *)error
{
    [[UIView appearance] setTintColor: [UIColor appPrimaryColor]];

    if (reason == ORKTaskViewControllerFinishReasonFailed) {
        if (error != nil) {
            APCLogError2 (error);
        }
    }
    [super taskViewController: taskViewController didFinishWithReason: reason error: error];
}

#pragma  mark  -  View Controller Methods

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationBar.topItem.title = NSLocalizedString(kWalkingActivityTitle, nil);
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

#pragma mark - Helper methods

- (NSDictionary *)computeTotalDistanceForDashboardItem:(NSURL *)fileURL{
    
    NSDictionary  *distanceResults = [self readFileResultsFor:fileURL];
    NSArray       *locations       = [distanceResults objectForKey:kFileResultsKey];
    
    int lastTotalNumberOfSteps = (int) [[[locations lastObject] objectForKey:kNumberOfStepsTotalOnReturn] integerValue];
    
    return  @{ @"numberOfSteps" : @(lastTotalNumberOfSteps) };
}

- (NSDictionary *) readFileResultsFor:(NSURL *)fileURL {
    
    NSError*        error       = nil;
    NSString*       contents    = [NSString stringWithContentsOfURL:fileURL encoding:NSUTF8StringEncoding error:&error];
    NSDictionary*   results     = nil;
    
    APCLogError2(error);
    
    if (!error) {
        NSError  *error = nil;
        NSData   *data  = [contents dataUsingEncoding:NSUTF8StringEncoding];
        
        results = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
        
        APCLogError2(error);
    }
    return  results;
}

@end
