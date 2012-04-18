<?php
require_once('../protected/utils.inc');

get_db();

$novelname = $_POST['tale'];
if (! isset($novelname) || ! verify_id($novelname) ) {
  exit();
}

$when = $_POST['when'];
if (! isset($when) || ! verify_date($when) ) {
  exit();
}

$tweet_text = stripslashes($_POST['text']);
if (! isset($tweet_text) || strlen($tweet_text) > 140 ) {
  exit();
}

?>
<html>
<head>
<title>Adding tweet to <?php print $novelname; ?></title>
</head>
<body>
<pre>
<?php
print "$when\n";
print "$tweet_text\n";
?>
</pre>
<?php
$added = add_tweet_to_novel($novelname, $tweet_text, $when);

if ($added) {
?>
<p>Tweet was added.</p>
<?php
    } else {
?>
<p>Tweet was not added.</p>
<?php
    }
?>
</body>
</html>
