#ifndef TWEETHANDLER_H
#define	TWEETHANDLER_H

#include <Arduino.h>

class TweetHandler {
    public:
        TweetHandler();
        void setUser(String in);
        void setTweet(String in);
        String getUser();
        String getTweet();
        int getTweetLength();
        String getTweetBegin();
        String getPrevUser();
        String getPrevTweet();
        bool useScroll();
    private:
        const byte LCDWIDTH;
        String user;
        String tweet;
        String prevUser;
        String prevTweet;
        String beginning;
};

#endif	/* TWEETHANDLER_H */

