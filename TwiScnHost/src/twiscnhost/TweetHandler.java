//used to handle most twitter related functionality
package twiscnhost;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.logging.Level;
import twitter4j.*;

public class TweetHandler {
    
    private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger(LogHandler.class.getName());
    private Options opt;
    private DeviceHandler twiScn;
    private TwitterStream twitterStream;
    private Twitter twitter;
    private boolean stopped = false;
   
    public TweetHandler(Options opt, DeviceHandler twiScn) {                    //constructor, needs the Options and DeviceHandler instances
        this.opt = opt;
        this.twiScn = twiScn;
        twitter = new TwitterFactory().getInstance();
        twitterStream = new TwitterStreamFactory().getInstance();               //create a new instance of TwitterStream
        twitterStream.addListener(listener);                                    //add our listener to it   
    }
    
    public void init() {                                                        //used to resend the latest tweet and restart the stream
        sendLatestTweet();                                                      //try to send the latest tweet to the device
        //startStream();                                                          //start the twitterstream thread
                twitterStream.filter(new FilterQuery(getFollowUsers()));    
        logger.log(Level.INFO, "TwitterStream started");
    }
    
    public long[] getFollowUsers() {                                           //used to get the array of userIDs to follow, and check if they're protected
        long[] in = opt.getFollowUsers();                                       //get the current array of IDs out
        for(int i = 0;i < in.length;i++) {                                      //for each ID
            try {
                if(getProtected(in[i])) {                                       //try checking if the ID is protected
                    logger.log(Level.WARNING, "userID " + in[i] + " is protected, removing from list.");
                    opt.delFollowUser(in[i]);                                   //remove the bad ID from the array if it is
                }
            } catch (TwitterException e) {                                      //need to catch any twitter errors
                logger.log(Level.WARNING, "Failed to check if userID is protected", e);
                System.exit(0);                                                 //chances are twitter is not accessible, so kill the program
            } catch(IllegalStateException e) {                                  //check if there is a problem with the properties
                logger.log(Level.WARNING, "Unable to login to Twitter, check twitter4j.properties", e);
                System.exit(0);
            }
        }
        return opt.getFollowUsers();                                            //return the updated array
    }
    
    private String[] getLatestTweet() throws TwitterException {                 //returns a String array containing the latest tweet, throws TwitterException
        Query query = new Query(getUserQuery());                                //try to get a formatted query string created (will throw TwitterException if it fails)
        QueryResult result = twitter.search(query);                             //try to get a list of recent tweets (will throw TwitterException if it fails)
        List<Status> statuses = result.getTweets();                             //get the returned tweets into a List of statuses (will throw TwitterException if it fails)
        Status status = statuses.get(0);                                        //get the first status out, it's the latest one
        return formatTweet(status);                                             //format the status and return the result (throws TwitterException if it fails)
    }
    
    public Date getLastTweetDate(String userName) throws TwitterException {     //returns the date of a user's last tweet, throws TwitterException
        Query query = new Query("from:" + userName);                            //try to get a formatted query string created (will throw TwitterException if it fails)
        QueryResult result = twitter.search(query);                             //try to get a list of recent tweets (will throw TwitterException if it fails)
        List<Status> statuses = result.getTweets();                             //get the returned tweets into a List of statuses (will throw TwitterException if it fails)
        try {
            Status status = statuses.get(0);                                    //get the first status out, it's the latest one      
            return status.getCreatedAt();                                       //format the status and return the result (throws TwitterException if it fails)
        }
        catch(IndexOutOfBoundsException e) {                                    //user has not made any tweets, can't return a date
            return null;
        }
    }
    
    private String[] formatTweet(Status status) {                               //returns a formatted String array, needs a valid Status
        Date createdAt = status.getCreatedAt();                                 //get the Date the status was created on
        SimpleDateFormat ft = new SimpleDateFormat ("hh:mm a");                //prepare to reformat that date
        String user = status.getUser().getScreenName() + ":";                   //get the screenName out of the status and add a ":" to it
        String text = status.getText() + " " + ft.format(createdAt);            //get the tweet text out of the status and add the formatted Date to the end
        String[] out = {user, text};                                            //prepare a String array containing the above
        return out;
    }
    
    public void sendLatestTweet() {                                            //tries to send the latest tweet to the device
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
    
    public boolean checkExists(String userName) {                               //used to check if a user exists
        try{
            twitter.showUser(getUserID(userName));                              //try to get the user
            return true;                                                        //got it, user exists
        }
        catch(TwitterException ex){                                             //catch any twitter errors
            if(ex.getStatusCode() == 404){                                      //if the error code is 404, the user doesn't exist
                return false;
            } 
            else {                                                              //we have bigger problems if it wasn't just a 404
                java.util.logging.Logger.getLogger(Gui.class.getName()).log(Level.SEVERE, null, ex);
                return false;
            }
        }   
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
        long[] users = getFollowUsers();                                        //get the array of users
        String out = "";
        for(int i = 0;i <= users.length - 1;i++) {                              //for each user in there
            out += "from:" + getScreenName(users[i]);                           //try to get the user's screenName (throws TwitterException if it fails)
            if(i < users.length - 1) {                                          //if we are not on the last user
                out += " OR ";                                                  //add an OR in between for use by the query thing
            }
        }
        return out;
    }
    
    public boolean getProtected(long ID) throws TwitterException {
        User user = twitter.showUser(ID);                                       //get the user info from showUser, and set it to a new User (throws TwitterException if it fails)
        return user.isProtected();
    }
    
//==============================================================================
    
    public void startStream() {                                                 //starts the twitterStream thread
        stopped = false;
    }
    
    public void stopStream() {                                                  //stops the twitterStream thread
        stopped = true;
        //twitterStream.shutdown();
        //logger.log(Level.INFO, "TwitterStream stopped");
    }
    
    private boolean correctUser(Status status) {                                //used to check if a tweet was made by the correct user
        boolean checking = true;
        long[] following = getFollowUsers();
        
        while(checking) {                                                       //while we are still checking
            for(int i = 0;i < following.length; i++) {                          //for each user the program is following
                String user = status.getUser().getScreenName();                 //get the screenname of the user who created the tweet
                String checkUser = "";                                          //create this now to get it out of the way
                try {
                    checkUser = getScreenName(following[i]);                    //try to get the screenname of the current user we are checking
                } catch (TwitterException ex) {
                    java.util.logging.Logger.getLogger(TweetHandler.class.getName()).log(Level.SEVERE, null, ex);
                }                
                if(user.equalsIgnoreCase(checkUser)) {                          //if the usernames match
                    return true;                                                //user is correct, return here
                }
            }
        }
        return false;                                                           //assuming false at this point, return it
            
    }
    
    private final UserStreamListener listener = new UserStreamListener() {      //UserStreamListener implementation
        @Override
        public void onStatus(Status status) {
            //System.out.println("onStatus @" + status.getUser().getScreenName() + " - " + status.getText());
            if(!stopped) {
                
                //check if the tweet was sent by the correct user first
                if(correctUser(status)) { 
                    sendTweet(status);
                }
            }
        }

        @Override
        public void onDeletionNotice(StatusDeletionNotice statusDeletionNotice) {
            //System.out.println("Got a status deletion notice id:" + statusDeletionNotice.getStatusId());
            if(!stopped) {
                sendLatestTweet();
            }
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