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
        getTweetBegin();
        getPrevUser();
        getPrevTweet();
        useScroll();
    private:
        LCDControl lcd;
        const byte LCDWIDTH;
        String user;
        String tweet;
        String prevUser;
        String prevTweet;
        String beginning;
};

#endif	/* TWEETHANDLER_H */

