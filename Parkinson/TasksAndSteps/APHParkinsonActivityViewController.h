//
//  APHParkinsonActivityViewController.h
//  mPower
//
//  Copyright (c) 2015 Apple, Inc. All rights reserved.
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


#import <APCAppCore/APCAppCore.h>

#import <Foundation/Foundation.h>

#import <ResearchKit/ResearchKit.h>

    //
    //    keys for the extra step ('Pre-Survey') that;'s injected
    //        into Parkinson Activities to ask the Patient if they
    //        have taken their medications
    //

extern  NSString  *const  kMomentInDayStepIdentifier;

extern  NSString  *const  kMomentInDayFormat;

extern  NSString  *const  kMomentInDayFormatTitle;

extern  NSString  *const  kMomentInDayFormatItemText;
extern  NSString  *const  kMomentInDayFormatChoiceJustWokeUp;
extern  NSString  *const  kMomentInDayFormatChoiceTookMyMedicine;
extern  NSString  *const  kMomentInDayFormatChoiceEvening;
extern  NSString  *const  kMomentInDayFormatChoiceNone;

    //
    //    key for Parkinson Stashed Question Result
    //        from 'When Did You Take Your Medicine' Pre-Survey Question
    //
extern  NSString  *const  kMomentInDayUserDefaultsKey;
    //
    //    keys for Parkinson Stashed Question Result Dictionary
    //        from 'When Did You Take Your Medicine' Pre-Survey Question
    //
extern  NSString  *const  kMomentInDayChoiceAnswerKey;
extern  NSString  *const  kMomentInDayQuestionTypeKey;
extern  NSString  *const  kMomentInDayIdentifierKey;
extern  NSString  *const  kMomentInDaystartDateKey;
extern  NSString  *const  kMomentInDayendDateKey;


    //
    //    keys for Parkinson Conclusion Step View Controller
    //
extern  NSString  *const  kConclusionStepThankYouTitle;
extern  NSString  *const  kConclusionStepViewDashboard;

@interface APHParkinsonActivityViewController : APCBaseTaskViewController

+ (ORKOrderedTask *)modifyTaskWithPreSurveyStepIfRequired:(ORKOrderedTask *)task andTitle:(NSString *)taskTitle;

@end
