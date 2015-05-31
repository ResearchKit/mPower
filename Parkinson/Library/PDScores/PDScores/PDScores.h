//
//  PDScores.h
//  PDScores
//
//  Created by Erin Mounts on 2/9/15.
//  Copyright (c) 2015 Sage Bionetworks. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PDScores : NSObject

+ (double)scoreFromGaitTest:(NSArray *)gaitData;
+ (double)scoreFromPhonationTest:(NSURL *)phonationAudioFile;
+ (double)scoreFromTappingTest:(NSArray *)tappingData;
+ (double)scoreFromPostureTest:(NSArray *)postureData;

+ (double)scoreFromPhonationTest:(NSURL *)phonationAudioFile gaitTest:(NSArray *)gaitData postureTest:(NSArray *)postureData tappingTest:(NSArray *)tappingData;
+ (double)combinedScoreFromPhonationScore:(double)phonationScore gaitScore:(double)gaitScore postureScore:(double)postureScore tappingScore:(double)tappingScore;

@end
