//used to handle most twitter related functionality
package twiscnhost;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.logging.Level;
import twitter4j.*;

public class TweetHandler {
    
    private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger(LogHandler.class.getName());
    Options opt;
    DeviceHandler twiScn;
    TwitterStream twitterStream;
    Twitter twitter;
   
    public TweetHandler(Options opt, DeviceHandler twiScn) {                    //constructor, needs the Options and DeviceHandler instances
        this.opt = opt;
        this.twiScn = twiScn;
        twitter = new TwitterFactory().getInstance();
        twitterStream = new TwitterStreamFactory().getInstance();               //create a new instance of TwitterStream
        twitterStream.addListener(listener);                                    //add our listener to it
        
        sendLatestTweet();                                                      //try to send the latest tweet to the device
        startStream();
    }
    
    private String[] getLatestTweet() throws TwitterException {                 //returns a String array containing the latest tweet, throws TwitterException
        Query query = new Query(getUserQuery());                                //try to get a formatted query string created (will throw TwitterException if it fails)
        QueryResult result = twitter.search(query);                             //try to get a list of recent tweets (will throw TwitterException if it fails)
        List<Status> statuses = result.getTweets();                             //get the returned tweets into a List of statuses (will throw TwitterException if it fails)
        Status status = statuses.get(0);                                        //get the first status out, it's the latest one
        return formatTweet(status);                                             //format the status and return the result (throws TwitterException if it fails)
    }
    
    private String[] formatTweet(Status status) {                               //returns a formatted String array, needs a valid Status
        Date createdAt = status.getCreatedAt();                                 //get the Date the status was created on
        SimpleDateFormat ft = new SimpleDateFormat (" hh:mm a");                //prepare to reformat that date
        String user = status.getUser().getScreenName() + ":";                   //get the screenName out of the status and add a ":" to it
        String text = status.getText() + " " + ft.format(createdAt);            //get the tweet text out of the status and add the formatted Date to the end
        String[] out = {user, text};                                            //prepare a String array containing the above
        return out;
    }
    
    private void sendLatestTweet() {                                            //tries to send the latest tweet to the device
        try {
            String[] tweet = getLatestTweet();                                  //get the latest tweet into a new array (will catch TwitterException if it fails)
            twiScn.newTweet(tweet[0], tweet[1]);                                //send the tweet to the device
        }
        catch (TwitterException te) {                                           //catch any TwitterException that could have happened here
            logger.log(Level.WARNING, "Failed to send Latest Tweet", te);
            //cant send anything new to the device, so pretend this never happened
            //might want to kill the program with an error actually, could be a twitter problem
        }
    }
    
    private void sendTweet(Status status) {                                     //tries to send the latest tweet to the device
            String[] tweet = formatTweet(status);                               //get the latest tweet into a new array (will catch TwitterException if it fails)
            twiScn.newTweet(tweet[0], tweet[1]);                                //send the tweet to the device
    }
    
//==============================================================================
  
    public long getUserID(String screenName) throws TwitterException {          //returns the user's userID, needs their screenName, and throws TwitterException
        User user = twitter.showUser(screenName);                               //get the user info from showUser, and set it to a new User (throws TwitterException if it fails)
        return user.getId();                                                    //get the userID out of the User and return it
    }
    
    public String getScreenName(long ID) throws TwitterException {              //returns the user's screenName, needs their userID, and throws TwitterException
        User user = twitter.showUser(ID);                                       //get the user info from showUser, and set it to a new User (throws TwitterException if it fails)
        return user.getScreenName();                                            //get the screenName out of the User and return it
    }
    
    private String getUserQuery() throws TwitterException {                     //returns a formatted query string, throws TwitterException   
        //used for getting a query that returns the latest tweet from a group of users
        long[] users = opt.getFollowUsers();                                    //get the array of users
        String out = "";
        for(int i = 0;i <= users.length - 1;i++) {                              //for each user in there
            out += "from:" + getScreenName(users[i]);                           //try to get the user's screenName (throws TwitterException if it fails)
            if(i < users.length - 1) {                                          //if we are not on the last user
                out += " OR ";                                                  //add an OR in between for use by the query thing
            }
        }
        return out;
    }
    
//==============================================================================
    
    public void startStream() {                                                 //starts the twitterStream thread
        twitterStream.filter(new FilterQuery(opt.getFollowUsers()));    
        logger.log(Level.INFO, "TwitterStream started");
    }
    
    public void stopStream() {                                                  //stops the twitterStream thread
        twitterStream.shutdown();
        logger.log(Level.INFO, "TwitterStream stopped");
    }
    
    private final UserStreamListener listener = new UserStreamListener() {      //UserStreamListener implementation
        @Override
        public void onStatus(Status status) {
            //System.out.println("onStatus @" + status.getUser().getScreenName() + " - " + status.getText());
            sendTweet(status);
        }

        @Override
        public void onDeletionNotice(StatusDeletionNotice statusDeletionNotice) {
            //System.out.println("Got a status deletion notice id:" + statusDeletionNotice.getStatusId());
            sendLatestTweet();
        }

        @Override
        public void onDeletionNotice(long directMessageId, long userId) {
            //System.out.println("Got a direct message deletion notice id:" + directMessageId);
        }

        @Override
        public void onTrackLimitationNotice(int numberOfLimitedStatuses) {
            //System.out.println("Got a track limitation notice:" + numberOfLimitedStatuses);
        }

        @Override
        public void onScrubGeo(long userId, long upToStatusId) {
            //System.out.println("Got scrub_geo event userId:" + userId + " upToStatusId:" + upToStatusId);
        }

        @Override
        public void onStallWarning(StallWarning warning) {
            //System.out.println("Got stall warning:" + warning);
        }

        @Override
        public void onFriendList(long[] friendIds) {
            //System.out.print("onFriendList");
            //for (long friendId : friendIds) {
               // System.out.print(" " + friendId);
            //}
            //System.out.println();
        }

        @Override
        public void onFavorite(User source, User target, Status favoritedStatus) {
            //System.out.println("onFavorite source:@"
            //        + source.getScreenName() + " target:@"
            //        + target.getScreenName() + " @"
            //        + favoritedStatus.getUser().getScreenName() + " - "
            //        + favoritedStatus.getText());
        }

        @Override
        public void onUnfavorite(User source, User target, Status unfavoritedStatus) {
            //System.out.println("onUnFavorite source:@"
            //        + source.getScreenName() + " target:@"
            //        + target.getScreenName() + " @"
            //        + unfavoritedStatus.getUser().getScreenName()
            //        + " - " + unfavoritedStatus.getText());
        }

        @Override
        public void onFollow(User source, User followedUser) {
            //System.out.println("onFollow source:@"
            //        + source.getScreenName() + " target:@"
            //        + followedUser.getScreenName());
        }

        @Override
        public void onUnfollow(User source, User followedUser) {
            //System.out.println("onFollow source:@"
            //        + source.getScreenName() + " target:@"
            //        + followedUser.getScreenName());
        }

        @Override
        public void onDirectMessage(DirectMessage directMessage) {
            //System.out.println("onDirectMessage text:"
            //        + directMessage.getText());
        }

        @Override
        public void onUserListMemberAddition(User addedMember, User listOwner, UserList list) {
            //System.out.println("onUserListMemberAddition added member:@"
            //        + addedMember.getScreenName()
            //        + " listOwner:@" + listOwner.getScreenName()
            //        + " list:" + list.getName());
        }

        @Override
        public void onUserListMemberDeletion(User deletedMember, User listOwner, UserList list) {
            //System.out.println("onUserListMemberDeleted deleted member:@"
            //        + deletedMember.getScreenName()
            //       + " listOwner:@" + listOwner.getScreenName()
            //        + " list:" + list.getName());
        }

        @Override
        public void onUserListSubscription(User subscriber, User listOwner, UserList list) {
            //System.out.println("onUserListSubscribed subscriber:@"
            //        + subscriber.getScreenName()
            //        + " listOwner:@" + listOwner.getScreenName()
            //        + " list:" + list.getName());
        }

        @Override
        public void onUserListUnsubscription(User subscriber, User listOwner, UserList list) {
            //System.out.println("onUserListUnsubscribed subscriber:@"
            //        + subscriber.getScreenName()
            //        + " listOwner:@" + listOwner.getScreenName()
            //        + " list:" + list.getName());
        }

        @Override
        public void onUserListCreation(User listOwner, UserList list) {
            //System.out.println("onUserListCreated  listOwner:@"
            //        + listOwner.getScreenName()
            //        + " list:" + list.getName());
        }

        @Override
        public void onUserListUpdate(User listOwner, UserList list) {
            //System.out.println("onUserListUpdated  listOwner:@"
            //        + listOwner.getScreenName()
            //        + " list:" + list.getName());
        }

        @Override
        public void onUserListDeletion(User listOwner, UserList list) {
            //System.out.println("onUserListDestroyed  listOwner:@"
            //        + listOwner.getScreenName()
            //        + " list:" + list.getName());
        }

        @Override
        public void onUserProfileUpdate(User updatedUser) {
            //System.out.println("onUserProfileUpdated user:@" + updatedUser.getScreenName());
        }

        @Override
        public void onBlock(User source, User blockedUser) {
            //System.out.println("onBlock source:@" + source.getScreenName()
            //        + " target:@" + blockedUser.getScreenName());
        }

        @Override
        public void onUnblock(User source, User unblockedUser) {
            //System.out.println("onUnblock source:@" + source.getScreenName()
            //        + " target:@" + unblockedUser.getScreenName());
        }

        @Override
        public void onException(Exception ex) {
            logger.log(Level.WARNING, "TwitterStream got an exception", ex);
        }
    };     
}