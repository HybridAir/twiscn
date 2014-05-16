//used for holding and handling tweets
//eventually add in the ability to reprint a previous tweet

#include <Arduino.h>

TweetHandler::TweetHandler() { 
//    LCDWIDTH = widthIn;
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

int TweetHandler::getTweetLength() {
    return tweet.length();
}

String TweetHandler::getPrevUser() {
    return prevUser;
}

String TweetHandler::getPrevTweet() {
    return prevTweet;
}

String TweetHandler::getTweetBegin() {                                          //returns the beginning of the tweet
    if(tweet.length() <= LCDWIDTH) {                                            //check if the tweet is less than LCDWIDTH first
        return tweet;                                                           //no need to shorten, just return the unchanged tweet
    } 
    else {                                                                      //needs to be shortened, longer than LCDWIDTH
        beginning = tweet.substring(0, LCDWIDTH - 1);                           //create a substring containing the first LCDWIDTH characters                                  
        return beginning;
    }
}

//==============================================================================

bool TweetHandler::useScroll() {                                                //returns if tweet scrolling is necessary (longer than LCDWIDTH)
    if(tweet.length() <= LCDWIDTH) {
        return false;
    }
    else {
        return true; 
    }
}