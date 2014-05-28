//used to handle most twitter related functionality
package twiscnhost;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import twitter4j.*;

public class TweetHandler {
    
    Options opt;
    DeviceHandler twiScn;
    TwitterStream twitterStream;
    Twitter twitter;
   
    public TweetHandler(Options opt, DeviceHandler twiScn) {
        this.opt = opt;
        this.twiScn = twiScn;
        twitter = new TwitterFactory().getInstance();
        twitterStream = new TwitterStreamFactory().getInstance();
        twitterStream.addListener(listener);
        
        sendLatestTweet();
    }
    
    public String[] getLatestTweet() {
        try {
            Query query = new Query(getUserQuery());
            QueryResult result = twitter.search(query);
            List<Status> statuses = result.getTweets();
            Status status = statuses.get(0);
            return formatTweet(status);
        } catch (TwitterException te) {
            //te.printStackTrace();
            System.out.println("Failed to get latest tweet: " + te.getMessage());
            return null;
        }
    }
    
    private String[] formatTweet(Status status) {
            Date createdAt = status.getCreatedAt();
            SimpleDateFormat ft = new SimpleDateFormat (" hh:mm a");
            String user = status.getUser().getScreenName() + ":";
            String text = status.getText() + " " + ft.format(createdAt);
            String[] out = {user, text};
            return out;
    }
    
    public void sendLatestTweet() {
        String[] tweet = getLatestTweet();
        if(tweet != null) {
            twiScn.newTweet(tweet[0], tweet[1]);
        }
    }
  
    public long getUserID(String screenName) throws TwitterException {
        //try {
            User user = twitter.showUser(screenName);
            return user.getId();
        //} 
        //catch (TwitterException te) {
        //    te.printStackTrace();
        //    return 0;
        //}
    }
    
    public String getScreenName(long ID) throws TwitterException {
        User user = twitter.showUser(ID);
        return user.getScreenName();
    }
    
    private String getUserQuery() throws TwitterException {      
        long[] users = opt.getFollowUsers(); 
        String out = "";
        for(int i = 0;i <= users.length - 1;i++) { 
            out += "from:" + getScreenName(users[i]);
            if(i < users.length - 1) {                                             //if we are not on the last ID
                out += " OR ";                                                     //add a , to be used as a delimiter
            }
        }
        return out;
    }
    
//==============================================================================
    
    public void startStream() {
        twitterStream.filter(new FilterQuery(opt.getFollowUsers()));    
        System.out.println("TwitterStream started");
    }
    
    public void stopStream() {
        twitterStream.shutdown();
        System.out.println("TwitterStream stopped");
    }
    
     private static final UserStreamListener listener = new UserStreamListener() {
        @Override
        public void onStatus(Status status) {
            System.out.println("onStatus @" + status.getUser().getScreenName() + " - " + status.getText());
        }

        @Override
        public void onDeletionNotice(StatusDeletionNotice statusDeletionNotice) {
            System.out.println("Got a status deletion notice id:" + statusDeletionNotice.getStatusId());
        }

        @Override
        public void onDeletionNotice(long directMessageId, long userId) {
            System.out.println("Got a direct message deletion notice id:" + directMessageId);
        }

        @Override
        public void onTrackLimitationNotice(int numberOfLimitedStatuses) {
            System.out.println("Got a track limitation notice:" + numberOfLimitedStatuses);
        }

        @Override
        public void onScrubGeo(long userId, long upToStatusId) {
            System.out.println("Got scrub_geo event userId:" + userId + " upToStatusId:" + upToStatusId);
        }

        @Override
        public void onStallWarning(StallWarning warning) {
            System.out.println("Got stall warning:" + warning);
        }

        @Override
        public void onFriendList(long[] friendIds) {
            System.out.print("onFriendList");
            for (long friendId : friendIds) {
                System.out.print(" " + friendId);
            }
            System.out.println();
        }

        @Override
        public void onFavorite(User source, User target, Status favoritedStatus) {
            System.out.println("onFavorite source:@"
                    + source.getScreenName() + " target:@"
                    + target.getScreenName() + " @"
                    + favoritedStatus.getUser().getScreenName() + " - "
                    + favoritedStatus.getText());
        }

        @Override
        public void onUnfavorite(User source, User target, Status unfavoritedStatus) {
            System.out.println("onUnFavorite source:@"
                    + source.getScreenName() + " target:@"
                    + target.getScreenName() + " @"
                    + unfavoritedStatus.getUser().getScreenName()
                    + " - " + unfavoritedStatus.getText());
        }

        @Override
        public void onFollow(User source, User followedUser) {
            System.out.println("onFollow source:@"
                    + source.getScreenName() + " target:@"
                    + followedUser.getScreenName());
        }

        @Override
        public void onUnfollow(User source, User followedUser) {
            System.out.println("onFollow source:@"
                    + source.getScreenName() + " target:@"
                    + followedUser.getScreenName());
        }

        @Override
        public void onDirectMessage(DirectMessage directMessage) {
            System.out.println("onDirectMessage text:"
                    + directMessage.getText());
        }

        @Override
        public void onUserListMemberAddition(User addedMember, User listOwner, UserList list) {
            System.out.println("onUserListMemberAddition added member:@"
                    + addedMember.getScreenName()
                    + " listOwner:@" + listOwner.getScreenName()
                    + " list:" + list.getName());
        }

        @Override
        public void onUserListMemberDeletion(User deletedMember, User listOwner, UserList list) {
            System.out.println("onUserListMemberDeleted deleted member:@"
                    + deletedMember.getScreenName()
                    + " listOwner:@" + listOwner.getScreenName()
                    + " list:" + list.getName());
        }

        @Override
        public void onUserListSubscription(User subscriber, User listOwner, UserList list) {
            System.out.println("onUserListSubscribed subscriber:@"
                    + subscriber.getScreenName()
                    + " listOwner:@" + listOwner.getScreenName()
                    + " list:" + list.getName());
        }

        @Override
        public void onUserListUnsubscription(User subscriber, User listOwner, UserList list) {
            System.out.println("onUserListUnsubscribed subscriber:@"
                    + subscriber.getScreenName()
                    + " listOwner:@" + listOwner.getScreenName()
                    + " list:" + list.getName());
        }

        @Override
        public void onUserListCreation(User listOwner, UserList list) {
            System.out.println("onUserListCreated  listOwner:@"
                    + listOwner.getScreenName()
                    + " list:" + list.getName());
        }

        @Override
        public void onUserListUpdate(User listOwner, UserList list) {
            System.out.println("onUserListUpdated  listOwner:@"
                    + listOwner.getScreenName()
                    + " list:" + list.getName());
        }

        @Override
        public void onUserListDeletion(User listOwner, UserList list) {
            System.out.println("onUserListDestroyed  listOwner:@"
                    + listOwner.getScreenName()
                    + " list:" + list.getName());
        }

        @Override
        public void onUserProfileUpdate(User updatedUser) {
            System.out.println("onUserProfileUpdated user:@" + updatedUser.getScreenName());
        }

        @Override
        public void onBlock(User source, User blockedUser) {
            System.out.println("onBlock source:@" + source.getScreenName()
                    + " target:@" + blockedUser.getScreenName());
        }

        @Override
        public void onUnblock(User source, User unblockedUser) {
            System.out.println("onUnblock source:@" + source.getScreenName()
                    + " target:@" + unblockedUser.getScreenName());
        }

        @Override
        public void onException(Exception ex) {
            ex.printStackTrace();
            System.out.println("onException:" + ex.getMessage());
        }
    };
     
}