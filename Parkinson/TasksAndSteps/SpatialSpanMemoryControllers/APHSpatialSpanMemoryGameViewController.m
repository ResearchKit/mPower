// 
//  APHSpatialSpanMemoryGameViewController.m 
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
 
#import "APHSpatialSpanMemoryGameViewController.h"

static  NSString       *kTaskViewControllerTitle      = @"Memory Activity";
static  NSString       *kMemorySpanTitle              = @"Memory Activity";

    //
    //        Step Identifiers
    //
static  NSString *const kInstructionStepIdentifier    = @"instruction1";
static  NSString       *kConclusionStepIdentifier     = @"conclusion";

        NSString *const kSpatialMemoryScoreSummaryKey = @"spatialMemoryScoreSummaryKey";

static  NSInteger       kInitialSpan                  =  3;
static  NSInteger       kMinimumSpan                  =  2;
static  NSInteger       kMaximumSpan                  =  15;
static  NSTimeInterval  kPlaySpeed                    = 1.0;
static  NSInteger       kMaximumTests                 = 5;
static  NSInteger       kMaxConsecutiveFailures       = 3;
static  NSString       *kCustomTargetPluralName       = nil;
static  BOOL            kRequiresReversal             = NO;

static NSString * const kIdentifierKey              = @"identifier";
static NSString * const kStartDateKey               = @"startDate";
static NSString * const kEndDateKey                 = @"endDate";
static NSString * const kUserInfoKey                = @"userInfo";
static NSString * const kItemKey                    = @"item";

//
//    Spatial Span Memory Dictionary Keys — Summary
//
static  NSString  *const  kSpatialSpanMemorySummaryNumberOfGamesKey     = @"MemoryGameNumberOfGames";
static  NSString  *const  kSpatialSpanMemorySummaryNumberOfFailuresKey  = @"MemoryGameNumberOfFailures";
static  NSString  *const  kSpatialSpanMemorySummaryOverallScoreKey      = @"MemoryGameOverallScore";
static  NSString  *const  kSpatialSpanMemorySummaryGameRecordsKey       = @"MemoryGameGameRecords";
static  NSString  *const  kSpatialSpanMemorySummaryFilenameKey          = @"MemoryGameResults.json";
//
//    Spatial Span Memory Dictionary Keys — Touch Samples
//
static  NSString  *const  kSpatialSpanMemoryTouchSampleTimeStampKey     = @"MemoryGameTouchSampleTimestamp";
static  NSString  *const  kSpatialSpanMemoryTouchSampleTargetIndexKey   = @"MemoryGameTouchSampleTargetIndex";
static  NSString  *const  kSpatialSpanMemoryTouchSampleLocationKey      = @"MemoryGameTouchSampleLocation";
static  NSString  *const  kSpatialSpanMemoryTouchSampleIsCorrectKey     = @"MemoryGameTouchSampleIsCorrect";
//
//    Spatial Span Memory Dictionary Keys — Game Status
//
static  NSString   *const  kSpatialSpanMemoryGameStatusKey              = @"MemoryGameStatus";
static  NSString   *const  kSpatialSpanMemoryGameStatusUnknownKey       = @"MemoryGameStatusUnknown";
static  NSString   *const  kSpatialSpanMemoryGameStatusSuccessKey       = @"MemoryGameStatusSuccess";
static  NSString   *const  kSpatialSpanMemoryGameStatusFailureKey       = @"MemoryGameStatusFailure";
static  NSString   *const  kSpatialSpanMemoryGameStatusTimeoutKey       = @"MemoryGameStatusTimeout";
//
//    Spatial Span Memory Dictionary Keys — Game Records
//
static  NSString   *const  kSpatialSpanMemoryGameRecordSeedKey          = @"MemoryGameRecordSeed";
static  NSString   *const  kSpatialSpanMemoryGameRecordSequenceKey      = @"MemoryGameRecordSequence";
static  NSString   *const  kSpatialSpanMemoryGameRecordGameSizeKey      = @"MemoryGameRecordGameSize";
static  NSString   *const  kSpatialSpanMemoryGameRecordTargetRectsKey   = @"MemoryGameRecordTargetRects";
static  NSString   *const  kSpatialSpanMemoryGameRecordTouchSamplesKey  = @"MemoryGameRecordTouchSamples";
static  NSString   *const  kSpatialSpanMemoryGameRecordGameScoreKey     = @"MemoryGameRecordGameScore";

@interface APHSpatialSpanMemoryGameViewController ()

@end

@implementation APHSpatialSpanMemoryGameViewController

#pragma  mark  -  Task Creation Methods

+ (ORKOrderedTask *)createTask:(APCScheduledTask *) __unused scheduledTask
{
        ORKOrderedTask  *task = [ORKOrderedTask spatialSpanMemoryTaskWithIdentifier:kMemorySpanTitle
            intendedUseDescription:nil
            initialSpan:kInitialSpan
            minimumSpan:kMinimumSpan
            maximumSpan:kMaximumSpan
            playSpeed:kPlaySpeed
            maxTests:kMaximumTests
            maxConsecutiveFailures:kMaxConsecutiveFailures
            customTargetImage:nil
            customTargetPluralName:kCustomTargetPluralName
            requireReversal:kRequiresReversal
            options:ORKPredefinedTaskOptionNone];
    
    [task.steps[3] setTitle:NSLocalizedString(kConclusionStepThankYouTitle, nil)];
    [task.steps[3] setText:NSLocalizedString(kConclusionStepViewDashboard, nil)];

    [[UIView appearance] setTintColor:[UIColor appPrimaryColor]];
    
    ORKOrderedTask  *replacementTask = [self modifyTaskWithPreSurveyStepIfRequired:task andTitle:(NSString *)kTaskViewControllerTitle];
    return  replacementTask;
}

#pragma  mark  -  Task View Controller Delegate Methods

- (void)taskViewController:(ORKTaskViewController *) __unused taskViewController stepViewControllerWillAppear:(ORKStepViewController *)stepViewController
{
    
    if ([stepViewController.step.identifier isEqualToString:kConclusionStepIdentifier]) {
        [[UIView appearance] setTintColor:[UIColor appTertiaryColor1]];
    }
    [stepViewController.step setTitle:@"Good Job!"];
  
    if ([stepViewController.step.identifier isEqualToString:kInstructionStepIdentifier]) {
        UILabel *label = ((UILabel *)((UIView *)((UIView *)((UIView *) ((UIScrollView *)stepViewController.view.subviews[0]).subviews[0]).subviews[0]).subviews[0]).subviews[2]);
        label.text = NSLocalizedString(@"Some of the flowers will light up one at a time. "
                                       @"Tap those flowers in the same order they lit up.\n\n"
                                       @"To begin, tap Next, then watch closely.",
                                       @"Instruction text for memory activity in Parkinson");
    }
}

- (void) taskViewController: (ORKTaskViewController *) taskViewController didFinishWithReason: (ORKTaskViewControllerFinishReason)reason error: (NSError *) error
{
    [[UIView appearance] setTintColor: [UIColor appPrimaryColor]];

    if (reason == ORKTaskViewControllerFinishReasonFailed) {
        if (error != nil) {
            APCLogError2 (error);
        }
    }
    [super taskViewController: taskViewController didFinishWithReason: reason error: error];
}

#pragma mark - Results for Dashboard

- (NSString *)createResultSummary
{
    ORKTaskResult  *taskResults = self.result;
    self.createResultSummaryBlock = ^(NSManagedObjectContext * context) {
        ORKSpatialSpanMemoryResult  *memoryResults = nil;
        BOOL  found = NO;
        for (ORKStepResult  *stepResult  in  taskResults.results) {
            if (stepResult.results.count > 0) {
                for (id  object  in  stepResult.results) {
                    if ([object isKindOfClass:[ORKSpatialSpanMemoryResult class]] == YES) {
                        found = YES;
                        memoryResults = object;
                        break;
                    }
                }
                if (found == YES) {
                    break;
                }
            }
        }
        
        // Create the summary dictionary
        NSDictionary *summary = @{kSpatialMemoryScoreSummaryKey: @(memoryResults.score)};
        
        NSError  *error = nil;
        NSData  *data = [NSJSONSerialization dataWithJSONObject:summary options:0 error:&error];
        NSString  *contentString = nil;
        if (data == nil) {
            APCLogError2 (error);
        } else {
            contentString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
        }
        
        if (contentString.length > 0)
        {
            [APCResult updateResultSummary:contentString forTaskResult:taskResults inContext:context];
        }
    };
    return nil;
}

#pragma  mark  -  View Controller Methods

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationBar.topItem.title = NSLocalizedString(kTaskViewControllerTitle, nil);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*********************************************************************************/
#pragma mark - Add Task-Specific Results — Spatial Span Memory
/*********************************************************************************/

- (NSArray *)makeTouchSampleRecords:(NSArray *)touchSamples
{
    NSMutableArray  *samples = [NSMutableArray array];
    
    for (ORKSpatialSpanMemoryGameTouchSample  *sample  in  touchSamples) {
        
        NSMutableDictionary  *aTouchSample = [NSMutableDictionary dictionary];
        
        aTouchSample[kSpatialSpanMemoryTouchSampleTimeStampKey]   = @(sample.timestamp);
        aTouchSample[kSpatialSpanMemoryTouchSampleTargetIndexKey] = @(sample.targetIndex);
        aTouchSample[kSpatialSpanMemoryTouchSampleLocationKey]    = NSStringFromCGPoint(sample.location);
        aTouchSample[kSpatialSpanMemoryTouchSampleIsCorrectKey]   = @(sample.isCorrect);
        
        [samples addObject:aTouchSample];
    }
    return  samples;
}

- (NSArray *)makeTargetRectangleRecords:(NSArray *)targetRectangles
{
    NSMutableArray  *rectangles = [NSMutableArray array];
    
    for (NSValue  *value  in  targetRectangles) {
        CGRect  rectangle = [value CGRectValue];
        NSString  *stringified = NSStringFromCGRect(rectangle);
        [rectangles addObject:stringified];
    }
    return  rectangles;
}

- (void)addSpatialSpanMemoryResultsToArchive:(ORKSpatialSpanMemoryResult *)result
{
    
    NSString  *gameStatusKeys[] = { kSpatialSpanMemoryGameStatusUnknownKey, kSpatialSpanMemoryGameStatusSuccessKey, kSpatialSpanMemoryGameStatusFailureKey, kSpatialSpanMemoryGameStatusTimeoutKey };
    
    NSMutableDictionary  *memoryGameResults = [NSMutableDictionary dictionary];
    
    //
    //    ORK Result
    //
    memoryGameResults[kIdentifierKey] = result.identifier;
    memoryGameResults[kStartDateKey]  = result.startDate;
    memoryGameResults[kEndDateKey]    = result.endDate;
    //
    //    ORK ORKSpatialSpanMemoryResult
    //
    memoryGameResults[kSpatialSpanMemorySummaryNumberOfGamesKey]    = @(result.numberOfGames);
    memoryGameResults[kSpatialSpanMemorySummaryNumberOfFailuresKey] = @(result.numberOfFailures);
    memoryGameResults[kSpatialSpanMemorySummaryOverallScoreKey]     = @(result.score);
    
    memoryGameResults[kItemKey] = kSpatialSpanMemorySummaryFilenameKey;
    
    //
    //    Memory Game Records
    //
    NSMutableArray   *gameRecords = [NSMutableArray arrayWithCapacity:[result.gameRecords count]];
    
    for (ORKSpatialSpanMemoryGameRecord  *aRecord  in  result.gameRecords) {
        
        NSMutableDictionary  *aGameRecord = [NSMutableDictionary dictionary];
        
        aGameRecord[kSpatialSpanMemoryGameRecordSeedKey]      = @(aRecord.seed);
        aGameRecord[kSpatialSpanMemoryGameRecordGameSizeKey]  = @(aRecord.gameSize);
        aGameRecord[kSpatialSpanMemoryGameRecordGameScoreKey] = @(aRecord.score);
        aGameRecord[kSpatialSpanMemoryGameRecordSequenceKey]  = aRecord.sequence;
        aGameRecord[kSpatialSpanMemoryGameStatusKey]          = gameStatusKeys[aRecord.gameStatus];
        
        NSArray  *touchSamples = [self makeTouchSampleRecords:aRecord.touchSamples];
        aGameRecord[kSpatialSpanMemoryGameRecordTouchSamplesKey] = touchSamples;
        
        NSArray  *rectangles = [self makeTargetRectangleRecords:aRecord.targetRects];
        aGameRecord[kSpatialSpanMemoryGameRecordTargetRectsKey] = rectangles;
        
        [gameRecords addObject:aGameRecord];
    }
    memoryGameResults[kSpatialSpanMemorySummaryGameRecordsKey] = gameRecords;
    
    NSDictionary  *serializableData = [APCJSONSerializer serializableDictionaryFromSourceDictionary: memoryGameResults];
    [self.archive insertIntoArchive:serializableData filename:kSpatialSpanMemorySummaryFilenameKey];
}


@end
