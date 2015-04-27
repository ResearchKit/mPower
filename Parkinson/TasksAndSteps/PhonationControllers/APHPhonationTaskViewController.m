// 
//  APHPhonationTaskViewController.m 
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
 
#import "APHPhonationTaskViewController.h"
#import <AVFoundation/AVFoundation.h>
#import <APCAppCore/APCAppCore.h>
#import "PDScores.h"
#import "APHIntervalTappingRecorderDataKeys.h"
#import "APHAppDelegate.h"

static NSString *const kTaskName                              = @"Voice";

    //
    //        Step Identifiers
    //
static  NSString *const kInstructionStepIdentifier            = @"instruction";
static  NSString *const kInstruction1StepIdentifier           = @"instruction1";
static  NSString *const kCountdownStepIdentifier              = @"countdown";
static  NSString *const kAudioStepIdentifier                  = @"audio";
static  NSString *const kConclusionStepIdentifier             = @"conclusion";

static NSString *const kMomentInDayStepIdentifier             = @"momentInDay";

static NSString *const kMomentInDayFormat                     = @"momentInDayFormat";

static NSString *const kMomentInDayFormatTitle                = @"We would like to understand how your performance on"
                                                                 " this activity could be affected by the timing of your medication.";

static NSString *const kMomentInDayFormatItemText             = @"When are you performing this Activity?";
static NSString *const kMomentInDayFormatChoiceJustWokeUp     = @"Immediately before Parkinson medication";
static NSString *const kMomentInDayFormatChoiceTookMyMedicine = @"Just after Parkinson medication (at your best)";
static NSString *const kMomentInDayFormatChoiceEvening        = @"Another time";
static NSString *const kMomentInDayFormatChoiceNone           = @"I don't take Parkinson medications";

static NSString *      kEnableMicrophoneMessage               = @"You need to enable access to microphone.";

static double kMinimumAmountOfTimeToShowSurvey                = 20.0 * 60.0;

static  NSString       *kTaskViewControllerTitle              = @"Voice Activity";

static  NSTimeInterval  kGetSoundingAaahhhInterval            = 10.0;

@interface APHPhonationTaskViewController ( )  <ORKTaskViewControllerDelegate>

@end

@implementation APHPhonationTaskViewController

#pragma  mark  -  Initialisation

+ (ORKOrderedTask *)createTask:(APCScheduledTask *) __unused scheduledTask
{
    NSDictionary  *audioSettings = @{ AVFormatIDKey         : @(kAudioFormatAppleLossless),
                                      AVNumberOfChannelsKey : @(1),
                                      AVSampleRateKey       : @(44100.0)
                                      };
    
    ORKOrderedTask  *task = [ORKOrderedTask audioTaskWithIdentifier:kTaskViewControllerTitle
                                             intendedUseDescription:nil
                                                  speechInstruction:nil
                                             shortSpeechInstruction:nil
                                                           duration:kGetSoundingAaahhhInterval
                                                  recordingSettings:audioSettings
                                                            options:0];
    
    //  Adjust apperance and text for the task
    [[UIView appearance] setTintColor:[UIColor appPrimaryColor]];
        //
        //    set up initial steps, which may have an extra step injected
        //    after the first if the user needs to say where they are in
        //    their medication schedule
        //
    [task.steps[0] setTitle:NSLocalizedString(kTaskName, nil)];

    [task.steps[1] setTitle:NSLocalizedString(kTaskName, nil)];
    [task.steps[1] setText:NSLocalizedString(@"Take a deep breath and say “Aaaaah” into the microphone for as long as you can. Keep a steady volume so the audio bars remain blue.", nil)];
    [task.steps[1] setDetailText:NSLocalizedString(@"Tap Next to begin the test.", nil)];

    [task.steps[4] setTitle:NSLocalizedString(@"Thank You!", nil)];
    [task.steps[4] setText:NSLocalizedString(@"The results of this activity can be viewed on the dashboard", nil)];

    APHAppDelegate *appDelegate = (APHAppDelegate *) [UIApplication sharedApplication].delegate;
    NSDate *lastCompletionDate = appDelegate.dataSubstrate.currentUser.taskCompletion;
    NSTimeInterval numberOfSecondsSinceTaskCompletion = [[NSDate date] timeIntervalSinceDate: lastCompletionDate];
    
    if (numberOfSecondsSinceTaskCompletion > kMinimumAmountOfTimeToShowSurvey || lastCompletionDate == nil) {
        
        NSMutableArray *stepQuestions = [NSMutableArray array];
        
        ORKFormStep *step = [[ORKFormStep alloc] initWithIdentifier:kMomentInDayStepIdentifier title:nil text:NSLocalizedString(kMomentInDayFormatTitle, nil)];
        step.optional = NO;
        {
            NSArray *choices = @[
                                 NSLocalizedString(kMomentInDayFormatChoiceJustWokeUp,
                                                   kMomentInDayFormatChoiceJustWokeUp),
                                 NSLocalizedString(kMomentInDayFormatChoiceTookMyMedicine,
                                                   kMomentInDayFormatChoiceTookMyMedicine),
                                 NSLocalizedString(kMomentInDayFormatChoiceEvening,
                                                   kMomentInDayFormatChoiceEvening),
                                 NSLocalizedString(kMomentInDayFormatChoiceNone,
                                                   kMomentInDayFormatChoiceNone)
                                 ];
            
            ORKAnswerFormat *format = [ORKTextChoiceAnswerFormat choiceAnswerFormatWithStyle:ORKChoiceAnswerStyleSingleChoice
                                                                                 textChoices:choices];
            
            ORKFormItem *item = [[ORKFormItem alloc] initWithIdentifier:kMomentInDayFormat
                                                                   text:NSLocalizedString(kMomentInDayFormatItemText, kMomentInDayFormatItemText)
                                                           answerFormat:format];
            [stepQuestions addObject:item];
        }
        [step setFormItems:stepQuestions];
        
        NSMutableArray  *phonationSteps = [task.steps mutableCopy];
        if ([phonationSteps count] >= 1) {
            [phonationSteps insertObject:step atIndex:1];
        }
        
        task = [[ORKOrderedTask alloc] initWithIdentifier:kTaskViewControllerTitle steps:phonationSteps];
    }
    return  task;
}

#pragma  mark  -  Task View Controller Delegate Methods

- (void)taskViewController:(ORKTaskViewController *) __unused taskViewController stepViewControllerWillAppear:(ORKStepViewController *)stepViewController
{
    ORKStep  *step = stepViewController.step;
    
    if ([step.identifier isEqualToString: kAudioStepIdentifier])
    {
        [[UIView appearance] setTintColor:[UIColor appTertiaryBlueColor]];
    }
    else if ([step.identifier isEqualToString: kConclusionStepIdentifier]) {
        [[UIView appearance] setTintColor:[UIColor appTertiaryColor1]];
    } else {
        [[UIView appearance] setTintColor:[UIColor appPrimaryColor]];
    }
}

- (void) taskViewController: (ORKTaskViewController *) taskViewController
        didFinishWithReason: (ORKTaskViewControllerFinishReason)reason
                      error: (NSError *) error
{
    [[UIView appearance] setTintColor: [UIColor appPrimaryColor]];
    
    if (reason  == ORKTaskViewControllerFinishReasonFailed && error != nil)
    {
        APCLogError2 (error);
    } else if (reason  == ORKTaskViewControllerFinishReasonDiscarded) {
    } else if (reason  == ORKTaskViewControllerFinishReasonCompleted) {
        APHAppDelegate *appDelegate = (APHAppDelegate *) [UIApplication sharedApplication].delegate;
        appDelegate.dataSubstrate.currentUser.taskCompletion = [NSDate date];
        [[UIView appearance] setTintColor:[UIColor appPrimaryColor]];
    }
    
    [super taskViewController: taskViewController
          didFinishWithReason: reason
                        error: error];
}

#pragma  mark  -  Results For Dashboard

- (NSString *)createResultSummary
{
    ORKTaskResult  *taskResults = self.result;
    self.createResultSummaryBlock = ^(NSManagedObjectContext * context) {
        
        ORKFileResult  *fileResult = nil;
        BOOL  found = NO;
        for (ORKStepResult  *stepResult  in  taskResults.results) {
            if (stepResult.results.count > 0) {
                for (id  object  in  stepResult.results) {
                    if ([object isKindOfClass:[ORKFileResult class]] == YES) {
                        found = YES;
                        fileResult = object;
                        break;
                    }
                }
                if (found == YES) {
                    break;
                }
            }
        }
        
        double scoreSummary = [PDScores scoreFromPhonationTest: fileResult.fileURL];
        scoreSummary = isnan(scoreSummary) ? 0 : scoreSummary;
        
        NSDictionary  *summary = @{kScoreSummaryOfRecordsKey : @(scoreSummary)};
        
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

#pragma  mark  - View Controller methods

- (void)willResignActiveNotificationWasReceived:(NSNotification *) __unused notification
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    if ([self.delegate respondsToSelector:@selector(taskViewController:didFinishWithReason:error:)] == YES) {
        [self.delegate taskViewController:self didFinishWithReason:ORKTaskViewControllerFinishReasonDiscarded error:NULL];
    }
}

#pragma  mark  - View Controller methods

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationBar.topItem.title = NSLocalizedString(kTaskViewControllerTitle, nil);
   
   // Once you give Audio permission to the application. Your app will not show permission prompt again.
    [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL granted) {
        if (granted) {
            // Microphone enabled
        }
        else {
            // Microphone disabled
            //Inform the user that they will to enable the Microphone
            UIAlertController * alert = [UIAlertController simpleAlertWithTitle:NSLocalizedString(kEnableMicrophoneMessage, nil) message:nil];
            [self presentViewController:alert animated:YES completion:nil];
        }
    }];
   
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    NSNotificationCenter  *centre = [NSNotificationCenter defaultCenter];
    [centre addObserver:self selector:@selector(willResignActiveNotificationWasReceived:) name:UIApplicationWillResignActiveNotification object:nil];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end
