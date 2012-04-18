<?php
require_once('../protected/utils.inc');

get_db();

$novelname = $_GET['novel'];

if (! isset($novelname) || ! verify_id($novelname) ) {
  exit();
}

$tale_info = get_novel_data($novelname);

if (count($tale_info) == 0) {
  exit();
}

$twittername = $tale_info['twittername'];
$hashtag = $tale_info['hashtag'];
$lastid = $tale_info['lastid'];

?>
<html>
<head>
<title>Updating <?php print $novelname; ?></title>
</head>
<body>
<?php
print "<pre>";
print "$twittername\n";
print "$hashtag\n";
print "$lastid\n";
print "</pre>";

$had_results = load_new_tweets_from_twitter($novelname, $twittername, $hashtag, $lastid);

if ($had_results) {
  if (mail('me@brighambrown.com',
	   "$novelname was updated on Twitter!",
	   "$twittername has updated the novel $novelname on Twitter.\n" .
	   "http://twittertales.com/$novelname\n")) {
    print "Notification email was sent successfully.";
  } else {
    print "Notification email was NOT sent.";
  }
}

?>
</body>
</html>
