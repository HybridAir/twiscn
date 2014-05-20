//used for storing and handling tweets

#include "TweetHandler.h"

TweetHandler::TweetHandler(int widthIn) {                                       //constructor, needs the LCDWIDTH   
    LCDWIDTH = widthIn;
    //set all Strings to empty
    user = "";
    prevUser = "";
    tweet = "";
    prevTweet = "";
}

void TweetHandler::setUser(String in) {                                         //sets the username
    prevUser = user;                                                            //save a copy of the current (now previous) user
    user = in;                                                                  //set the new user
}

void TweetHandler::setTweet(String in) {                                        //sets the tweet text
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

int TweetHandler::getPrevLength() {
    return prevTweet.length();
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
        beginning = tweet.substring(0, LCDWIDTH);                               //create a substring containing the first LCDWIDTH characters                                  
        return beginning;
    }
}

String TweetHandler::getPrevBegin() {                                           //returns the beginning of the previous tweet
    if(prevTweet.length() <= LCDWIDTH) {                                        //check if the tweet is less than LCDWIDTH first
        return tweet;                                                           //no need to shorten, just return the unchanged tweet
    } 
    else {                                                                      //needs to be shortened, longer than LCDWIDTH
        beginning = prevTweet.substring(0, LCDWIDTH);                           //create a substring containing the first LCDWIDTH characters                                  
        return beginning;
    }
}

//==============================================================================

bool TweetHandler::useScroll(bool current) {                                    //returns if tweet scrolling is necessary (longer than LCDWIDTH)
    if(current) {
        if(tweet.length() <= LCDWIDTH) {
            return false;
        }
    }
    else {
        if(prevTweet.length() <= LCDWIDTH) {
            return false;
        }
    }
    return true; 
}