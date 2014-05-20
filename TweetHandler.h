#ifndef TWEETHANDLER_H
#define	TWEETHANDLER_H

#include <Arduino.h>

class TweetHandler {
    public:
        TweetHandler(int widthIn);
        void setUser(String in);
        void setTweet(String in);     
        int getTweetLength();
        int getPrevLength();
        String getTweetBegin();
        String getPrevBegin();
        String getPrevUser();
        String getPrevTweet();
        String getUser();
        String getTweet();
        bool useScroll(bool current);
    private:
        byte LCDWIDTH;
        String user;
        String tweet;
        String prevUser;
        String prevTweet;
        String beginning;
};

#endif	/* TWEETHANDLER_H */

