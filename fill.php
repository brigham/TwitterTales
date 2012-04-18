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
print "</pre>";

load_new_tweets_from_twitter($novelname, $twittername, $hashtag, 0);

?>
</body>
</html>
