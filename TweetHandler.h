#ifndef TWEETHANDLER_H
#define	TWEETHANDLER_H

#include <Arduino.h>

class TweetHandler {
    public:
        TweetHandler(LCDControl lcdin);
        setUser(String in);
        setTweet(String in);
        getUser();
        getTweet();
        getPrevUser();
        getPrevTweet();
        sendTweet();
    private:
        LCDControl lcd;
        String user;
        String tweet;
        String prevUser;
        String prevTweet;
};

#endif	/* TWEETHANDLER_H */

