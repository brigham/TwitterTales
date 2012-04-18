<?php
require_once('../protected/utils.inc');

get_db();

$novelname = $_GET['tale'];

if (! isset($novelname) || ! verify_id($novelname) ) {
  exit();
}

$tweetid = $_GET['tweet'];
if (! isset($tweetid) || ! verify_number($tweetid) ) {
  exit();
}

$tale_info = get_novel_data($novelname);

if (count($tale_info) == 0) {
  exit();
}

$twittername = $tale_info['twittername'];
$hashtag = $tale_info['hashtag'];

$tweet_data = pull_one_tweet_from_twitter($novelname, $twittername, $hashtag, $tweetid);
$tweet_text = $tweet_data['text'];
$when = $tweet_data['when'];
?>
<html>
<head>
<title>Adding tweet <?php print $tweetid; ?> to <?php print $novelname; ?></title>
</head>
<body>
<form action="addone.php" method="post">
<input type="hidden" name="tale" value="<?php print $novelname; ?>">
<label for="when">When:</label><input type="text" name="when" 
       value="<?php print $when; ?>"><br>
<label for="text">Text:</label><textarea name="text" cols="40" rows="4"><?php 
       print htmlentities($tweet_text); ?></textarea><br>
<input type="submit" value="Add">
</form>
</body>
</html>
