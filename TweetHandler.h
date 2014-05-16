#ifndef TWEETHANDLER_H
#define	TWEETHANDLER_H

#include <Arduino.h>

class TweetHandler {
    public:
        TweetHandler(byte widthIn);
        setUser(String in);
        setTweet(String in);
        getUser();
        getTweet();
        getTweetLength();
        getTweetBegin();
        getPrevUser();
        getPrevTweet();
        useScroll();
    private:
        const byte LCDWIDTH;
        String user;
        String tweet;
        String prevUser;
        String prevTweet;
        String beginning;
};

#endif	/* TWEETHANDLER_H */

