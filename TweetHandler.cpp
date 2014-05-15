//used for holding and handling tweets

#include <Arduino.h>

TweetHandler::TweetHandler(LCDControl lcdin) { 
    lcd = lcdin;
}

void TweetHandler::setUser(String in) {
    prevUser = user;                                                            //save a copy of the current (now previous) user
    user = in;                                                                  //set the new user
}

void TweetHandler::setTweet(String in) {
    prevTweet = tweet;                                                          //save a copy of the current (now previous) tweet
    tweet = in;                                                                 //set the new tweet
}

//==============================================================================

String TweetHandler::getUser() {
    return user;
}

String TweetHandler::getTweet() {
    return tweet;
}

String TweetHandler::getPrevUser() {
    return prevUser;
}

String TweetHandler::getPrevTweet() {
    return prevTweet;
}

//==============================================================================

String TweetHandler::sendTweet() {
    if(tweet.length() =< lcd.LCDWIDTH) {
        lcd.printNewTweet(user, tweet);
    }
    else {
        String beginning = tweet.substring(0, 15);
        lcd.printNewTweet(lcd, beginning, tweet); 
    }
}